#include <boost/format.hpp>
#include "renderdownsample.h"
#include "types.h"

namespace wavevis2 {

int renderdownsample_compare_timeid(Db *db, const Dbt *a, const Dbt *b)
{
  // Returns:
  // < 0 if a < b
  // = 0 if a = b
  // > 0 if a > b
  timeid_t ad; 
  timeid_t bd; 
  memcpy(&ad, a->get_data(), sizeof(a));
  memcpy(&bd, b->get_data(), sizeof(b));
  
  if (ad < bd) return -1; 
  if (ad > bd) return 1;
  return 0; 
}


RenderDownSample::RenderDownSample(timeid_t sampledur, size_t binsize, 
				   bf::path scratch) :
  sampledur_(sampledur),
  //  binsize_(binsize),
  bindur_(binsize * sampledur),
  dbEnv_(0)
{
  dbEnv_.set_cachesize(0, 1024 * 1024 * 5, 1); 

  assert(binsize_ <= GLPointBuffer_t::BUFFERN); 
  
  u_int32_t env_flags = DB_CREATE |     // If the environment does not
    DB_INIT_MPOOL |
    DB_INIT_CDB |
    DB_THREAD ; // Initialize the in-memory cache.
  
  boost::format envname("renderdownsample_%d_%d"); 
  envname % sampledur % binsize; 

  bf::path envHome = scratch / boost::str(envname);
  boost::filesystem::remove_all(envHome); 
  boost::filesystem::create_directories(envHome);
  
  try {
    dbEnv_.open(envHome.string().c_str(), env_flags, 0);
  } catch(DbException &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } catch(std::exception &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } 

  db_ = new Db(&dbEnv_, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |   DB_THREAD; // Open flags;
  
  std::string dbname("my_db.db"); 
  boost::filesystem::remove_all(dbname );
  
  db_->set_pagesize(1<<13); 
  db_->set_bt_compare(renderdownsample_compare_timeid);
  db_->open(NULL,                // Transaction pointer
	  dbname.c_str(),          // Database file name
	  NULL,                // Optional logical database name
	  DB_BTREE,            // Database access method
	  oFlags,              // Open flags
	  0);                  // File mode (using defaults)

  
}


void RenderDownSample::newSample(WaveBuffer_t & wb) {
  
  /* 
     We will need to entertain the hypothesis that
     we might cross buffers over the course of 
     a particular wave buffer. 
   */ 
  
  // first, get the start time of the bin that this buffer
  // is in: 



  double period = 1/wb.samprate; 
  timeid_t period_ns = timeid_t(period * 1e9); 

  Dbc * cursorp; 


  GLPointBuffer_t * pb; 

  int ret = db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
  if (ret != 0 ) {
    throw std::runtime_error("inconsistency between presentBins_ and database"); 
  }

  
  bool currentBuffer = false; 

  typedef std::map<timeid_t, GLPointBuffer_t *> cachemap_t; 
  cachemap_t cachemaps; 

  for (int sample_pos = 0; sample_pos < wb.data.size(); sample_pos++) { 
    timeid_t sample_time = (period_ns * sample_pos) + wb.time; 
    
    timeid_t sample_time_bin = getBinStart(sample_time); 
    
    // find the queue in the cache 

    cachemap_t::iterator bmi = cachemaps.find(sample_time_bin); 
    if (bmi == cachemaps.end()) {
      // Cache miss, open it! 

      if (presentBins_.find(sample_time_bin) == presentBins_.end()) {
	// NEW BIN, set the data
	pb = new GLPointBuffer_t; 
	pb->size = 0; 
	for(int i = 0; i < GLPointBuffer_t::BUFFERN; i++) {

	  pb->data[i].t = double(sampledur_)/1e9 * i;
	  
	}

	presentBins_.insert(sample_time_bin); 

	
      } else { 
	// the bin exists currently, so load it into the cache
	pb = new GLPointBuffer_t; 

	Dbt search_key(&sample_time_bin, sizeof(sample_time_bin));
	Dbt found_data(pb, sizeof(GLPointBuffer_t)); 
        
	int ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);
	if (ret != 0 ) {
	  throw std::runtime_error("inconsistency between presentBins_ and database"); 
	}
    
	// copy into our space for mutation!
	memcpy(pb, found_data.get_data(), sizeof(GLPointBuffer_t)); 

	
      }
      // insert into the cache: 
      cachemaps.insert(std::make_pair(sample_time_bin, pb)); 

    } else { 
      pb = bmi->second; 
    }
    

    size_t pb_pos =  (sample_time - sample_time_bin) / sampledur_; 
    assert(pb_pos < GLPointBuffer_t::BUFFERN); 
    
    pb->data[pb_pos].x = wb.data[sample_pos]; 
    if(pb_pos + 1 > pb->size) 
      pb->size = pb_pos +1; 

    
  }

  // flush the cache to disk: 
  cachemap_t::iterator bmi; 
  for(bmi = cachemaps.begin(); bmi != cachemaps.end(); ++bmi) { 
    timeid_t time = bmi->first; 

    Dbt key(&time, sizeof(time));
    Dbt data(bmi->second, sizeof(GLPointBuffer_t)); 
    
    int ret = cursorp->put(&key, &data, DB_KEYFIRST); 
    if (ret != 0) { 
      std::cout << "Error putting " << std::endl; 
    }

    delete bmi->second; 
  }

  cursorp->close(); 

}

void RenderDownSample::renderStream(timeid_t t1, timeid_t t2, int pixels)
{
  /* Must be thread-safe!
   */
   
  shared_lock_t trunclock(truncate_mutex_); 
    
  Dbc *cursorp;
  
  timeid_t searchval = t1; 
  Dbt search_key(&searchval, sizeof(searchval)); 
  Dbt found_data; 
  
  
  db_->cursor(NULL, &cursorp, 0); 

  int ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);
  if (ret == 0 ) { 
    ret = cursorp->get(&search_key, &found_data, DB_PREV); 
    if (ret != 0) { 
      ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);
    }

    
    while ((ret == 0) and (*((timeid_t*)search_key.get_data()) < t2)) {
      timeid_t buftime = *((timeid_t*)search_key.get_data()) ; 
      
      GLPointBuffer_t * bufptr; 
      bufptr = (GLPointBuffer_t * )found_data.get_data(); 
      renderGLPointBuffer(buftime - t1, bufptr); 
      ret = cursorp->get(&search_key, &found_data, DB_NEXT);
    } 
  }
  cursorp->close(); 
  

}

void RenderDownSample::renderGLPointBuffer(timeid_t origintime, 
				    GLPointBuffer_t * bufptr)
{
  double originf = double(origintime) / 1e9; 
  glPushMatrix();
  glTranslatef(originf, 0, 0); 
  glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), &(bufptr->data[0]) ); 
  glDrawArrays(GL_LINE_STRIP, 0, bufptr->size); 
  glPopMatrix(); 
}


void RenderDownSample::reset() {
  /* 
     Instead of truncating, we delete all the records; ug, slow. 
  */ 

  upgrade_lock_t reqlock(truncate_mutex_);
  // get exclusive access
  up_unique_lock_t requniqueLock(reqlock);
  std::cout << "RenderDownSample::reset() " << std::endl; 
  presentBins_.clear();
  uint32_t x; 
//   db_->close(DB_NOSYNC); 

//   std::string dbname("my_db2.db"); 

// //   db_->remove(dbname.c_str(), NULL, 0); 
//   delete db_;  
  
//   db_ = new Db(&dbEnv_, 0);               // Instantiate the Db object
  
//   //   //  db_->remove(); 
//   u_int32_t oFlags = DB_CREATE |  DB_THREAD; // Open flags;

  
//   //   boost::filesystem::remove_all(dbname );
  
//   int ret = db_->open(NULL,                // Transaction pointer
// 		      dbname.c_str(),          // Database file name
// 		      NULL,                // Optional logical database name
// 		      DB_BTREE,            // Database access method
// 		      oFlags,              // Open flags
// 		      0);                  // File mode (using defaults)
  
//   if(ret != 0) {
//     std::cout << "ERROR OPENING DATABASE" << std::endl;
//   }

  db_->truncate(0, &x, 0); 
  std::cout << "RenderDownSample::reset() done " << std::endl; 

//   // clear the entire database! 
//   timeid_t tid= 0; 
//   Dbt key(&tid, sizeof(tid));
//   GLPointBuffer_t pb; 
//   Dbt data(&pb, sizeof(GLPointBuffer_t)); 
//   Dbc * cursorp; 
//   db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
//   int ret; 
//   while ((ret = cursorp->get(&key, &data, 
// 			     DB_SET)) == 0) {
//     cursorp->del(0);
//   }
//   cursorp->close(); 


}

RenderDownSample::~RenderDownSample()
{

  delete db_; 

}

timeid_t RenderDownSample::getBinStart(timeid_t tid) {
  /* return the starting time for the bin that contains time tid; 
     
     Note that for negative values, the bin is the one over range
     [-bindur_, 0)
  */
  

  timeid_t val = tid / bindur_; 
  if (tid < 0) { 
    val--;
  }

  return val * bindur_; 

}


}
