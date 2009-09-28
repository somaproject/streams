#include <iostream> 
#include "spectvisrenderer.h"
#include "spectvistriggerrenderer.h"
#include "wave.h"
#include <assert.h>


SpectVisTriggerRenderer::SpectVisTriggerRenderer(SpectBlockpVector_t * pSpectBlocks) :
  pSpectBlocks_(pSpectBlocks), 
  emptyTriggerList_(), 
  triggerQueueView_(emptyTriggerList_)

{
 
}

SpectVisTriggerRenderer::~SpectVisTriggerRenderer()
{

}

void SpectVisTriggerRenderer::newSample()
{

}

void SpectVisTriggerRenderer::drawTriggers(float tbefore, float tafter, float timepoint)
{

  std::vector<GLWavePoint_t>::iterator  i1, i2;
  GLWavePoint_t p1, p2; 


  
//   // right now, we just plot the head
//   if (not trigTimeList_.empty() )
//     {
//       float tx= trigTimeList_.back(); 

//       p1.t = tx - tbefore;       
//       i1 = lower_bound(pSamples_->begin(), pSamples_->end(), 
// 		       p1, compareTime); 
      
//       p2.t = tx + tafter + 0.01; 
//       i2 = lower_bound(pSamples_->begin(), pSamples_->end(), 
// 		       p2, compareTime); 
//       if (i2 == pSamples_->end()) {
// 	i2 == --pSamples_->end(); 
//       }
      
      
//       glColor4f(1.0, 1.0, 1.0, 1.0); 
      
//       int len  = i2 - i1; 

//       glTranslatef(-tx, 0.0, 0.0); 

//       glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
// 		      &(*i1)); 
//       glDrawArrays(GL_LINE_STRIP, 0, len); 
//       // restore -- i should really learn to push and pop

//       glTranslatef(tx, 0.0, 0.0); 
      
//     }
}


sigc::signal<void> & SpectVisTriggerRenderer::invWaveSignal()
{
  return invWaveSignal_;
}

void SpectVisTriggerRenderer::updateTriggers(bool reset)
{
  if (reset)
    {
      
      trigTimeList_.clear(); 
      triggerQueueView_.reset(); 
    }
  else 
    {
      while ( not triggerQueueView_.empty()) {
	trigTimeList_.push_back(triggerQueueView_.front()); 
	triggerQueueView_.pop(); 
      }
      invWaveSignal_.emit(); 
    }
}

void SpectVisTriggerRenderer::setTriggerSource(const QueueView<float> & tqv)
{
  triggerQueueView_ = tqv; 
}
