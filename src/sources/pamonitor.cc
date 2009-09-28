#include "pamonitor.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o

using namespace soma::streams; 

const std::string PulseAudioMonitorWave::TYPENAME = "PulseAudioMonitorWave"; 

PulseAudioMonitorWave::PulseAudioMonitorWave(std::string name, pTimer_t ptimer, 
					     bf::path scratch) :
  SourceBase(name), 
  pTimer_(ptimer), 
  pSourcePad_(createSourcePad<WaveBuffer_t>(dataList_, "default")),
  start_time(boost::posix_time::microsec_clock::local_time()),
  samppos_(0)
{
//   lastTime_ = pTimer_->getStreamTime(); 
//   pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
// 					      &PulseAudioMonitorWave::timeUpdate)); 
  
//   for (double t = -preload; t < 0; t += 0.1) { 
//     lastTime_ = t; 
//     streamtime_t delta = 0.1; 
//     nextData(delta); 
//   }
  pasource_.datasig.connect(sigc::mem_fun(*this, &PulseAudioMonitorWave::nextData)); 

}

PulseAudioMonitorWave::~PulseAudioMonitorWave()
{
  

}

void PulseAudioMonitorWave::nextData(const float * d, unsigned l)
{
  /* Buffer start time here is tricky */ 
  
  
//   boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time(); 
//   boost::posix_time::time_duration td = now - start_time; 
  
//   std::cout <<  pTimer_->getStreamTime() << " " << l << std::endl; 
  int FS = 44100; 

  dataList_.push_back(new WaveBuffer_t()); 
  dataList_.back().time = double(samppos_)/FS; 
  dataList_.back().samprate = FS; 
  dataList_.back().data.reserve(l/2); 
  for(int i = 0; i < l/2; i++) {
    dataList_.back().data.push_back(d[i*2]); 
  }
  pSourcePad_->newData(); 
  samppos_ += l/2;   
}
