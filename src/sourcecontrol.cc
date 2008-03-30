#include "fakewave.h" 
#include "sourcecontrol.h"

SourceControl::SourceControl(pSourceControlMonitor_t scm, pTimer_t pt) :
  pTimer_(pt),
  pMonitor_(scm)
{
  // 
  
  
}

pWaveSource_t SourceControl::createWaveSource(std::string name)
{
  if (name == "fake") {
    pFakeWave_t pfw(new FakeWave(pTimer_));
    pMonitor_->create(pfw); // notify the monitor
    return pfw; 

  } else {
    // FIXME Throw error
    std::cout << "THIS IS NOT A VALID SOURCE" << std::endl; 
  }

  
}
