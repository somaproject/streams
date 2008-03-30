#include "teststreamsapp.h"
#include "sourcecontrol.h"
#include "sourcecontrolmonitor.h"
#include "viscontrolmonitor.h"
#include "viscontrol.h"


int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Gtk::GL::init(argc, argv);
  
  pTimer_t ptimer = Timer::createDummyTimer(); 
  pSourceControlMonitor_t pscm(new SourceControlMonitor); 
  pSourceControl_t psc(new SourceControl(pscm, ptimer)); 
  pVisControlMonitor_t pvcm(new VisControlMonitor); 
  pVisControl_t pvc(new VisControl(pvcm)); 

  pWaveSource_t fw = psc->createWaveSource("fake"); 
  pStreamVisBase_t wv = pvc->createVis(fw, "wave"); 

  wv->setYOffset(100.0); 

  TestStreamsApp teststreamsapp(psc, pvc); 

  kit.run(teststreamsapp);

  return 0;
}
