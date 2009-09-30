#include <iostream>
#include <boost/foreach.hpp> 
#include "wavevisrenderer.h"
#include <assert.h>


WaveVisRenderer::WaveVisRenderer(bf::path scratch) :
  scale_(1.0), 
  color_("red"),
  scratchdir_(scratch)
{
  // low value
  
  for (int i = 0; i < 16; i++) {
    renderers_[10.0 * (1<<i)] = new RenderDownSample(0.0005 * (1 << i), scratchdir_); 
  }

}



WaveVisRenderer::~WaveVisRenderer()
{

  BOOST_FOREACH(rendermap_t::value_type & r, renderers_) { 
    delete r.second;
  }
  
  
  
}

void WaveVisRenderer::newSample(const WaveBuffer_t & wb)
{
  /* Take in a wave buffer and convert it to a GLPointBuffer_t
     of points. 
     
  */
  // construct and append new buffer
  BOOST_FOREACH(rendermap_t::value_type & r, renderers_) { 
    r.second->newSample(wb); 
  }
  
  
}

void WaveVisRenderer::renderStream(streamtime_t t1, streamtime_t t2, 
				   int pixels)
{
  // pixels is just a hint
  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  streamtime_t scale = pixels / (t2 -t1); 
  
  // draw the little range indicator
  setGLColor(0.8); 


//   glBegin(GL_LINES); 
//   glVertex2f(1/scale, -float(pixheight_)/2); 
//   glVertex2f(1/scale,  float(pixheight_)/2); 
//   glEnd(); 
  

  glLineWidth(1.0); 
  
  // perform scaling
  glPushMatrix(); 
  glScalef(1.0, 1.0/scale_, 1.0); 
  

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
  
  rendermap_t::iterator lb= renderers_.lower_bound(windowsize);
  if (lb != renderers_.end()) { 
//     std::cout << "rendering with " << lb->first << std::endl; 
    lb->second->renderStream(t1, t2, pixels); 
  } else {
    // just render with the last one
    lb--; 
    lb->second->renderStream(t1, t2, pixels); 

  }
  glPopMatrix(); 

}

sigc::signal<void> & WaveVisRenderer::invWaveSignal()
{
  //  return invWaveSignal_;
}


void WaveVisRenderer::setScale(float scale, float pixheight)
{
  scale_ = scale; 
  pixheight_ = pixheight; 
  invWaveSignal_.emit(); 

}

void WaveVisRenderer::setColor(Gdk::Color c)
{
  color_ = c; 
  invWaveSignal_.emit(); 

}

void WaveVisRenderer::setGLColor(float alpha)
{
  float div = 1.0;
  glColor4f(color_.get_red_p()/div,
	    color_.get_green_p()/div,
	    color_.get_blue_p()/div, 
	    alpha); 
  
  glColor4f(1.0, 1.0, 1.0, 1.0); // FIXME
  
}

void WaveVisRenderer::printStatus()
{


}

void WaveVisRenderer::invalidateSamples()
{
  
  BOOST_FOREACH(rendermap_t::value_type & r, renderers_) { 
    r.second->reset(); 
  }

}
