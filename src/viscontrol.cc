#include "viscontrol.h"

VisControl::VisControl()
{




}

invStreamRenderSignal_t & VisControl::invStreamRenderSignal()
{
  return invStreamRenderSignal_; 

}

invTriggerRenderSignal_t & VisControl::invTriggerRenderSignal()
{
  return invTriggerRenderSignal_; 
}


void VisControl::emitInvStreamRenderSignal()
{
  invStreamRenderSignal_.emit(); 

}

void VisControl::emitInvTriggerRenderSignal()
{
  invTriggerRenderSignal_.emit(); 
}

void VisControl::connectVis(pIVis_t iv) {
  invStreamRenderSignal().connect(sigc::mem_fun(this, &VisControl::emitInvStreamRenderSignal)); 
  invTriggerRenderSignal().connect(sigc::mem_fun(this, &VisControl::emitInvTriggerRenderSignal)); 
  
}
