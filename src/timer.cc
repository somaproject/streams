#include <gtkmm.h>
#include <iostream>
#include "timer.h"

Timer::Timer() :
  time_(0.0)
{
  Glib::signal_timeout().connect(sigc::mem_fun(*this, 
					       &Timer::timeout), 
				 100); 
  

}

bool Timer::timeout() 
{
  time_ += 0.10; 
  timeSignal_.emit(time_); 
  std::cout << "Timeout" << std::endl; 
}


streamtime_t Timer::getStreamTime()
{


}

Timer::~Timer()
{


}
