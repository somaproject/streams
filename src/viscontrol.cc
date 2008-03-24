#include "viscontrol.h"
#include "wavevis.h" 
#include "spectvis.h" 

VisControl::VisControl()
{




}

pStreamVisBase_t VisControl::createVis(std::string name)
{
  pStreamVisBase_t psvb(new SpectVis()); 

  visList_.push_back(psvb); 
  psvb->invMainWaveSignal().connect(sigc::mem_fun(*this, 
						  &VisControl::emitInvMainWaveSignal)); 
  psvb->invTriggerWaveSignal().connect(sigc::mem_fun(*this, 
						  &VisControl::emitInvTriggerWaveSignal)); 
  return psvb; 
  
}

invWaveSignal_t & VisControl::invMainWaveSignal()
{
  return invMainWaveSignal_; 

}

invWaveSignal_t & VisControl::invTriggerWaveSignal()
{
  return invTriggerWaveSignal_; 
}


void VisControl::emitInvMainWaveSignal()
{
  invMainWaveSignal_.emit(); 

}

void VisControl::emitInvTriggerWaveSignal()
{
  invTriggerWaveSignal_.emit(); 
}
