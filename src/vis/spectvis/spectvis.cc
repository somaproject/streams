#include <fftw3.h>
#include <iostream>
#include <boost/format.hpp>

#include "spectvis.h"




const std::string SpectVis::TYPENAME = "SpectVis"; 
using namespace spectvis; 

class Identity : public spectvis::IFFTop { 
public:
  pFFT_t operator() (float * data, int data_size, int N, float fs)
  {
    
    //Identity operation that just copies the data 
    pFFT_t y(new FFT); 
    y->data.reserve(data_size); 
    for (int i = 0; i < data_size; i++) { 
      y->data.push_back(data[i]); 
    }
    y->N = y->data.size(); 
    y->maxfreq = fs/2; 
    return y; 
  }
};

SpectVis::SpectVis(std::string name, bf::path scratch) :
  VisBase(name), 
  scale(1.0), 
  fftN(512), 
  windowsize(1.0),
  overlapFactor(1),
  cmap_max(1000.0), 
  pSinkPad_(createSinkPad<WaveBuffer_t>("default")), 
  yheight_(100),
  verticalScale_(1.0),
  scratchdir_(scratch / name),
  pixelHeight_(100),
  fftengine_(),
  dscache_(40, 1)
{
  
  std::cout << "Creating SpectVIs" << std::endl;
  streamRenderer_ = new spectvis::SpectVisRenderer(fftengine_, dscache_); 
  fftengine_.newfft_signal().connect(sigc::mem_fun(*this, &SpectVis::on_new_fft)); 

  //pfftwop_ = new Identity; // FFTW(fftN, 4096); 
  pfftwop_ = new FFTW(fftN, 1<<16); 
  fftengine_.set_op(pfftwop_); 

  fftengine_.set_fftN(fftN); 
  fftengine_.set_windowsize(windowsize*1e9) ; 
  fftengine_.set_overlapFactor(overlapFactor); 
  
}

void SpectVis::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{

  shared_lock_t trunclock(truncate_mutex_); 

  // i really hate how this modifies the gloabl GL state
  streamRenderer_->renderStream(t1, t2, pixels); 
  
}

SpectVis::~SpectVis()
{
  
  delete streamRenderer_; 
  delete pfftwop_; 

}



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


void SpectVis::process(elements::timeid_t id)
{
  
  // --------------------------------------------------------------
  // GUI properties
  // --------------------------------------------------------------
  if(scale.pendingRequest()) {
//     std::cout << "WaveVis2::process pending request scale" << std::endl;
    scale.set_value(scale.get_req_value()); 
    streamRenderer_->set_vscale(scale); 
  }

  if(cmap_max.pendingRequest()) {
    cmap_max.set_value(cmap_max.get_req_value()); 
    streamRenderer_->set_cmap_max(cmap_max); 

  }

  // --------------------------------------------------------------
  // properties that cause a recomputation
  // --------------------------------------------------------------
  bool recompute  = false; 
  if(fftN.pendingRequest()) { 
    fftN.set_value(fftN.get_req_value()); 
    fftengine_.set_fftN(fftN); 
    recompute = true; 
  }
  
  if(windowsize.pendingRequest()) { 
    windowsize.set_value(windowsize.get_req_value()); 
    fftengine_.set_windowsize(windowsize*1e9) ; 
    std::cout << "setting windowsize to " << windowsize << std::endl; 
    recompute = true; 
  }

  if(overlapFactor.pendingRequest()) { 
    overlapFactor.set_value(overlapFactor.get_req_value()); 
    fftengine_.set_overlapFactor(overlapFactor); 
    recompute = true; 
  }
  
  if (recompute) { 
    fftengine_.recompute(); 
    dscache_.reset(); 
  }

  int MAXCNT = 10; 
  int cnt = 0; 
  while (cnt < MAXCNT) {
    {

      if(pSinkPad_->commandqueue_.empty() == false) { 
	elements::MESSAGES m = pSinkPad_->commandqueue_.get(); 
	if (m == elements::RESET) { 
	  while(pSinkPad_->dataqueue_.empty()	  ) {
	    // consume remaining data until reset
	    boost::shared_ptr<elements::LinkElement<WaveBuffer_t> > le = pSinkPad_->dataqueue_.get(); 
	    if (le->state == 
		elements::LinkElement<WaveBuffer_t>::RESET) {
	      reset(); 
	      break; 
	    }

	  }

	} else {

	  std::cerr << "Unknown command?" << std::endl; 
	}
      }
      
      if(pSinkPad_->dataqueue_.empty()) {
	break; 
      }
      boost::shared_ptr<elements::LinkElement<WaveBuffer_t> > le = pSinkPad_->dataqueue_.get(); 
      
      if (le->state  
	  == elements::LinkElement<WaveBuffer_t>::RESET) {
	
	reset(); 
      } else { 
	
	WaveBuffer_t wb = le->datum; 
	fftengine_.appendData(wb); 
	
      }
      cnt++; 
      
    }
  }
  fftengine_.process(MAXCNT); 
  
}


void SpectVis::reset()
{
  std::cout << "Beginning reset " << std::endl; 
//   renderall_.reset(); 
//   BOOST_FOREACH(dsmap_t::value_type & i, downsampledRenderers_) {
//     i.second->reset(); 
//     std::cout << "Downsample had a successful reset" << std::endl; 
//   }
  std::cout << "reset done " << std::endl; 
  fftengine_.reset(); 
  dscache_.reset(); 

}

void SpectVis::recompute()
{

  dscache_.reset();   
  fftengine_.recompute() ; 

}

void SpectVis::on_new_fft(spectvis::pFFT_t fft)
{
  dscache_.addFFT(fft); 

}
