#include "wavestreamtrigger.h"

WaveStreamTrigger::WaveStreamTrigger() :
  triggerLevel_(0.0), 
  triggerEnabled_(false) 
{


}

WaveStreamTrigger::~WaveStreamTrigger()
{
  
  
  
}

bool WaveStreamTrigger::triggerFunc(GLWavePoint_t p1,  GLWavePoint_t p2)
{
  // p1 comes before p2:
  assert (p1.t < p2.t); 
  
  if (p1.x < triggerLevel_ and p2.x >= triggerLevel_) {
    assert(p1.x < p2.x) ; 
    return true; 
  }
   

  return false; 

}

void WaveStreamTrigger::append(GLWavePoint_t p)
{

  if (triggerEnabled_ and ! samples_.empty()) {
    if (triggerFunc(samples_.back(), p) )
      
      {
	float xdelta = triggerLevel_ - samples_.back().x ; 
	float tdelta = xdelta * (p.t -  samples_.back().t) /(p.x -  samples_.back().x); 
	
	triggers_.push_back(samples_.back().t +tdelta); 
	newTriggersSignal_.emit(); 
      }
  }

  // add the sample to the buffer
  samples_.push_back(p);
}

void WaveStreamTrigger::enableTrigger(bool state)
{
  if (state == triggerEnabled_) {
    return;
  }

  if (state == true) {
    assert (triggers_.empty()); 
    triggerEnabled_ = true; 

    // enable the trigger
    for (int i = 1; i < samples_.size(); i++)
      {
	if ( triggerFunc(samples_[i-1], samples_[i]) )
	  {

	    // compute the time and val:
	    float xdelta = triggerLevel_ - samples_[i-1].x ; 
	    float tdelta = xdelta * (samples_[i].t -  samples_[i].t) /(samples_[i].x -  samples_[i].x); 
	    
	    triggers_.push_back(samples_[i].t) ; 
	  }


      }
    newTriggersSignal_.emit(); 
  } else {
    // disable the trigger

    triggerEnabled_=false;     
    
    triggers_.clear(); 
    invalidateTriggersSignal_.emit(); 

    
  }
  


}

void WaveStreamTrigger::setTriggerValue(float val)
{
  triggerLevel_ = val;
}

QueueView<float> WaveStreamTrigger::getTriggerQueueView()
{
  return QueueView<float>(triggers_); 
}

newTriggersSignal_t & WaveStreamTrigger::newTriggersSignal()
{
  return newTriggersSignal_; 

}

invalidateTriggersSignal_t & WaveStreamTrigger::invalidateTriggersSignal()
{
  return invalidateTriggersSignal_; 

}

