#include "renderdownsample.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>

int compare_double(Db *db, const Dbt *a, const Dbt *b)
{
  // Returns:
  // < 0 if a < b
  // = 0 if a = b
  // > 0 if a > b
  double ad; 
  double bd; 
  memcpy(&ad, a->get_data(), sizeof(ad));
  memcpy(&bd, b->get_data(), sizeof(bd));
  
  if (ad < bd) return -1; 
  if (ad > bd) return 1;
  return 0; 
}

RenderDownSample::RenderDownSample(float detail, bf::path scrachdir) :
  detail_(detail), 
  NEWBUFTIME_(100*detail)
{
  pdb_ = new Db(NULL, 0); 
  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;

  boost::format dbpath("renderdownsample_%f.db");
  dbpath % detail; 
  
  pdb_->set_pagesize(1<<16); 
  pdb_->set_bt_compare(compare_double);
  int ret = pdb_->open(NULL,                // Transaction pointer
		       boost::str(dbpath).c_str(), 
		       NULL,                // Optional logical database name
		       DB_BTREE,            // Database access method
		       oFlags,              // Open flags
		       0);                  // File mode (using defaults)
  // DbException is not subclassed from std::exception, so
  // need to catch both of these.
  assert(ret == 0); 
  buffer_.size = 0; 
}

RenderDownSample::~RenderDownSample()
{
  delete pdb_; 

}

void RenderDownSample::renderGLPointBuffer(double origintime, 
					   GLPointBuffer_t  * bufptr)
{
    glPushMatrix();
    glTranslatef(origintime, 0, 0); 
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), &(bufptr->data[0]) ); 
    glDrawArrays(GL_LINE_STRIP, 0, bufptr->size); 

    glPopMatrix(); 
}


void RenderDownSample::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  size_t x = 0; 
  size_t dp = 0; 

  Dbc *cursorp;
  pdb_->cursor(NULL, &cursorp, 0); 
  double searchval = t1 - 1; 
  Dbt search_key(&searchval, sizeof(searchval)); 
  Dbt found_data; 

  int ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);
  if (ret == DB_NOTFOUND) { 
    ret = cursorp->get(&search_key, &found_data, DB_LAST);
    
  } else { 
    ret = cursorp->get(&search_key, &found_data, DB_PREV);
    if (ret == DB_NOTFOUND ) { // already first record
    ret = cursorp->get(&search_key, &found_data, DB_FIRST);
    }
  }

  while ((ret != DB_NOTFOUND) and *((double *)(search_key.get_data())) < t2) {

    GLPointBuffer_t * bufptr; 
    bufptr = (GLPointBuffer_t * )found_data.get_data(); 
    renderGLPointBuffer(*((double*)search_key.get_data()) - t1, 
			bufptr); 

    ret = cursorp->get(&search_key, &found_data, DB_NEXT);
  }
  
  cursorp->close(); 

  if ((bufstarttime_ < t2) and (buffer_.size > 0)
       and ((bufstarttime_ + buffer_.data[buffer_.size -1].t) > t1)) {
    renderGLPointBuffer(bufstarttime_ - t1, &buffer_);     
  }

}


void RenderDownSample::newSample(const WaveBuffer_t & wb)
{
  double datumtime = wb.time; 
  double T = 1.0 / wb.samprate; 

  BOOST_FOREACH(float x, wb.data) { 
    newDataPoint(datumtime, x); 
    datumtime += T; 
  }

}

void RenderDownSample::newDataPoint(double streamtime, float data)
{
  
  if (buffer_.size == GLPointBuffer_t::BUFFERN) {
    // put in the buffer
    

//     std::cout << "putting with key " << bufstarttime_ << std::endl; 
    Dbt key(&bufstarttime_, sizeof(bufstarttime_));
    Dbt data(&buffer_, sizeof(buffer_)); 
    int ret = pdb_->put(NULL, &key, &data, DB_NOOVERWRITE);
    if (ret != 0) {
      pdb_->err(ret, "Put failed because key %f already exists", streamtime);
    }
    bufstarttime_ = streamtime; 
    buffer_.size = 0; 
    
  }
  
  if (buffer_.size == 0) {
    // first data point
    buffer_.data[buffer_.size].t =  streamtime - bufstarttime_; 
    buffer_.data[buffer_.size].x = data; 
    buffer_.size++; 
  } else { 
    double delta = (streamtime - bufstarttime_) - buffer_.data[buffer_.size-1].t; 
    if (delta > detail_) { 
      buffer_.data[buffer_.size].t = streamtime - bufstarttime_; 
      buffer_.data[buffer_.size].x = data; 
      buffer_.size++; 
    }
    
  }
}

