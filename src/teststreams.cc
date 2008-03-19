#include "teststreamsapp.h"
#include "sourcecontrol.h"
#include "viscontrol.h"


int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Gtk::GL::init(argc, argv);
  
  pTimer_t ptimer = Timer::createDummyTimer(); 

  pSourceControl_t psc(new SourceControl(ptimer)); 
  pVisControl_t pvc(new VisControl); 

  pStreamSourceBase_t fw = psc->createSource("test"); 
  pStreamVisBase_t wv = pvc->createVis("test"); 
  wv->setYOffset(100.0); 

  wv->connectSource(fw); 

  TestStreamsApp teststreamsapp(psc, pvc); 

  kit.run(teststreamsapp);

  return 0;
}
