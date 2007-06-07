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
  //std::cout << "Appending data" << p.t << ' ' << p.x <<  std::endl; 
  if (rates_.size() > 0) {
    assert(rates_.back().t < p.t ); 
  }
  rates_.push_back(p); 
  

  int S1N = 10; 

  if (ratesS2_.back().x[3] < p.x)
    ratesS2_.back().x[3] = p.x; 

  if (ratesS2_.back().x[1] > p.x)
    ratesS2_.back().x[1] = p.x; 
  ratesS2_.back().x[0] = p.t; 
  ratesS2_.back().x[2] = p.t; 

  if (rates_.size() % S1N == 0)
    {

      GLWaveQuadStrip_t s1poly = ratesS2_.back(); 
      s1poly.x[1] = 100e6; 
      s1poly.x[3] = -100e6; 
      
      ratesS2_.push_back(s1poly); 
    }

  
//   // check for triggers:
//   if (rates_.size() > 2) {
//     if (rates_.back().x > triggerLevel_ and 
// 	rates_[rates_.size() - 2].x <= triggerLevel_)
//       {
	
// 	TriggerTimeList_t ttl; 
// 	ttl.push_back(p.t); 
// 	newTriggerSignal_.emit(ttl); 
//       }
//   }

  if ( (mostRecentRenderT1_ <= p.t) and (mostRecentRenderT2_ >= p.t) )
    {
      invalidateLastRenderSignal_.emit(); 
    }


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

  //int pos1 = i1 - rates_.begin(); 
  int len  = i2 - i1; 

  if (scale > 100.0) {  
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		    &(*i1)); 
    glDrawArrays(GL_LINE_STRIP, 0, len); 
  }
  
  // scale 1 

//   p1.t = t1; 

//   i1 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
// 		   p1, compareTime); 
  
//   p2.t = t2; 
//   i2 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
//  		   p2, compareTime); 
  
  glColor4f(1.0, 1.0, 1.0, 1.0); 

//   int pos1 = i1 - rates_.begin(); 
//   len  = i2 - i1; 


  if (scale <= 100.0){
    glVertexPointer(2, GL_FLOAT, sizeof(float)*2, 
		    &ratesS2_[0]); 
    glDrawArrays(GL_QUAD_STRIP, 0, ratesS2_.size()*2); 
  }

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

// newTriggersSignal_t WaveStreamRenderer::newTriggers()
// {

//   return newTriggerSignal_; 

// }

// invalidateTriggersSignal_t WaveStreamRenderer::invalidateTriggers()
// {

//   return invalidateTriggersSignal_; 

// }


// void WaveStreamRenderer::setTriggerLevel(float tv)
// {

  


// }

// void WaveStreamRenderer::appendTriggers(const TriggerTimeList_t & ttl)
// {
//   // copy the waves

//   for (TriggerTimeList_t::const_iterator i = ttl.begin(); 
//        i != ttl.end(); i++)
//     {

//       trigTimeList_.push_back(*i);
//     }

// }

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
