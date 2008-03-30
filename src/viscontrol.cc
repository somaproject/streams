#include "viscontrol.h"
#include "wavevis.h" 
#include "spectvis.h" 

VisControl::VisControl(pVisControlMonitor_t pvcm) :
  pMonitor_(pvcm)
{




}

pStreamVisBase_t VisControl::createVis(pWaveSource_t pws, std::string name)
{
  if (name == "wave") {
    pWaveVis_t psv(new WaveVis()); 
    
    psv->connectSource(pws);
    visList_.push_back(psv); 
    psv->invMainWaveSignal().connect(sigc::mem_fun(*this, 
						    &VisControl::emitInvMainWaveSignal)); 
    psv->invTriggerWaveSignal().connect(sigc::mem_fun(*this, 
						       &VisControl::emitInvTriggerWaveSignal)); 
    
    
    pMonitor_->create(psv); 
    return psv; 

  } else {
    std::cout << "NOT FOUND" << std::endl;  // FIXME

  } 
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
