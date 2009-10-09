
#include "spectvis.h"
#include <iostream>
#include <boost/format.hpp>


const std::string SpectVis::TYPENAME = "SpectVis"; 

SpectVis::SpectVis(std::string name, bf::path scratch) :
  VisBase(name), 
  scale(1.0), 
  fftN(128), 
  windowsize(1.0),
  overlapFactor(1.0),
  pSinkPad_(createSinkPad<WaveBuffer_t>("default")), 
  yheight_(100),
  verticalScale_(1.0),
  scratchdir_(scratch / name),
  pixelHeight_(100),
  spectblockdb_(new Db(NULL, 0))
{

  streamRenderer_ = new SpectVisRenderer(spectblockdb_); 

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
}


void SpectVis::newData()
{
  // right now this is just a stupid no-op


  //  char * buffer = (char*)malloc(SpectRenderBlock::maxbytes()); 
  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
//   while (not pSinkPad_->getpQueueView()->empty())
//     {
//       // we're taking in WaveBuffer_t pointers

//       WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 

//       SpectRenderBlock sbb(wb.data.size(), 1); 

//       sbb.starttime = wb.time; 
//       double starttime = sbb.starttime; 
//       sbb.endtime = wb.time + wb.data.size() * 1.0 / wb.samprate;

//       sbb.lowfreq = 0;
//       sbb.highfreq = 100; 
      
//       int size = sbb.marshall_to_buffer(buffer); 
//       Dbt key(&(starttime), sizeof(starttime));
//       Dbt data(buffer, size); 
//       int ret = spectblockdb_->put(NULL, &key, &data, DB_NOOVERWRITE);
//       if (ret != 0) {
// 	spectblockdb_->err(ret, "Put failed because key %f already exists", sbb.starttime);
//       }
      
//       pSinkPad_->getpQueueView()->pop();       
//     }
  
//   free(buffer); 
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


void SpectVis::process(elements::timeid_t id)
{
  
  if(scale.pendingRequest()) {
//     std::cout << "WaveVis2::process pending request scale" << std::endl;
    scale.set_value(scale.get_req_value()); 
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
	
	//newSample(wb); 
//  	renderall_.newSample(wb); 
// 	BOOST_FOREACH(dsmap_t::value_type & i, downsampledRenderers_) {
// 	  i.second->newSample(wb);
// 	}

 	cnt++; 
      }
    }
  }

  
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


}

// void SpectVis::newSample(const WaveBuffer_t & wb) 
// {
//   // 1. add buffer range to range list. 

//   // 2. get the implicated buffers

//   // for each buffer:
//   //   is it complete? if so, mark it as done-able. 
//   //   -- reasonable heuristic -- in general, arrival of a packet will complete a buffer
//   // 

// }

// void SpectVis::computeFFTs()
// {
//   /* 
//      Examine the list of FFT windows that we could deal with but haven't yet, and
//      FFT them. 
     
//      // get the buffer
//   */ 
  
  
//   /* Stick the results in the FFT DB
     

//    */ 

// }
