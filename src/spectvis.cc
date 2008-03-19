
#include "spectvis.h"
#include <iostream>
#include <vsip/initfin.hpp>
#include <vsip/support.hpp>
#include <vsip/signal.hpp>
#include <vsip/math.hpp>
#include <vsip/core/profile.hpp>


SpectVis::SpectVis() :
  streamRenderer_(&spectBlocks_), 
  streamTrigger_(&spectBlocks_), 
  streamTriggerRenderer_(&spectBlocks_), 
  yheight_(100),
  color_("red"),
  verticalScale_(1.0)
{
  

}

void SpectVis::drawMainWave(wavetime_t t1, wavetime_t t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state
  streamRenderer_.draw(t1, t2, pixels); 
  
}

SpectVis::~SpectVis()
{
  

}

void SpectVis::drawTriggerWave(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint)
{
  streamTriggerRenderer_.drawTriggers(tbefore, tafter, timepoint); 
  
}

void SpectVis::newData()
{

  using namespace vsip;
  using namespace vsip_csl;
  using namespace vsip::impl::profile;


  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
  while (not pInDataQueue_->empty())
    {
      // we're taking in WaveBuffer_t pointers

      pWaveBuffer_t pwb = pInDataQueue_->front(); 
      pInDataQueue_->pop(); 

      // do the signal processing with VSIPL++
      
      int FFTN = 100;
      // perform the STFT 
      spectBlocks_.push_back(new SpectBlock_t); 
      spectBlocks_.back().starttime = pwb->time; 
      spectBlocks_.back().endtime = pwb->time + pwb->data.size() * 1.0/pwb->samprate; 
      spectBlocks_.back().width = 1; 
      spectBlocks_.back().height = FFTN; 
      

      // this is slow to the point of embarrassment, but whatever, it's due tomorrow
      
      typedef Fft<const_Vector, cscalar_f, cscalar_f, fft_fwd> f_fft_type;
  
      // Create FFT objects
      f_fft_type f_fft(Domain<1>(FFTN), 1.0);

      // Allocate input and output buffers
      Vector<cscalar_f> in(FFTN, cscalar_f(1.f));
      Vector<cscalar_f> out(FFTN);
      
      // copy data to in:
      for (int i = 0; i < FFTN; i++) {
	in(i) = cscalar_f(pwb->data[i]); 
      }

      //Compute forward and inverse FFT's
      out = f_fft(in);

      spectBlocks_.back().data.reserve(FFTN*1*3);
 
      for (int i = 0; i < FFTN; i++) {
 	double mag = out(i).real() * out(i).real() + out(i).imag() * out(i).imag(); 
	
 	double scaledmag = mag; 
	//std::cout << "i = " << i  << " mag = "  << mag << " " << scaledmag << std::endl; 
 	spectBlocks_.back().data[i*3 + 0] = scaledmag; 
 	spectBlocks_.back().data[i*3 + 1] = scaledmag; 
 	spectBlocks_.back().data[i*3 + 2] = scaledmag; 
	
      }
      
      streamRenderer_.newSample(); 
      streamTrigger_.newSample(); 
      streamTriggerRenderer_.newSample(); 
    }
  

}

void SpectVis::invalidateData()
{


}

updateTriggersSignal_t & SpectVis::updateTriggersSignal()
{
  return streamTrigger_.updateTriggersSignal(); 
}



QueueView<wavetime_t>  SpectVis::getTriggerQueueView() 
{
  return streamTrigger_.getTriggerQueueView(); 
  
}

void SpectVis::setTriggerValue(wavetime_t level)
{
  streamTrigger_.setTriggerValue(level); 
}

void SpectVis::enableTrigger(bool value)
{

  streamTrigger_.enableTrigger(value); 

}



void SpectVis::updateTriggers(bool reset)
{

  streamRenderer_.updateTriggers(reset); 
  streamTriggerRenderer_.updateTriggers(reset); 

}

void SpectVis::setTriggerSource(const QueueView<wavetime_t> & tqv)
{

  streamRenderer_.setTriggerSource(tqv);
  streamTriggerRenderer_.setTriggerSource(tqv); 

}

float SpectVis::getYOffset(){
  return yoffset_; 
}

void SpectVis::setYOffset(float x)
{
  yoffset_ = x; 
}

void SpectVis::setYHeight(float x)
{
  yheight_ = x; 
  streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
}

void SpectVis::setVerticalScale(float volts){

  verticalScale_ = volts; 
  
  streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
  verticalScaleSignal_.emit(volts); 

}

float SpectVis::getVerticalScale()
{
  return verticalScale_; 
}

void SpectVis::setColor(Gdk::Color c)
{
  
  color_ = c; 
  streamRenderer_.setColor(c); 
  colorSignal_.emit(c); 
}

Gdk::Color SpectVis::getColor()
{
  return color_; 
}

GLWavePoint_t SpectVis::filterNextPoint(GLWavePoint_t wp)
{
  
  wp.t -= 0.1; // delay

  return wp; 



}


