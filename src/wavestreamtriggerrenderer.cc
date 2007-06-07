#include <iostream> 
#include "wavestreamsource.h"
#include "wavestreamrenderer.h"
#include <assert.h>


WaveStreamRenderer::WaveStreamRenderer() :
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

void WaveStreamRenderer::append(GLWavePoint_t p)
{

  if (rates_.size() > 0) {
    assert(rates_.back().t < p.t ); 
  }
  rates_.push_back(p); 

}

void WaveStreamRenderer::draw(float t1, float t2, int pixels)
{
  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  float scale = pixels / (t2 -t1); 

  std::vector<GLWavePoint_t>::iterator  i1, i2;
  GLWavePoint_t p1, p2; 

  p1.t = t1; 

  i1 = lower_bound(rates_.begin(), rates_.end(), 
		   p1, compareTime); 
  
  p2.t = t2; 
  i2 = lower_bound(rates_.begin(), rates_.end(), 
		   p2, compareTime); 
  
  
  glColor4f(1.0, 1.0, 1.0, 1.0); 

  int len  = i2 - i1; 

  if (scale > 100.0) {  
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
  
  glColor4f(1.0, 0.0, 0.0, 0.5); 


  len  = qi2 - qi1; 


  //  if (scale <= 100.0){
    glVertexPointer(2, GL_FLOAT, sizeof(float)*2, 
		    &(*qi1)); 
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * len); 
    //}

  // stupid trigger rendering
  std::vector<float>::iterator trigi1, trigi2; 
  trigi1 = lower_bound(trigTimeList_.begin(), 
 		       trigTimeList_.end(), 
 		       t1); 

  trigi2 = lower_bound(trigTimeList_.begin(), 
 		       trigTimeList_.end(), 
 		       t2); 

  glColor4f(0.0, 1.0, 0.0, 1.0); 

  //std::cout << "Rendering triggers!" << trigTimeList_.size() << std::endl; 
   for(std::vector<float>::iterator i = trigi1; 
       i != trigi2; i++)
     {

       glBegin(GL_LINE_STRIP); 
       glVertex2f(*i, -50); 
       glVertex2f(*i, 50); 
       glEnd(); 
     }
      

}


sigc::signal<void> & WaveStreamRenderer::invalidateLastRenderSignal()
{
  return invalidateLastRenderSignal_;
}

void WaveStreamRenderer::resetTriggers()
{

  trigTimeList_.clear(); 
  triggerQueueView_.reset(); 
}
void WaveStreamRenderer::newTriggers()
{
  while ( not triggerQueueView_.empty()) {
    trigTimeList_.push_back(triggerQueueView_.front()); 
    triggerQueueView_.pop(); 
  }

}

void WaveStreamRenderer::setTriggerSource(const QueueView<float> & tqv)
{
  triggerQueueView_ = tqv; 
}
