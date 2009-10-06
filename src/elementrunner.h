#ifndef __ELEMENT_RUNNER_H__
#define __ELEMENT_RUNNER_H__

#include <elements/ielement.h>
#include "timer.h"

class StaticElementRunner
{
  /*
    Run (process) a fixed list of elements
  */
public: 
  StaticElementRunner(std::list<elements::pIElement_t> elements, 
		      pTimer_t timer); 

  void run(); 
  void stop(); 
  

private:
  std::list<elements::pIElement_t> elements_; 
  
  void worker(); 

  boost::thread * pthread_; 
  pTimer_t timer_; 
  bool running_; 

};


#endif
