#include <somanetwork/fakenetwork.h>

#include "streamsapp.h"

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Network network("127.0.0.1"); 

  Gtk::GL::init(argc, argv);
  
  StreamsApp vis(&network);
  network.run(); 

  kit.run(vis);

  return 0;
}
