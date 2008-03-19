#include "fakewave.h"
#include <iostream>
#include <gtkmm.h>


FakeWave::FakeWave(pTimer_t ptimer) :
  pTimer_(ptimer), 
  fs_(1.0), 
  variance_(1.0)
{
  pTimer_->timeSignal().connect(sigc::mem_fun(*this, 
					      &FakeWave::timeUpdate)); 
  
  lastTime_ = pTimer_->getStreamTime(); 
}

FakeWave::~FakeWave()
{
  

}

void FakeWave::nextData()
{
  
  pBuffer_t pwb(new Buffer_t); 
  pwb->time = lastTime_; 
  pwb->samprate = 1000.0; 
  pwb->data.reserve(100); 
  for (int i = 0; i < 100; i++) {
    if (i % 2 == 0) { 
      pwb->data.push_back(float(i)/-1.0); 
    } else {
      pwb->data.push_back(float(i)/1.0); 
    }
      
  }
  data_.push_back(pwb); 
  newDataSignal_.emit(); 
  
}

void FakeWave::setFs(double fs) {
  fs_ = fs; 
}

void FakeWave::setVariance(double var) {
  variance_ = var; 
}

void FakeWave::timeUpdate(streamtime_t time) {
  lastTime_ = pTimer_->getStreamTime(); 
  nextData(); 

}
