#ifndef VISCONTROLMONITOR_H
#define VISCONTROLMONITOR_H

#include "wavevis.h"
#include "spectvis.h"
#include "streamvis.h"

class VisControlMonitor
{
 public:
  virtual void create(pWaveVis_t pv) {}; 
  virtual void create(pSpectVis_t ps) {}; 

}; 

typedef boost::shared_ptr<VisControlMonitor> pVisControlMonitor_t; 

#endif // VISCONTROLMONITOR_H
