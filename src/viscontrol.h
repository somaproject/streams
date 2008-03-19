#ifndef VISCONTROL_H
#define VISCONTROL_H

#include <boost/shared_ptr.hpp>
#include "streamsourcebase.h" 
#include "streamvis.h"

class VisControl
{
public:
  VisControl(); 

  invWaveSignal_t & invMainWaveSignal(); 
  invWaveSignal_t & invTriggerWaveSignal(); 

  std::list<pStreamVisBase_t> & getVisList() {
    return visList_; }
  
  pStreamVisBase_t createVis(std::string name); 

  

private:
  invWaveSignal_t invMainWaveSignal_; 
  invWaveSignal_t invTriggerWaveSignal_; 
  
  std::list<pStreamVisBase_t> visList_; 

  void emitInvMainWaveSignal(); 
  void emitInvTriggerWaveSignal(); 
}; 

typedef boost::shared_ptr<VisControl> pVisControl_t; 


#endif // VISCONTROL_H
