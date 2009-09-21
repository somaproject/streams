
#include "wavestreamvis.h"
#include <iostream>

WaveStreamVis::WaveStreamVis(WaveStreamSource * wss) :
  streamSource_ (wss), 
  inDataQueue_(streamSource_->getQueueView()),
  streamRenderer_(&filteredSamples_), 
  streamTrigger_(&filteredSamples_), 
  streamTriggerRenderer_(&filteredSamples_),
  yheight_(100),
  color_("red"),
  verticalScale_(1.0)
{

  // connect the signals
  streamSource_->newDataSignal().connect(sigc::mem_fun(*this, 
					     &WaveStreamVis::newData)); 
//   streamSource_->invalidateDataSignal().connect(sigc::mem_fun(*this, 
// 						    &WaveStreamVis::invalidateData)); 
  
  

}

void WaveStreamVis::drawMainWave(wavetime_t t1, wavetime_t t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state

  streamRenderer_.draw(t1, t2, pixels); 
  
}

WaveStreamVis::~WaveStreamVis()
{
  

}

void WaveStreamVis::drawTriggerWave(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint)
{
  streamTriggerRenderer_.drawTriggers(tbefore, tafter, timepoint); 
  
}

void WaveStreamVis::newData()
{

  while (not inDataQueue_.empty())
    {
      // we're taking in WaveBuffer_t pointers

      pWaveBuffer_t pwb = inDataQueue_.front(); 
      inDataQueue_.pop(); 
      for (int i = 0; i < pwb->data.size(); i++)
	{
	  GLWavePoint_t wp; 
	  wp.t = pwb->time + (i / pwb->samprate);
	  wp.x = pwb->data[i]; 
	  
	  GLWavePoint_t fwp = filterNextPoint(wp); 
	  filteredSamples_.push_back(fwp); 
	  streamRenderer_.newSample(); 
	  streamTrigger_.newSample(); 
	  streamTriggerRenderer_.newSample(); 
	}
      
    }


}

void WaveStreamVis::invalidateData()
{


}

updateTriggersSignal_t & WaveStreamVis::updateTriggersSignal()
{
  return streamTrigger_.updateTriggersSignal(); 
}



QueueView<wavetime_t>  WaveStreamVis::getTriggerQueueView() 
{
  return streamTrigger_.getTriggerQueueView(); 
  
}

void WaveStreamVis::setTriggerValue(wavetime_t level)
{
  streamTrigger_.setTriggerValue(level); 
}

void WaveStreamVis::enableTrigger(bool value)
{

  streamTrigger_.enableTrigger(value); 

}



void WaveStreamVis::updateTriggers(bool reset)
{

  streamRenderer_.updateTriggers(reset); 
  streamTriggerRenderer_.updateTriggers(reset); 

}

void WaveStreamVis::setTriggerSource(const QueueView<wavetime_t> & tqv)
{

  streamRenderer_.setTriggerSource(tqv);
  streamTriggerRenderer_.setTriggerSource(tqv); 

}

float WaveStreamVis::getYOffset(){
  return yoffset_; 
}

void WaveStreamVis::setYOffset(float x)
{
  yoffset_ = x; 
}

void WaveStreamVis::setYHeight(float x)
{
  yheight_ = x; 
  streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
}

void WaveStreamVis::setVerticalScale(float volts){

  verticalScale_ = volts; 
  
  streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
  verticalScaleSignal_.emit(volts); 

}

float WaveStreamVis::getVerticalScale()
{
  return verticalScale_; 
}

void WaveStreamVis::setColor(Gdk::Color c)
{
  
  color_ = c; 
  streamRenderer_.setColor(c); 
  colorSignal_.emit(c); 
}

Gdk::Color WaveStreamVis::getColor()
{
  return color_; 
}

GLWavePoint_t WaveStreamVis::filterNextPoint(GLWavePoint_t wp)
{
  
  //wp.t -= 0.1; // delay

  return wp; 



}

