#include "fakewave.h" 


#include "sourcecontrol.h"

SourceControl::SourceControl(pTimer_t pt) :
  pTimer_(pt)
{
  // 
  
  
}

pStreamSourceBase_t SourceControl::createSource(std::string name)
{
  return pStreamSourceBase_t(new FakeWave(pTimer_)); 
  
}
