#ifndef TIMER_H
#define TIMER_H

#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>


typedef float streamtime_t; 

class Timer
{
public:
  Timer(); 
  ~Timer(); 
  sigc::signal<void, streamtime_t>  & timeSignal() { return timeSignal_;} 
  streamtime_t getStreamTime(); 
  bool timeout(); 
  streamtime_t time_; 
private:
  sigc::signal<void, streamtime_t> timeSignal_; 
  
}; 

typedef boost::shared_ptr<Timer> pTimer_t; 

#endif // TIMER
