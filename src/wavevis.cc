
#include "wavevis.h"
#include <iostream>

WaveVis::WaveVis() :
  streamRenderer_(&filteredSamples_), 
  streamTrigger_(&filteredSamples_), 
  streamTriggerRenderer_(&filteredSamples_),
  yheight_(100),
  color_("red"),
  verticalScale_(1.0)
{

  // connect the signals
//   streamSource_->newDataSignal().connect(sigc::mem_fun(*this, 
// 					     &WaveVis::newData)); 
//   streamSource_->invalidateDataSignal().connect(sigc::mem_fun(*this, 
// 						    &WaveVis::invalidateData)); 
  
  

}

void WaveVis::drawMainWave(wavetime_t t1, wavetime_t t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state
  streamRenderer_.draw(t1, t2, pixels); 
  
}

WaveVis::~WaveVis()
{
  

}

void WaveVis::drawTriggerWave(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint)
{
  streamTriggerRenderer_.drawTriggers(tbefore, tafter, timepoint); 
  
}

void WaveVis::newData()
{

  
  while (not pInDataQueue_->empty())
    {
      // we're taking in WaveBuffer_t pointers

      pWaveBuffer_t pwb = pInDataQueue_->front(); 
      pInDataQueue_->pop(); 
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

void WaveVis::invalidateData()
{


}

updateTriggersSignal_t & WaveVis::updateTriggersSignal()
{
  return streamTrigger_.updateTriggersSignal(); 
}



QueueView<wavetime_t>  WaveVis::getTriggerQueueView() 
{
  return streamTrigger_.getTriggerQueueView(); 
  
}

void WaveVis::setTriggerValue(wavetime_t level)
{
  streamTrigger_.setTriggerValue(level); 
}

void WaveVis::enableTrigger(bool value)
{

  streamTrigger_.enableTrigger(value); 

}



void WaveVis::updateTriggers(bool reset)
{

  streamRenderer_.updateTriggers(reset); 
  streamTriggerRenderer_.updateTriggers(reset); 

}

void WaveVis::setTriggerSource(const QueueView<wavetime_t> & tqv)
{

  streamRenderer_.setTriggerSource(tqv);
  streamTriggerRenderer_.setTriggerSource(tqv); 

}

float WaveVis::getYOffset(){
  return yoffset_; 
}

void WaveVis::setYOffset(float x)
{
  yoffset_ = x; 
}

void WaveVis::setYHeight(float x)
{
  yheight_ = x; 
  streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
}

void WaveVis::setVerticalScale(float volts){

  verticalScale_ = volts; 
  
  streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
  verticalScaleSignal_.emit(volts); 

}

float WaveVis::getVerticalScale()
{
  return verticalScale_; 
}

void WaveVis::setColor(Gdk::Color c)
{
  
  color_ = c; 
  streamRenderer_.setColor(c); 
  colorSignal_.emit(c); 
}

Gdk::Color WaveVis::getColor()
{
  return color_; 
}

GLWavePoint_t WaveVis::filterNextPoint(GLWavePoint_t wp)
{
  
  //wp.t -= 0.1; // delay

  return wp; 



}


