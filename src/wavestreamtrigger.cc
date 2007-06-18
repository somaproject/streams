#include "wavestreamtrigger.h"

WaveStreamTrigger::WaveStreamTrigger(std::vector<GLWavePoint_t> * pSamples) :
  pSamples_(pSamples), 
  triggerLevel_(0.8), 
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

void WaveStreamTrigger::newSample()
{
  
  GLWavePoint_t p = pSamples_->back(); 

  if (triggerEnabled_ and ! pSamples_->empty()) {
    // try and extract out the relevant iterators
    std::vector<GLWavePoint_t>::iterator pold, pnew; 
    pnew = --(pSamples_->end()); 
    
    pold = pnew; 
    pold--; 
    if (pold != pnew and pnew != pSamples_->begin()){
      if (triggerFunc(*pold, *pnew) )
	
	{
	  assert (pSamples_->size() > 1); 

	  float xdelta = triggerLevel_ - pold->x ; 
	  float tdelta = xdelta * (pnew->t -  pold->t) /(pnew->x -  pold->x); 
	  
	  triggers_.push_back(pold->t +tdelta); 
	  updateTriggersSignal_.emit(false); 
	}
    }
    
  }
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
    for (int i = 1; i < pSamples_->size(); i++)
      {
	if ( triggerFunc((*pSamples_)[i-1], (*pSamples_)[i]) )
	  {

	    // compute the time and val:
	    float xdelta = triggerLevel_ - (*pSamples_)[i-1].x ; 
	    float tdelta = xdelta * ((*pSamples_)[i].t -  (*pSamples_)[i].t) /((*pSamples_)[i].x -  (*pSamples_)[i].x); 
	    
	    triggers_.push_back((*pSamples_)[i].t) ; 
	  }


      }
    updateTriggersSignal_.emit(false); 
  } else {
    // disable the trigger

    triggerEnabled_=false;     
    
    triggers_.clear(); 
    updateTriggersSignal_.emit(true); 

    
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

updateTriggersSignal_t & WaveStreamTrigger::updateTriggersSignal()
{
  return updateTriggersSignal_; 

}
