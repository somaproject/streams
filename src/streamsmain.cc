#include <somanetwork/fakenetwork.h>

#include "streamsapp.h"

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  //Network network("127.0.0.1"); 

  Gtk::GL::init(argc, argv);
  
  pTimer_t ptimer = Timer::createDummyTimer(); 

  pSourceControl_t psc(new SourceControl(ptimer)); 
  pVisControl_t pvc(new VisControl); 


  StreamsApp vis(psc, pvc);

  kit.run(vis);

  return 0;
}
