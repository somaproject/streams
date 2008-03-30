#include "fakewave.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>


FakeWave::FakeWave(pTimer_t ptimer) :
  pTimer_(ptimer), 
  fs_(1.0), 
  variance_(1.0)
{
  pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
					      &FakeWave::timeUpdate)); 
  
  lastTime_ = pTimer_->getStreamTime(); 
}

FakeWave::~FakeWave()
{
  

}

void FakeWave::nextData(streamtime_t delta)
{
  
  int FS = 1000; 
  pBuffer_t pwb(new Buffer_t); 
  pwb->time = lastTime_; 
  pwb->samprate = FS; 

  int N = delta * FS; 
  pwb->data.reserve(N); 
  static double freq = 1.0; 
  freq += 10; 

  for (int i = 0; i < N; i++) {
    float y = double(random()) / RAND_MAX; 
    float x = 1 * sin (float(i) / FS * 3.14152*2 * freq) + y;

    pwb->data.push_back(x); 
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
  streamtime_t oldtime = lastTime_; 
  lastTime_ = pTimer_->getStreamTime(); 
  streamtime_t delta = lastTime_ - oldtime; 
  nextData(delta); 
  
}
