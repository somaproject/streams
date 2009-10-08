#include "pamonitor.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o

using namespace soma::streams; 

const std::string PulseAudioMonitorWave::TYPENAME = "PulseAudioMonitorWave"; 

PulseAudioMonitorWave::PulseAudioMonitorWave(std::string name, 
					     bf::path scratch) :
  SourceBase(name), 
  pSourcePad_(createSourcePad<WaveBuffer_t>("default")),
  start_time(boost::posix_time::microsec_clock::local_time()),
  samppos_(0)
{
//   lastTime_ = pTimer_->getStreamTime(); 
//   pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
// 					      &PulseAudioMonitorWave::timeUpdate)); 
  
  // note that this is going to be called out of process! 
  pasource_.datasig.connect(sigc::mem_fun(*this, &PulseAudioMonitorWave::nextData)); 

}

void PulseAudioMonitorWave::process(elements::timeid_t t)
{
  pasource_.process(50); // assuming it's in ms

}

PulseAudioMonitorWave::~PulseAudioMonitorWave()
{
  

}

void PulseAudioMonitorWave::nextData(const float * d, unsigned l)
{
  /* Buffer start time here is tricky */ 
  


//   boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time(); 
//   boost::posix_time::time_duration td = now - start_time; 
  int FS = 44100; 

  WaveBuffer_t wb; 
  double time = double(samppos_)/FS; 
  wb.time = timeid_t(time * elements::TIMEID_PER_SEC); 
  wb.samprate = FS; 

  int N = l/2; 
  wb.data.reserve(N); 
  for(int i = 0; i < N; i++) {
    wb.data.push_back(d[i*2]); 
  }
  
  pSourcePad_->newData(wb.time, 
		       wb.time + (wb.samprate * wb.data.size()) *
		       elements::TIMEID_PER_SEC, 
		       wb);
//   std::cout << "PulseAudioMonitorWave::nextData N=" << N
//  	    << " time = " << time << " wb.time= " << wb.time << std::endl; 
  samppos_ += N;   
}
