#include <boost/foreach.hpp>
#include <limits.h>
#include "fixeddownsample.h"

namespace wavevis2 { 

FixedDownSample::FixedDownSample(timeid_t binsize) :
  BINSIZE_(binsize), 
  BUFFERDUR_(BINSIZE_*BUFFERLEN)  
{
  
  

}

FixedDownSample::~FixedDownSample()
{


}

void FixedDownSample::reset()
{
  data_.clear(); 
  seentimes_.clear(); 

}

void FixedDownSample::renderStream(timeid_t t1, timeid_t t2, int pixels)
{
  /* 
     
     
   */ 
  
  datamap_t::iterator i; 
  i = data_.lower_bound(t1); 

  if (i != data_.begin()) {
    i--; 
  }
  
  for(i; i != data_.end(); i++) { 
    if (i->first > t2) {
      break; 
    }
    glPushMatrix(); 
    glTranslatef(double(i->first - t1)/1e9, 0, 0); 

    pFixedDownSampleBuffer_t pbuf = i->second; 
    
    for(int pos = 0; pos < BUFFERLEN; pos++) { 
      if(pbuf->present[pos]) { 
	float bin_start_time = pbuf->times[pos]; 
	float bin_end_time= bin_start_time + double(BINSIZE_)/1e9; 
	float max = pbuf->maxs[pos]; 
	float min = pbuf->mins[pos]; 


	glBegin(GL_QUADS); 
	glVertex2f(bin_start_time, min); 
	glVertex2f(bin_start_time, max); 
	glVertex2f(bin_end_time, max); 
	glVertex2f(bin_end_time, min); 
	glEnd(); 

      }
    }
    glPopMatrix(); 
  }

}

void FixedDownSample::newDataWindow(const elements::datawindow_t<pWaveBuffer_t> & datawindow ) 
{
  
  BOOST_FOREACH(pWaveBuffer_t wb, datawindow.data) {
    newSample(wb); 
  }

}


void FixedDownSample::newSample(pWaveBuffer_t wb)
{
  // for each data point in the buffer:
  //    bid = buffer for this data point
  //    ti  = time index for this buffer point
  elements::timeid_t t = wb->time; 
  if(seentimes_.find(t) == seentimes_.end()) { 
    // make sure we haven't seen
    // this buffer

    size_t N = wb->data.size(); 

    for(int i  = 0; i < N;  ++i) { 
      elements::timeid_t buftime = i * (1.0/wb->samprate) * 
	elements::TIMEID_PER_SECF; 
      elements::timeid_t samplet = t + buftime; 

    std::pair<bufferid_t, unsigned int> pos = 
      getBinAndPos(samplet, BINSIZE_, BUFFERLEN); 
    
    bufferid_t bid = pos.first; 
    elements::timeid_t buffer_pos = pos.second; 
    
    datamap_t::iterator di = data_.find(bid); 
    pFixedDownSampleBuffer_t dsb; 

    if (di == data_.end()) { 
      
      pFixedDownSampleBuffer_t dsbnew(new FixedDownSampleBuffer); 
      dsbnew->mins.resize(BUFFERLEN); 
      dsbnew->maxs.resize(BUFFERLEN); 
      dsbnew->times.resize(BUFFERLEN); 
      dsbnew->present.resize(BUFFERLEN); 
      
      data_.insert(std::make_pair(bid, dsbnew)); 
      dsb = dsbnew; 

      // set the times 
      for(int j = 0; j < BUFFERLEN; j++) { 
	dsbnew->mins[j] = std::numeric_limits<float>::max(); 
	dsbnew->maxs[j] = -std::numeric_limits<float>::max(); 
	dsbnew->times[j] = double(j * BINSIZE_)/1e9; 	
      }

    } else { 
      dsb = di->second; 
    }

    float sampleval = wb->data[i]; 
    
    // now we know the bin, etc. 
    dsb->present[buffer_pos] = true; 

    if(dsb->mins[buffer_pos] > sampleval) {
      dsb->mins[buffer_pos] = sampleval; 
    } 
    
    if(dsb->maxs[buffer_pos] < sampleval) {
      dsb->maxs[buffer_pos] = sampleval; 
    } 
    
    }
    seentimes_.insert(t); 
  } 
}


pFixedDownSampleBuffer_t FixedDownSample::createNewBuffer(bufferid_t bucket)
{
  pFixedDownSampleBuffer_t pbuf(new FixedDownSampleBuffer); 

  pbuf->starttime = bucket * BUFFERDUR_; 
  pbuf->endtime = (bucket+1)*BUFFERDUR_ - 1; 
  pbuf->mins.resize(BUFFERLEN); 
  pbuf->maxs.resize(BUFFERLEN); 
  pbuf->times.resize(BUFFERLEN); 
  pbuf->present.resize(BUFFERLEN); 

}

bufferid_t FixedDownSample::computeBin(timeid_t time)
{
  bufferid_t bid = 0; 
  //times
  
  return bid; 
}


std::pair<bufferid_t, unsigned int> 
FixedDownSample::getBinAndPos(timeid_t time, timeid_t binsize, 
			      timeid_t bufferlen) 
{
  /* 
     returns the bufferid of the time and the
     buffer pos of the time

     binsize is the size of each bin in ns
     bufferlen is the number of bins

  */ 
  timeid_t buffer_duration = binsize * bufferlen; 
  
  double floorval = floor(double(time) / double(buffer_duration)); 
  bufferid_t id = bufferid_t(floorval); 

    
  // std::cout << "The time = " << time << " id = " << id 
  // 	    << " floor=" << floorval << std::endl; 
  std::pair<bufferid_t, unsigned int> result; 
  result.first = id; 
  
  timeid_t r = time % buffer_duration; 
  if (time < 0) { 
    r += buffer_duration;     
  }

  // std::cout << "r =" << r << std::endl;

  double pos = floor(double(r) / double(binsize)); 
  
  result.second = bufferid_t(pos); 
  
  return result; 
}

}
