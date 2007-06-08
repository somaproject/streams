
#include "wavestreamvis.h"
#include <iostream>

WaveStreamVis::WaveStreamVis(WaveStreamSource * wss) :
  streamSource_ (wss), 
  inDataQueue_(streamSource_->getQueueView())
{

  // connect the signals
  streamSource_->newDataSignal().connect(sigc::mem_fun(*this, 
					     &WaveStreamVis::newData)); 
  streamSource_->invalidateDataSignal().connect(sigc::mem_fun(*this, 
						    &WaveStreamVis::invalidateData)); 
  
  

}

void WaveStreamVis::drawMainWave(float t1, float t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state

  streamRenderer_.draw(t1, t2, pixels); 
  
}

WaveStreamVis::~WaveStreamVis()
{
  

}

void WaveStreamVis::drawTriggerWave(float tbefore, float tafter, float timepoint)
{
  streamTriggerRenderer_.drawTriggers(tbefore, tafter, timepoint); 
  
}

void WaveStreamVis::newData()
{

  while (not inDataQueue_.empty())
    {
      // we're taking in WaveBuffer_t pointers

      WaveBuffer_t * pwb = inDataQueue_.front(); 
      inDataQueue_.pop(); 
      for (int i = 0; i < pwb->data.size(); i++)
	{
	  GLWavePoint_t wp; 
	  wp.t = pwb->time + (i / pwb->samprate);
	  wp.x = pwb->data[i]; 
	  
	  streamRenderer_.append(wp); 
	  streamTrigger_.append(wp); 
	  streamTriggerRenderer_.append(wp); 
	}
      
    }


}

void WaveStreamVis::invalidateData()
{


}

newTriggersSignal_t & WaveStreamVis::newTriggersSignal()
{
  return streamTrigger_.newTriggersSignal(); 
}

invalidateTriggersSignal_t & WaveStreamVis::invalidateTriggersSignal() 
{
  return streamTrigger_.invalidateTriggersSignal(); 
}

QueueView<float>  WaveStreamVis::getTriggerQueueView() 
{
  return streamTrigger_.getTriggerQueueView(); 
  
}

void WaveStreamVis::setTriggerValue(float level)
{
  streamTrigger_.setTriggerValue(level); 
}

void WaveStreamVis::enableTrigger(bool value)
{

  streamTrigger_.enableTrigger(value); 

}


void WaveStreamVis::resetTriggers()
{
  streamRenderer_.resetTriggers(); 
}

void WaveStreamVis::newTriggers()
{

  streamRenderer_.newTriggers(); 
  streamTriggerRenderer_.newTriggers(); 

}

void WaveStreamVis::setTriggerSource(const QueueView<float> & tqv)
{

  streamRenderer_.setTriggerSource(tqv);
  streamTriggerRenderer_.setTriggerSource(tqv); 

}
