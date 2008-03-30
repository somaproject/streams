#ifndef SOURCECONTROLMONITOR_H
#define SOURCECONTROLMONITOR_H

#include "fakewave.h"

class SourceControlMonitor
{
 public:
  virtual  void create(pFakeWave_t fw) {}; 

}; 

typedef boost::shared_ptr<SourceControlMonitor> pSourceControlMonitor_t; 

#endif // SOURCECONTROLMONITOR_H
