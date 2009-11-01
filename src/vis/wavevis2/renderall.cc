#include "renderall.h"
#include "types.h"

namespace wavevis2 {

int renderall_compare_timeid(Db *db, const Dbt *a, const Dbt *b)
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


RenderAll::RenderAll(bf::path scratch) 
{

}


void RenderAll::newSample(pWaveBuffer_t pwb) {

  data_.insert(std::make_pair(pwb->time, pwb)); 
  
}

void RenderAll::renderStream(timeid_t t1, timeid_t t2, int pixels)
{
  /* Must be thread-safe!
   */
  
  
  timeid_t searchval = t1; 
  datamap_t::iterator dmi = data_.lower_bound(t1); 
  if (dmi != data_.end())
    {
      dmi--; 
    }

  for(dmi; (dmi != data_.end()) and (dmi->first < t2); ++dmi) { 
    
    timeid_t buftime = dmi->first;   
    pWaveBuffer_t pwb = dmi->second; 
    GLPointBuffer_t * pb = new GLPointBuffer_t; 
    pb->size = 0; 
    double period = 1/pwb->samprate; 
      // do the conversion
    for(int i = 0; i < pwb->data.size(); i++) { 
      pb->data[pb->size].t = period * i; 
      pb->data[pb->size].x = pwb->data[i]; 
      pb->size++; 
    }
    renderGLPointBuffer(buftime - t1, pb); 

    delete pb; 
  }

}

void RenderAll::renderGLPointBuffer(timeid_t origintime, 
				    GLPointBuffer_t * bufptr)
{
  double originf = double(origintime) / 1e9; 
  glPushMatrix();
  glTranslatef(originf, 0, 0); 
  glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), &(bufptr->data[0]) ); 
  glDrawArrays(GL_LINE_STRIP, 0, bufptr->size); 
  glPopMatrix(); 
}


void RenderAll::reset() {
  /* 
     Instead of truncating, we delete all the records; ug, slow. 
  */ 
//   std::cout << "RenderAll::reset()" << std::endl;

//   upgrade_lock_t reqlock(truncate_mutex_);
//   // get exclusive access
//   up_unique_lock_t requniqueLock(reqlock);

//   uint32_t x; 
//   db_->truncate(0, &x, 0); 

// //   timeid_t tid= 0; 
// //   Dbt key(&tid, sizeof(tid));
// //   GLPointBuffer_t pb; 
// //   Dbt data(&pb, sizeof(GLPointBuffer_t)); 
// //   Dbc * cursorp; 
// //   db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
// //   int ret; 
// //   while ((ret = cursorp->get(&key, &data, 
// // 			     DB_SET)) == 0) {
// //     cursorp->del(0);
// //   }
// //   cursorp->close(); 

//   std::cout << "RenderAll::reset() done" << std::endl;
}

RenderAll::~RenderAll()
{

  //  delete db_; 

}

  


}
