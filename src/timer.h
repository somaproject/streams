#ifndef TIMER_H
#define TIMER_H

#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>
#include <gtkmm.h>

#include "streamtime.h"

#include "somanetcodec.h"

class Timer; 

typedef boost::shared_ptr<Timer> pTimer_t; 

class Timer
{
public:

  static pTimer_t createDummyTimer() {
    
    // get fake soma time -- noop
    pTimer_t pt(new Timer(0)); 
    Glib::signal_timeout().connect(sigc::mem_fun(pt.get(), 
						 &Timer::dummytimeout), 
				   100); 
    
    return pt; 
    
  }

  static pTimer_t createNetworkTimer(pSomaNetCodec_t snc)
  {
    // get current network time from the codec
    
    // set the times
    
    // 
    pTimer_t pt(new Timer(0)); 
    snc->signalTimeUpdate().connect(sigc::mem_fun((*pt), 
						  &Timer::updateSomaTime)); 
    
    
    return pt; 
    
  }
  
  
  ~Timer(); 
  sigc::signal<void, streamtime_t>  & streamTimeSignal() { return streamTimeSignal_;} 
  streamtime_t getStreamTime(); 
  walltime_t getWallTime(); // thin wrapper around gettimeofday-like functionality
  streamtime_t getExperimentTime(); 
  
  
  streamtime_t time_; 
  
  static const uint32_t SOMATIME_PER_SEC = 50000; 

  // update and manipulation functions
  walltime_t somaTimeToWallTime(somatime_t); 
  streamtime_t somaTimeToStreamTime(somatime_t); 
  streamtime_t somaTimeToExpTime(somatime_t); 
  

private:
  Timer(somatime_t);  // must create via methods
  
  somatime_t startSomaTime_; // what was the soma time when the program started
  walltime_t startWallTime_; // what was the wall time when the program started
  streamtime_t expTimeOffset_; // how far is experiment time ahead/behind wall time
  
  somatime_t currentSomaTime_; 
  walltime_t currentWallTime_; 
  streamtime_t currentStreamTime_; 
  streamtime_t currentExpTime_; 
  
  sigc::signal<void, streamtime_t> streamTimeSignal_; 
  pSomaNetCodec_t pSomaNetCodec_; 
  
  
  void updateSomaTime(somatime_t) ; 
  
  bool dummytimeout(); 
  
}; 


#endif // TIMER
