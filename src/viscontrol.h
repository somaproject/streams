#ifndef VISCONTROL_H
#define VISCONTROL_H

#include <boost/shared_ptr.hpp>
#include "viscontrolmonitor.h"

#include "streamsourcebase.h" 
#include "wavesource.h"
#include "streamvis.h"

class VisControl
{
public:
  VisControl(pVisControlMonitor_t pvcm); 
	     

  invWaveSignal_t & invMainWaveSignal(); 
  invWaveSignal_t & invTriggerWaveSignal(); 

  std::list<pStreamVisBase_t> & getVisList() {
    return visList_; }
  
  pStreamVisBase_t createVis(pWaveSource_t, std::string name); 

  

private:

  pVisControlMonitor_t pMonitor_; 
  invWaveSignal_t invMainWaveSignal_; 
  invWaveSignal_t invTriggerWaveSignal_; 
  
  std::list<pStreamVisBase_t> visList_; 


  void emitInvMainWaveSignal(); 
  void emitInvTriggerWaveSignal(); 
}; 

typedef boost::shared_ptr<VisControl> pVisControl_t; 


#endif // VISCONTROL_H
