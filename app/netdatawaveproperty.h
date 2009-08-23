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
  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 
  PropertyWidgets::ComboBox<int> gainComboBox_; 

  pSomaConfig_t pSomaConfig_; 

};

#endif // WAVEVISPROPERTY_H
