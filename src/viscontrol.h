#ifndef VISCONTROL_H
#define VISCONTROL_H

#include <boost/shared_ptr.hpp>
#include <vis/istreamrenderer.h>
#include <vis/itriggerrenderer.h>

#include <vis/ivis.h>

class VisControl
{
public:
  VisControl(); 
  
  invStreamRenderSignal_t & invStreamRenderSignal(); 
  invTriggerRenderSignal_t & invTriggerRenderSignal(); 
  
  virtual std::list<pIVis_t> getVisList() {
    
  };  
  
protected: 
  
  //pVisControlMonitor_t pMonitor_; 
  invStreamRenderSignal_t invStreamRenderSignal_; 
  invTriggerRenderSignal_t invTriggerRenderSignal_; 
  
  void emitInvStreamRenderSignal(); 
  void emitInvTriggerRenderSignal(); 

  void connectVis(pIVis_t); 
  
}; 

typedef boost::shared_ptr<VisControl> pVisControl_t; 

#endif // VISCONTROL_H
