#include <iostream> 
#include "wavevisrenderer.h"
#include <assert.h>


WaveVisRenderer::WaveVisRenderer() :
  scale_(1.0), 
  color_("red")
{

  
}



WaveVisRenderer::~WaveVisRenderer()
{
  
  
  
}

void WaveVisRenderer::newSample(const WaveBuffer_t & wb)
{
  /* Take in a wave buffer and convert it to a GLPointBuffer_t
     of points. 
     
  */
  // construct and append new buffer
  std::vector<float>::const_iterator pdatum; 
  GLWavePoint_t p; 
  streamtime_t bufferstart = wb.time; 

  tbm_[bufferstart] = new GLPointBuffer_t; 
  timeBufferMap_t::iterator tbmi = tbm_.find(bufferstart); 
  
  float T = 1.0 / wb.samprate; 
  int pos = 0; 
  for(pdatum = wb.data.begin();
      pdatum != wb.data.end(); pdatum++) {
    p.x = *pdatum; 
    p.t = pos * T; 
    tbmi->second->push_back(p); 
    pos++; 
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

//   glBegin(GL_LINES);
//   glVertex2f(0, 0.0); 
//   glVertex2f(t2-t1, 0.0); 
//   glEnd(); 

  //std::cout << "t1 =" << t1 << "   t2=" << t2 << std::endl;
  // now we should be able to plot!

  // 
  timeBufferMap_t::iterator lb= tbm_.lower_bound(t1); 
  if (lb !=tbm_.begin()) 
    lb--; 

  while(lb->first < t2 and lb != tbm_.end()) { 
    glPushMatrix();
    glTranslatef(lb->first - t1, 0, 0); 

    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
 		    &((*(lb->second))[0])); 
    glDrawArrays(GL_LINE_STRIP, 0, lb->second->size()); 
    glPopMatrix(); 
    lb++; 
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
