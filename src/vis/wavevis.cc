
#include "wavevis.h"
#include <iostream>

const std::string WaveVis::TYPENAME = "WaveVis"; 

WaveVis::WaveVis(std::string name):
  VisBase(name), 
  pSinkPad_(createSinkPad<WaveBuffer_t>( "default")),
  streamRenderer_(), 
  pixelHeight_(100), 
  scale(0.0)
{
  
  pSinkPad_->newDataSignal().connect(sigc::mem_fun(*this, 
						   &WaveVis::newData)); 
  
  color.signal().connect(sigc::mem_fun(streamRenderer_, 
				       &WaveVisRenderer::setColor)); 
  scale.signal().connect(sigc::mem_fun(*this, 
				       &WaveVis::setScale)); 
  
}


WaveVis::~WaveVis()
{
  

}

void WaveVis::setScale(float voltsPerPixel) {
  /* scale value is volts per pixel (?)
     
  
  */
  streamRenderer_.setScale(voltsPerPixel, pixelHeight_); 

}

void WaveVis::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{

  streamRenderer_.renderStream(t1, t2, pixels); 

}

// TriggerRenderer interface:
void WaveVis::renderTrigger(streamtime_t deltapre, streamtime_t deltapost, 
			    int pixels)
{


}

int WaveVis::getPixelHeight() {

  return pixelHeight_; 

}

void WaveVis::setPixelHeight(int x) {

  pixelHeight_ = x; 
}


void WaveVis::newData()
{
  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
  while (not pSinkPad_->getpQueueView()->empty())
    {
      // we're taking in WaveBuffer_t pointers
      WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
      pSinkPad_->getpQueueView()->pop(); 
      streamRenderer_.newSample(wb); 

    }

}

void WaveVis::invalidateData()
{


}

// updateTriggersSignal_t & WaveVis::updateTriggersSignal()
// {
//   return streamTrigger_.updateTriggersSignal(); 
// }



// QueueView<wavetime_t>  WaveVis::getTriggerQueueView() 
// {
//   return streamTrigger_.getTriggerQueueView(); 
  
// }

// void WaveVis::setTriggerValue(wavetime_t level)
// {
//   streamTrigger_.setTriggerValue(level); 
// }

// void WaveVis::enableTrigger(bool value)
// {

//   streamTrigger_.enableTrigger(value); 

// }



// void WaveVis::updateTriggers(bool reset)
// {

//   streamRenderer_.updateTriggers(reset); 
//   streamTriggerRenderer_.updateTriggers(reset); 

// }

// void WaveVis::setTriggerSource(const QueueView<wavetime_t> & tqv)
// {

//   streamRenderer_.setTriggerSource(tqv);
//   streamTriggerRenderer_.setTriggerSource(tqv); 

// }

// float WaveVis::getYOffset(){
//   return yoffset_; 
// }

// void WaveVis::setYOffset(float x)
// {
//   yoffset_ = x; 
// }

// void WaveVis::setYHeight(float x)
// {
//   yheight_ = x; 
//   streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
// }

// void WaveVis::setVerticalScale(float volts){

//   verticalScale_ = volts; 
  
//   streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
//   verticalScaleSignal_.emit(volts); 

// }

// float WaveVis::getVerticalScale()
// {
//   return verticalScale_; 
// }

// void WaveVis::setColor(Gdk::Color c)
// {
  
//   color_ = c; 
//   streamRenderer_.setColor(c); 
//   colorSignal_.emit(c); 
// }

// Gdk::Color WaveVis::getColor()
// {
//   return color_; 
// }

// GLWavePoint_t WaveVis::filterNextPoint(GLWavePoint_t wp)
// {
  
//   wp.t -= 0.1; // delay

//   return wp; 



// }


