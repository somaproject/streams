#ifndef NETDATAWAVEPROPERTY_H
#define NETDATAWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/netdatawave.h>
#include <propwidgets/spinbutton.h>

class NetDataWaveProperty : public PropertyPane { 
public:
  NetDataWaveProperty(); 
  ~NetDataWaveProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Adjustment scaleadj_; 
  PropertyWidgets::SpinButton scaleSpin_; 

};

#endif // WAVEVISPROPERTY_H
