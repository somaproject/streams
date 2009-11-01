
#include "wavevis2.h"
#include <iostream>

const std::string WaveVis2::TYPENAME = "WaveVis2"; 

WaveVis2::WaveVis2(std::string name, bf::path scratchdir):
  VisBase(name), 
  pSinkPad_(createSinkPad<pWaveBuffer_t>("default")),
  pixelHeight_(100), 
  //  color(Gdk::Color::parse("red"))
  scale(0.0),
  renderMode(false), 
  renderAllData(false),
  scratchdir_(scratchdir / name),
  renderall_(scratchdir_), 
  renderTest_(scratchdir_, 40), 
  most_recent_render_window_(0, 0), 
  most_recent_series_(0)
{
  using namespace wavevis2; 
  for(int i = 0; i < 8; i++) { 
    timeid_t time = 10000000000; 
    RenderDownSample * rds = new RenderDownSample(1000000 * (1 << i), 100, scratchdir_); 
    downsampledRenderers_.insert(std::make_pair(time * 1 << i, rds)); 
  }
  
}


WaveVis2::~WaveVis2()
{
  

}

// void WaveVis2::setScale(float voltsPerPixel) {
//   /* scale value is volts per pixel (?)
     
  
//   */
//   streamRenderer_.setScale(voltsPerPixel, pixelHeight_); 

// }

void WaveVis2::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  /*
    This is called from another thread
    
  */

  // draw the little range indicator
  //setGLColor(0.8); 

  glLineWidth(1.0); 
  
  // perform scaling
  glPushMatrix(); 
  glScalef(1.0, 1.0/scale, 1.0); 
  

  //   // draw horizontal axis
  glColor4f(1.0, 1.0, 1.0, 1.0); 

  glBegin(GL_LINES);
  glVertex2f(0, 0.0); 
  glVertex2f(t2-t1, 0.0); 
  glEnd(); 

  //std::cout << "t1 =" << t1 << "   t2=" << t2 << std::endl;
  // now we should be able to plot!

  // 
  // pick the scale
  streamtime_t windowsize = t2 - t1; 
  timeid_t windowsize_ns = timeid_t(windowsize * 1e9);

//   boost::shared_lock<boost::shared_mutex> lock(buffer_mutex_);
    
//   buffer_map_t::iterator i = buffer_.lower_bound(t1); 
//   if ((i != buffer_.begin()) and (i != buffer_.end())) {
//     i--; 
//   }
//   while(i != buffer_.end()) { 
//     double origintime = i->first - t1; 
// //     std::cout << "time = " << i->first << std::endl;
//     renderGLPointBuffer(origintime, i->second); 
//     i++; 
//   }

  timeid_t timeid_t1 = timeid_t(t1 * 1e9); 
  timeid_t timeid_t2 = timeid_t(t2 * 1e9); 

  if (renderAllData) { 
    renderall_.renderStream(timeid_t1, timeid_t2, pixels); 
  }
  if (renderMode) { 
    renderTest_.renderStream(timeid_t1, timeid_t2, pixels); 
//   } else { 
// //     if (!downsampledRenderers_.empty()) { 
// //       dsmap_t::iterator lb = downsampledRenderers_.upper_bound(windowsize_ns); 
// //       if (lb == downsampledRenderers_.end())  { 
// // 	lb--; 
// //       }
// //       //     std::cout << "rendering with window = " << windowsize_ns 
// //       // 	      << " and iterator = " << lb->first << std::endl;
// //       lb->second->renderStream(timeid_t1, timeid_t2, pixels); 
// //     }
//   }
  }
  glPopMatrix(); 

  most_recent_render_window_ = elements::timewindow_t(timeid_t1, timeid_t2); 

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


// void WaveVis2::newData()
// {
//   // our inputs are buffers of data, our filtered
//   // outputs are GLwavePoints
// //   while (not pSinkPad_->getpQueueView()->empty())
// //     {
// //       // we're taking in WaveBuffer_t pointers
// //       WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
// //       pSinkPad_->getpQueueView()->pop(); 
// //       streamRenderer_.newSample(wb); 

// //     }

// }

// void WaveVis2::invalidateData()
// {
//   streamRenderer_.invalidateSamples(); 
// //   while (not pSinkPad_->getpQueueView()->empty())
// //     {
// //       // we're taking in WaveBuffer_t pointers
// //       WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 
// //       pSinkPad_->getpQueueView()->pop(); 
// //       streamRenderer_.newSample(wb); 
// //     }

// }

// // updateTriggersSignal_t & WaveVis2::updateTriggersSignal()
// // {
// //   return streamTrigger_.updateTriggersSignal(); 
// // }



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
  if(scale.pendingRequest()) {
    scale.set_value(scale.get_req_value()); 
  }
  
  if(renderMode.pendingRequest()) {
    renderMode.set_value(renderMode.get_req_value()); 
  }
  
  if(renderAllData.pendingRequest()) {
    renderAllData.set_value(renderAllData.get_req_value()); 
  }
  
  if (most_recent_series_ != pSinkPad_->get_series()) {
    reset(); 
    most_recent_series_ = pSinkPad_->get_series(); 
  }
  
//   std::cout << "most recent series = " << most_recent_series_ << std::endl; 
  elements::timeinterval_t render_win_req( most_recent_render_window_.start,
				 most_recent_render_window_.end); 
  
  // heuristic to get "extra" time
  elements::timeid_t pre_extratime = 5 * elements::TIMEID_MS; 
  elements::timeid_t post_extratime = 5 * elements::TIMEID_MS; 
  
  // find the part of the render interval we don't have
  intervals_t reqset; 
  reqset += render_win_req; 
  intervals_t not_seen_but_needed = reqset - observed_intervals_; 
  
  // FIXME: never request "too much", always look for small chunks
  
  int interval_cnt = 0; 
  
  BOOST_FOREACH(elements::timeinterval_t t_int, not_seen_but_needed) {

    timeid_t l = t_int.lower(); 
    timeid_t u = t_int.upper(); 
    
    if (t_int.is_left(boost::itl::open_bounded)) {
      l++; 
    } 

    if (t_int.is_right(boost::itl::open_bounded)) {
      u--; 
    } 

    elements::timeinterval_t req_int(l, u); 

//     std::cout << "requesting interval " << req_int.as_string(); 
    elements::datawindow_t<pWaveBuffer_t> datawindow = pSinkPad_->get_src_data(elements::timewindow_t(req_int.lower() - pre_extratime, 
											    req_int.upper() + post_extratime)); 
//     std::cout << " and got back " << datawindow.interval.as_string() << std::endl; 
    

    renderTest_.newDataWindow(datawindow); 
    BOOST_FOREACH(pWaveBuffer_t wb, datawindow.data) {
      // DEBUG TEST
      bool seen_before_debug = debug_seen_times_.find(wb->time) == debug_seen_times_.end(); 
      bool in_interval(observed_intervals_.contains(wb->time)); 
      assert(in_interval != seen_before_debug); 

      debug_seen_times_.insert(wb->time); 
      
      if (!in_interval) { // this check is so that
	// we don't waste time rendering things we've already seen. 
	
	renderall_.newSample(wb); 

	BOOST_FOREACH(dsmap_t::value_type & i, downsampledRenderers_) {
	  i.second->newSample(wb);
	}
      }
    }

    observed_intervals_ += datawindow.interval; 

    interval_cnt++; 
  }

}

void WaveVis2::reset()
{
  std::cout << "Beginning reset " << std::endl; 
  debug_seen_times_.clear(); 

  observed_intervals_.clear(); 
  renderall_.reset(); 
  BOOST_FOREACH(dsmap_t::value_type & i, downsampledRenderers_) {
    i.second->reset(); 
    std::cout << "Downsample had a successful reset" << std::endl; 
  }
  std::cout << "reset done " << std::endl; 


}
