#include <iostream> 
#include "wavestreamsource.h"
#include "wavestreamrenderer.h"
#include <assert.h>


WaveStreamRenderer::WaveStreamRenderer(std::vector<GLWavePoint_t> * pSamples ) :
  pSamples_(pSamples), 
  mostRecentRenderT1_(0.0), 
  mostRecentRenderT2_(0.0),
  emptyTriggerList_(), 
  triggerQueueView_(emptyTriggerList_),
  color_("red")
{
  // initialize data from stream source buffer
  
}



WaveStreamRenderer::~WaveStreamRenderer()
{
  
  
  
}

void WaveStreamRenderer::newSample()
{
  //std::cout << "Appending data" << p.t << ' ' << p.x <<  std::endl; 
  GLWavePoint_t p = pSamples_->back(); 
    

  int S1N = 10; 
  
  int N = pSamples_->size(); 
  
  if (ratesS2_.empty()) {
    GLWaveQuadStrip_t qs; 
    qs.xmin = p.x; 
    qs.xmax = p.x; 
    qs.tmin = p.t; 
    qs.tmax = p.t; 
    ratesS2_.push_back(qs); 
  } else if (N % S1N == 0) {
    // examine the last N points
    GLWavePoint_t minpoint, maxpoint; 
    minpoint.x = 100e6;
    maxpoint.x = -100e6; 
    for(int i = 0; i < S1N; i++) {
      int q =  N - i - 1; 
      GLWavePoint_t wp = (*pSamples_)[q]; 
      if (wp.x < minpoint.x)
	minpoint.x = wp.x; 
      
      if (wp.x > maxpoint.x)
	maxpoint.x = wp.x; 
    }

    // now we add a rectangle
    GLWaveQuadStrip_t pl, pr; 
    
    // left points
    pl.xmin = minpoint.x; 
    pl.xmax = maxpoint.x; 
    pl.tmin = (*pSamples_)[N-S1N].t; 
    pl.tmax = (*pSamples_)[N-S1N].t; 

    // right points
    pr.xmin = minpoint.x; 
    pr.xmax = maxpoint.x; 
    pr.tmin = (*pSamples_)[N-1].t; 
    pr.tmax = (*pSamples_)[N-1].t; 

    ratesS2_.push_back(pl); 
    ratesS2_.push_back(pr); 
      
    

  }
  
  if ( (mostRecentRenderT1_ <= p.t) and (mostRecentRenderT2_ >= p.t) )
    {
      invWaveSignal_.emit(); 
    }
  

}

void WaveStreamRenderer::draw(wavetime_t t1, wavetime_t t2, int pixels)
{


  // pixels is just a hint

  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  wavetime_t scale = pixels / (t2 -t1); 
  
  // draw the little range indicator
  setGLColor(0.8); 


  glBegin(GL_LINES); 
  glVertex2f(t1+1/scale, -float(pixheight_)/2); 
  glVertex2f(t1+1/scale,  float(pixheight_)/2); 
  glEnd(); 
  

  glLineWidth(1.0); 

  // perform scaling
  glPushMatrix(); 
  glScalef(1.0, scale_, 1.0); 
  

  // draw horizontal axis
  glColor4f(1.0, 1.0, 1.0, 1.0); 

  glBegin(GL_LINES);
  glVertex2f(t1, 0.0); 
  glVertex2f(t2, 0.0); 
  glEnd(); 

  std::vector<GLWavePoint_t>::iterator  i1, i2;
  GLWavePoint_t p1, p2; 

  p1.t = t1; 

  i1 = lower_bound(pSamples_->begin(), pSamples_->end(), 
		   p1, compareTime); 
  
  p2.t = t2; 
  i2 = lower_bound(pSamples_->begin(), pSamples_->end(), 
		   p2, compareTime); 
  

  int len  = i2 - i1; 
  float fadethold = 400.0; 
  if (scale > fadethold) {
    setGLColor(1.0); 
   } else {
     setGLColor(1.0 - (fadethold - scale)/200.0);
   }
  
  
  if (scale > 200.0) {  
     glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
 		    &(*i1)); 
     glDrawArrays(GL_LINE_STRIP, 0, len); 
     //std::cout << "rendering with len1 = " << len << std::endl; 

  }


  std::vector<GLWaveQuadStrip_t>::iterator  qi1, qi2;
  GLWaveQuadStrip_t qs1, qs2; 

  qs1.tmax = t1; 

  qi1 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
		   qs1, compareTime2); 
  
  qs2.tmax = t2; 
  qi2 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
		   qs2, compareTime2); 
  

  int len2 = qi2 - qi1; 


  if (scale < fadethold) {

    setGLColor((fadethold - scale)/fadethold  + 0.5); 
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), 
 		    &(ratesS2_[0])); 
   
    
    glDrawArrays(GL_QUAD_STRIP, 0, 2*ratesS2_.size()); 
    glDrawArrays(GL_LINES, 0, 2*ratesS2_.size()); 
    
  }
  
//   // stupid trigger rendering
//   std::vector<wavetime_t>::iterator trigi1, trigi2; 
//   trigi1 = lower_bound(trigTimeList_.begin(), 
//  		       trigTimeList_.end(), 
//  		       t1); 

//   trigi2 = lower_bound(trigTimeList_.begin(), 
//  		       trigTimeList_.end(), 
//  		       t2); 


//   //glColor4f(0.0, 1.0, 0.0, 1.0); 
//   printStatus();
//   //std::cout << "Rendering triggers!" << trigTimeList_.size() << std::endl; 
//    for(std::vector<wavetime_t>::iterator i = trigi1; 
//        i != trigi2; i++)
//      {

//        glBegin(GL_LINE_STRIP); 
//        glVertex2f(*i, -50); 
//        glVertex2f(*i, 50); 
//        glEnd(); 
//      }
      
  glPopMatrix(); 

}

sigc::signal<void> & WaveStreamRenderer::invWaveSignal()
{
  return invWaveSignal_;
}

void WaveStreamRenderer::updateTriggers(bool reset)
{
  if (reset) {

    trigTimeList_.clear(); 
    triggerQueueView_.reset(); 
  }
  else 
    {
      while ( not triggerQueueView_.empty()) {
	trigTimeList_.push_back(triggerQueueView_.front()); 
	triggerQueueView_.pop(); 
      }
      
    }
}

void WaveStreamRenderer::setTriggerSource(const QueueView<wavetime_t> & tqv)
{
  triggerQueueView_ = tqv; 
}

void WaveStreamRenderer::setScale(float scale, float pixheight)
{
  scale_ = scale; 
  pixheight_ = pixheight; 
  invWaveSignal_.emit(); 

}

void WaveStreamRenderer::setColor(Gdk::Color c)
{
  color_ = c; 
  invWaveSignal_.emit(); 

}

void WaveStreamRenderer::setGLColor(float alpha)
{
  float div = 1.0;
  glColor4f(color_.get_red_p()/div,
	    color_.get_green_p()/div,
	    color_.get_blue_p()/div, 
	    alpha); 
  
}

void WaveStreamRenderer::printStatus()
{
  std::cout << "samples size() =" << pSamples_->size() 
	    << " S2 size = " << ratesS2_.size() << std::endl; 


}
