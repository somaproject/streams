#ifndef SOURCECONTROL_H
#define SOURCECONTROL_H

#include "sourcecontrolmonitor.h"
#include "wavesource.h"
#include "timer.h"

class SourceControl
{
public:
  SourceControl(pSourceControlMonitor_t scm, pTimer_t); 
  pWaveSource_t createWaveSource(std::string); 
  
private:
  pTimer_t pTimer_; 
  pSourceControlMonitor_t pMonitor_; 
  std::list<pStreamSourceBase_t> sourceList_; 
  
}; 

typedef boost::shared_ptr<SourceControl> pSourceControl_t; 


#endif // SOURCECONTROL_H
