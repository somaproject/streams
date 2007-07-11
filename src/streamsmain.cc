#include <somanetwork/fakenetwork.h>

#include "streamsapp.h"

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  FakeNetwork network; 

  Gtk::GL::init(argc, argv);
  
  StreamsApp vis(&network);

  kit.run(vis);

  return 0;
}
