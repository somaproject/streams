#include <iostream> 
#include "wavestreamsource.h"
#include "wavestreamrenderer.h"
#include <assert.h>


WaveStreamRenderer::WaveStreamRenderer(std::vector<GLWavePoint_t> * pSamples ) :
  pSamples_(pSamples), 
  mostRecentRenderT1_(0.0), 
  mostRecentRenderT2_(0.0),
  emptyTriggerList_(), 
  triggerQueueView_(emptyTriggerList_)

{
  // initialize data from stream source buffer
  GLWaveQuadStrip_t s1poly;
  ratesS2_.push_back(s1poly); 
 
}



WaveStreamRenderer::~WaveStreamRenderer()
{
  
  
  
}

void WaveStreamRenderer::newSample()
{
  //std::cout << "Appending data" << p.t << ' ' << p.x <<  std::endl; 
  GLWavePoint_t p = pSamples_->back(); 
    

  int S1N = 10; 

  if (ratesS2_.back().xmax < p.x) {
    ratesS2_.back().xmax = p.x; 
    ratesS2_.back().tmax = p.t; 
  }
  if (ratesS2_.back().xmin > p.x){
    ratesS2_.back().xmin = p.x; 
    ratesS2_.back().tmin = p.t; 
  }


  if (pSamples_->size() % S1N == 0)
    {

      GLWaveQuadStrip_t s1poly = ratesS2_.back(); 
      s1poly.xmin = 100e6; 
      s1poly.xmax = -100e6; 
      
      ratesS2_.push_back(s1poly); 
    }
  
  


  if ( (mostRecentRenderT1_ <= p.t) and (mostRecentRenderT2_ >= p.t) )
    {
      invWaveSignal_.emit(); 
    }


}

void WaveStreamRenderer::draw(wavetime_t t1, wavetime_t t2, int pixels)
{
  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  wavetime_t scale = pixels / (t2 -t1); 

  std::vector<GLWavePoint_t>::iterator  i1, i2;
  GLWavePoint_t p1, p2; 

  p1.t = t1; 

  i1 = lower_bound(pSamples_->begin(), pSamples_->end(), 
		   p1, compareTime); 
  
  p2.t = t2; 
  i2 = lower_bound(pSamples_->begin(), pSamples_->end(), 
		   p2, compareTime); 
  

  int len  = i2 - i1; 
  int fadethold = 400.0; 
  if (scale > fadethold) {
    glColor4f(1.0, 1.0, 1.0, 1.0); 
  } else {
    glColor4f(1.0, 1.0, 1.0, 1.0 - (fadethold - scale)/200.0);
  }

  std::cout << scale << std::endl;
  if (scale > 200.0) {  
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		    &(*i1)); 
    glDrawArrays(GL_LINE_STRIP, 0, len); 
  }
  
  // first scale 
  std::vector<GLWaveQuadStrip_t>::iterator  qi1, qi2;

  GLWaveQuadStrip_t q1, q2; 
  q1.tmax = t1; 
  q1.tmin = t1; 

  
  qi1 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
 		   q1, &compareTime2); 
  
  q2.tmax = t2; 
  q2.tmin = t2; 

  qi2 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
  		   q2, &compareTime2); 
  



  len  = qi2 - qi1; 

  glColor4f(1.0, 1.0, 1.0, fadethold-scale/200.0); 
  if (scale <= 400.0){
    glVertexPointer(2, GL_FLOAT, sizeof(float)*2, 
 		    &(*qi1)); 
     glDrawArrays(GL_QUAD_STRIP, 0, 2 * len); 
  }

  // stupid trigger rendering
  std::vector<wavetime_t>::iterator trigi1, trigi2; 
  trigi1 = lower_bound(trigTimeList_.begin(), 
 		       trigTimeList_.end(), 
 		       t1); 

  trigi2 = lower_bound(trigTimeList_.begin(), 
 		       trigTimeList_.end(), 
 		       t2); 

  glColor4f(0.0, 1.0, 0.0, 1.0); 

  //std::cout << "Rendering triggers!" << trigTimeList_.size() << std::endl; 
   for(std::vector<wavetime_t>::iterator i = trigi1; 
       i != trigi2; i++)
     {

       glBegin(GL_LINE_STRIP); 
       glVertex2f(*i, -50); 
       glVertex2f(*i, 50); 
       glEnd(); 
     }
      

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
