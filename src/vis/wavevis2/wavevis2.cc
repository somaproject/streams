
#include "wavevis2.h"
#include <iostream>

const std::string WaveVis2::TYPENAME = "WaveVis2"; 

WaveVis2::WaveVis2(std::string name, bf::path scratchdir):
  VisBase(name), 
  pSinkPad_(createSinkPad<WaveBuffer_t>("default")),
  pixelHeight_(100), 
  //  color(Gdk::Color::parse("red"))
  scale(0.0),
  scratchdir_(scratchdir / name),
  streamRenderer_(scratchdir_)
{
  
//   pSinkPad_->newDataSignal().connect(sigc::mem_fun(*this, 
// 						   &WaveVis2::newData)); 
  
//   pSinkPad_->invalidateDataSignal().connect(sigc::mem_fun(*this, 
// 						   &WaveVis2::invalidateData)); 
  
//   color.signal().connect(sigc::mem_fun(streamRenderer_, 
// 				       &WaveVis2Renderer::setColor)); 
//   scale.signal().connect(sigc::mem_fun(*this, 
// 				       &WaveVis2::setScale)); 
  
}


WaveVis2::~WaveVis2()
{
  

}

void WaveVis2::setScale(float voltsPerPixel) {
  /* scale value is volts per pixel (?)
     
  
  */
  streamRenderer_.setScale(voltsPerPixel, pixelHeight_); 

}

void WaveVis2::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{

  streamRenderer_.renderStream(t1, t2, pixels); 

}

// TriggerRenderer interface:
void WaveVis2::renderTrigger(streamtime_t deltapre, streamtime_t deltapost, 
			    int pixels)
{


}

int WaveVis2::getPixelHeight() {

  return pixelHeight_; 

}

void WaveVis2::setPixelHeight(int x) {

  pixelHeight_ = x; 
}


void WaveVis2::newData()
{
  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
//   while (not pSinkPad_->getpQueueView()->empty())
//     {
//       // we're taking in WaveBuffer_t pointers
//       WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
//       pSinkPad_->getpQueueView()->pop(); 
//       streamRenderer_.newSample(wb); 

//     }

}

void WaveVis2::invalidateData()
{
  streamRenderer_.invalidateSamples(); 
//   while (not pSinkPad_->getpQueueView()->empty())
//     {
//       // we're taking in WaveBuffer_t pointers
//       WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
//       pSinkPad_->getpQueueView()->pop(); 
//       streamRenderer_.newSample(wb); 
//     }

}

// updateTriggersSignal_t & WaveVis2::updateTriggersSignal()
// {
//   return streamTrigger_.updateTriggersSignal(); 
// }



// QueueView<wavetime_t>  WaveVis2::getTriggerQueueView() 
// {
//   return streamTrigger_.getTriggerQueueView(); 
  
// }

// void WaveVis2::setTriggerValue(wavetime_t level)
// {
//   streamTrigger_.setTriggerValue(level); 
// }

// void WaveVis2::enableTrigger(bool value)
// {

//   streamTrigger_.enableTrigger(value); 

// }



// void WaveVis2::updateTriggers(bool reset)
// {

//   streamRenderer_.updateTriggers(reset); 
//   streamTriggerRenderer_.updateTriggers(reset); 

// }

// void WaveVis2::setTriggerSource(const QueueView<wavetime_t> & tqv)
// {

//   streamRenderer_.setTriggerSource(tqv);
//   streamTriggerRenderer_.setTriggerSource(tqv); 

// }

// float WaveVis2::getYOffset(){
//   return yoffset_; 
// }

// void WaveVis2::setYOffset(float x)
// {
//   yoffset_ = x; 
// }

// void WaveVis2::setYHeight(float x)
// {
//   yheight_ = x; 
//   streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
// }

// void WaveVis2::setVerticalScale(float volts){

//   verticalScale_ = volts; 
  
//   streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
//   verticalScaleSignal_.emit(volts); 

// }

// float WaveVis2::getVerticalScale()
// {
//   return verticalScale_; 
// }

// void WaveVis2::setColor(Gdk::Color c)
// {
  
//   color_ = c; 
//   streamRenderer_.setColor(c); 
//   colorSignal_.emit(c); 
// }

// Gdk::Color WaveVis2::getColor()
// {
//   return color_; 
// }

// GLWavePoint_t WaveVis2::filterNextPoint(GLWavePoint_t wp)
// {
  
//   wp.t -= 0.1; // delay

//   return wp; 



// }


void WaveVis2::process(elements::timeid_t t)
{


}
