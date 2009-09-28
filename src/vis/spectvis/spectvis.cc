
#include "spectvis.h"
#include <iostream>
#include <vsip/initfin.hpp>
#include <vsip/support.hpp>
#include <vsip/signal.hpp>
#include <vsip/math.hpp>
#include <vsip/core/profile.hpp>

const std::string SpectVis::TYPENAME = "SpectVis"; 

SpectVis::SpectVis(std::string name, bf::path scratch) :
  VisBase(name), 
  pSinkPad_(createSinkPad<WaveBuffer_t>("default")), 
  streamRenderer_(&spectBlocks_), 
  yheight_(100),
  color_("red"),
  verticalScale_(1.0),
  scratchdir_(scratch / name),
  vpp_(),
  pixelHeight_(100)
{
  using namespace vsip;
  using namespace vsip_csl;
  using namespace vsip::impl::profile;



  pSinkPad_->newDataSignal().connect(sigc::mem_fun(*this, 
						   &SpectVis::newData)); 
  
}

void SpectVis::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state
  streamRenderer_.renderStream(t1, t2, pixels); 
  
}

SpectVis::~SpectVis()
{
  

}


void SpectVis::newData()
{

  using namespace vsip;
  using namespace vsip_csl;
  using namespace vsip::impl::profile;


  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
  while (not pSinkPad_->getpQueueView()->empty())
    {
      // we're taking in WaveBuffer_t pointers

      WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
      pSinkPad_->getpQueueView()->pop(); 

      // do the signal processing with VSIPL++
      
      int FFTN = 128;
      // perform the STFT 
      spectBlocks_.push_back(new SpectBlock_t); 
      spectBlocks_.back().starttime = wb.time; 
      spectBlocks_.back().endtime = wb.time + wb.data.size() * 1.0/wb.samprate; 
      spectBlocks_.back().width = 1; 
      spectBlocks_.back().height = FFTN; 
      

      // this is slow to the point of embarrassment, but whatever, it's due tomorrow
      
      typedef Fft<const_Vector, cscalar_f, cscalar_f, fft_fwd> f_fft_type;
  
      // Create FFT objects
      f_fft_type f_fft(Domain<1>(FFTN), 1.0);

//       // Allocate input and output buffers

      Vector<cscalar_f> in(FFTN);
      in = cscalar_f(1.0); 
      Vector<cscalar_f> out(FFTN);
      
      // copy data to in:
      for (int i = 0; (i < FFTN) and ( i < wb.data.size()); i++) {
 	in(i) = cscalar_f(wb.data[i]); 
      }

//       //Compute forward and inverse FFT's
      out = f_fft(in);

      spectBlocks_.back().data.resize(FFTN*1);
 
      for (int i = 0; i < FFTN; i++) {
 	double mag = i; // out(i).real() * out(i).real() + out(i).imag() * out(i).imag(); 
	
 	double scaledmag = mag; 
	//std::cout << "i = " << i  << " mag = "  << mag << " " << scaledmag << std::endl; 
 	spectBlocks_.back().data[i] = scaledmag; 
      }
      
      streamRenderer_.newSample(); 

    }
  

}

void SpectVis::invalidateData()
{


}

// float SpectVis::getYOffset(){
//   return yoffset_; 
// }

// void SpectVis::setYOffset(float x)
// {
//   yoffset_ = x; 
// }

// void SpectVis::setYHeight(float x)
// {
//   yheight_ = x; 
//   streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
// }

// void SpectVis::setVerticalScale(float volts){

//   verticalScale_ = volts; 
  
//   streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
//   verticalScaleSignal_.emit(volts); 

// }

// float SpectVis::getVerticalScale()
// {
//   return verticalScale_; 
// }

// void SpectVis::setColor(Gdk::Color c)
// {
  
//   color_ = c; 
//   streamRenderer_.setColor(c); 
//   colorSignal_.emit(c); 
// }

// Gdk::Color SpectVis::getColor()
// {
//   return color_; 
// }

// GLWavePoint_t SpectVis::filterNextPoint(GLWavePoint_t wp)
// {
  
//   wp.t -= 0.1; // delay

//   return wp; 



// }




void SpectVis::renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels)
{


}



void SpectVis::setPixelHeight(int x)
{

  pixelHeight_ = x; 

}

int SpectVis::getPixelHeight()
{
  return pixelHeight_; 

}

void SpectVis::setScale(float)
{


}
