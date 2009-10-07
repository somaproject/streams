
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
  
//   std::cout << "WAVE2 VIS CREATED" << std::endl; 
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
  

  boost::shared_lock<boost::shared_mutex> lock(buffer_mutex_);
    
  buffer_map_t::iterator i = buffer_.lower_bound(t1); 
  if ((i != buffer_.begin()) and (i != buffer_.end())) {
    i--; 
  }
  while(i != buffer_.end()) { 
    double origintime = i->first - t1; 
//     std::cout << "time = " << i->first << std::endl;
    renderGLPointBuffer(origintime, i->second); 
    i++; 
  }
  glPopMatrix(); 

}


void WaveVis2::renderGLPointBuffer(double origintime, 
				   pGLPointBuffer_t  bufptr)
{
    glPushMatrix();
    glTranslatef(origintime, 0, 0); 
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), &(bufptr->data[0]) ); 
    glDrawArrays(GL_LINE_STRIP, 0, bufptr->size); 
    glPopMatrix(); 
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
//   std::cout << " WaveVis2::process(elements::timeid_t t)" << std::endl;
  if(scale.pendingRequest()) {
//     std::cout << "WaveVis2::process pending request scale" << std::endl;
    scale.set_value(scale.get_req_value()); 
  }

  int MAXCNT = 10; 
  int cnt = 0; 
  while (cnt < MAXCNT) {
    {
      if(pSinkPad_->dataqueue_.empty()) {
	break; 
      }
      boost::shared_ptr<elements::LinkElement<WaveBuffer_t> > le = pSinkPad_->dataqueue_.get(); 

      if (le->state  
	  == elements::LinkElement<WaveBuffer_t>::RESET) {
	
	boost::upgrade_lock<boost::shared_mutex> lock(buffer_mutex_);
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	buffer_.clear(); 

      } else { 
	
	WaveBuffer_t wb = le->datum; 
	
	pGLPointBuffer_t pb(new GLPointBuffer_t); 
	
	pb->size = 0; 
	double bufstarttime = (double)wb.time / 1e9; 
	
	double period = 1/wb.samprate; 
	// do the conversion
	for(int i = 0; i < wb.data.size(); i++) { 
	  pb->data[pb->size].t = period * i; 
	  pb->data[pb->size].x = wb.data[i]; 
	  pb->size++; 
	}
	// now stick in the buffer
	boost::upgrade_lock<boost::shared_mutex> lock(buffer_mutex_);
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	
	buffer_.insert(std::make_pair(bufstarttime, pb)); 
	cnt++; 
      }
    }
  }
  // FIXME: put in reset signal 
}
