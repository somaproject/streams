#ifndef NETDATAWAVEPROPERTY_H
#define NETDATAWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/netdatawave.h>
#include <propwidgets/spinbutton.h>
#include <propwidgets/combobox.h>

class NetDataWaveProperty : public PropertyPane { 
public:
  NetDataWaveProperty(); 
  ~NetDataWaveProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Adjustment scaleadj_; 
  PropertyWidgets::SpinButton scaleSpin_; 
  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 

  PropertyWidgets::ComboBox<NetDataWave::SourceType> sourceTypeComboBox_; 

};

#endif // WAVEVISPROPERTY_H
