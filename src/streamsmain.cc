#include "streamsapp.h"

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  //
  // Init gtkglextmm.
  //

  Gtk::GL::init(argc, argv);

  
  StreamsApp vis(true);

  kit.run(vis);

  return 0;
}
