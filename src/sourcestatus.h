#ifndef SOURCESTATUS_H
#define SOURCESTATUS_H
#include "streamsource.h"


class SourceStatus: public Gtk::HBox
{
 public: 
  virtual pStreamSourceBase_t getSourcePtr() = 0; 

}; 


#endif // SOURCESTATUS_H

