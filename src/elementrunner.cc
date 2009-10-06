#include "elementrunner.h"


StaticElementRunner::StaticElementRunner(std::list<elements::pIElement_t> elements, 
					 pTimer_t timer) :
  elements_(elements),
  pthread_(NULL),
  timer_(timer),
  running_(false)
{



}


void StaticElementRunner::run()
{
  running_ = true; 
  pthread_ = new boost::thread(&StaticElementRunner::worker, 
			       this); 
  
}

void StaticElementRunner::worker()
{
  while(running_) {
    BOOST_FOREACH(elements::pIElement_t e, elements_) {
      //e->process(timer_->getTimeID());       
    }
  }
  

}

void StaticElementRunner::stop()
{
  running_ = false; 
  
  if(pthread_) {
    pthread_->join(); 
  }

}
