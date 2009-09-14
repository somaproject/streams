#include "renderdownsample.h"
#include <boost/foreach.hpp>

RenderDownSample::RenderDownSample(float detail) :
  detail_(detail), 
  NEWBUFTIME_(100*detail), 
  currentTBiterator_(tbm_.end())
{
  


}

RenderDownSample::~RenderDownSample()
{


}


void RenderDownSample::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  timeBufferMap_t::iterator lb= tbm_.lower_bound(t1); 
  if (lb !=tbm_.begin()) 
    lb--; 

  while(lb->first < t2 and lb != tbm_.end()) { 
    glPushMatrix();
    glTranslatef(lb->first - t1, 0, 0); 

    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
 		    &((*(lb->second))[0])); 
    glDrawArrays(GL_LINE_STRIP, 0, lb->second->size()); 
    glPopMatrix(); 
    lb++; 
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
  
  if (currentTBiterator_ == tbm_.end() or 
//       (!currentTBiterator_->second->empty() and 
//        ((streamtime - currentTBiterator_->second->back().t ) > NEWBUFTIME_)) or
      currentTBiterator_->second->size() == BUFFERN) {
    // create a new buffer
    tbm_[streamtime] = new GLPointBuffer_t; 
    currentTBiterator_ = tbm_.find(streamtime); 
    currentTBiterator_->second->reserve(BUFFERN); 
    //    std::
  }
  
  if (currentTBiterator_->second->empty()) {
    // first data point
    GLWavePoint_t gp; 
    // 
    gp.t = streamtime - currentTBiterator_->first; 
    gp.x = data; 
    currentTBiterator_->second->push_back(gp); 
  } else { 
    double delta = (streamtime - currentTBiterator_->first) - currentTBiterator_->second->back().t; 
    if (delta > detail_) { 
      GLWavePoint_t gp; 
      // 
      gp.t = streamtime - currentTBiterator_->first; 
      gp.x = data; 
      currentTBiterator_->second->push_back(gp); 
    
    }
    
  }
}

