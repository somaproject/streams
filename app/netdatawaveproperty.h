#ifndef NETDATAWAVEPROPERTY_H
#define NETDATAWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/netdatawave.h>
#include <propwidgets/spinbutton.h>
#include <propwidgets/combobox.h>
#include "somaconfig.h"


class NetDataWaveProperty : public PropertyPane { 
public:
  NetDataWaveProperty(pSomaConfig_t sc); 
  ~NetDataWaveProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Adjustment scaleadj_; 
  PropertyWidgets::SpinButton scaleSpin_; 
  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 

  PropertyWidgets::ComboBox<NetDataWave::SourceType> sourceTypeComboBox_; 
  pSomaConfig_t pSomaConfig_; 

};

#endif // WAVEVISPROPERTY_H
