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
  pSourcePad_(createSourcePad<pWaveBuffer_t>("default", 
					     boost::bind(&PulseAudioMonitorWave::get_src_data, this, _1), 
					     boost::bind(&PulseAudioMonitorWave::get_sequence, this))), 
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
  if (current_pwb_ == 0) {
    current_pwb_ = pWaveBuffer_t(new WaveBuffer_t); 

    double time = double(samppos_)/FS; 
    current_pwb_->time = timeid_t(time * elements::TIMEID_PER_SEC); 
    current_pwb_->samprate = FS; 

    current_pwb_->data.reserve(1024);     
  }
  
  int N = l/2; 

  for(int i = 0; i < N; i++) {
    current_pwb_->data.push_back(d[i*2]); 
  }
  
  if (current_pwb_->data.size() > 512) { 
  
    boost::upgrade_lock<boost::shared_mutex> lock(data_mutex_);
    
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
    data_.insert(std::make_pair(current_pwb_->time, current_pwb_)); 

    current_pwb_.reset(); 

  }
  
  samppos_ += N;   
}

elements::datawindow_t<pWaveBuffer_t> PulseAudioMonitorWave::get_src_data(const elements::timewindow_t & tw)
{
  /* 
     We know that, by construction, we never have data <= 0, so
     the right answer here is to ... FIXME
   */ 
  

  elements::datawindow_t<pWaveBuffer_t> wb; 
  wb.sequenceid = 0; 
  

  
  timeid_t start = tw.start; 
  timeid_t end = tw.end; 

  if (data_.empty()) {
    return wb; 
  }



  boost::shared_lock<boost::shared_mutex> datareadlock(data_mutex_); 

  datamap_t::iterator i = data_.lower_bound(start); 
    
  if (i != data_.begin()) {
    i--; 
  }
  
  for (i; ((i != data_.end() ) and (i->first <= end)); ++i) {
    wb.data.push_back(i->second); 
  }

  
  if (!wb.data.empty()) {
    timeid_t starttime = std::min(wb.data.front()->time, start); 
    wb.interval = elements::timeinterval_t(starttime, 
					   wb.data.back()->time); 
  }
//   std::cout << "requested " << start << " " << end << " and are retruning" 
// 	    << wb.interval.as_string() << " of size " << wb.data.size() << std::endl; 
  return wb; 
}


