#ifndef NETDATAWAVEPROPERTY_H
#define NETDATAWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/netdatawave/netdatawave.h>
#include <propwidgets/spinbutton.h>
#include <propwidgets/combobox.h>
#include <propwidgets/checkbox.h>
#include "somaconfig.h"


class NetDataWaveProperty : public PropertyPane { 
public:
  NetDataWaveProperty(pSomaConfig_t sc); 
  ~NetDataWaveProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

private: 
  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 
  PropertyWidgets::ComboBox<int > gainComboBox_; 
  PropertyWidgets::CheckBox hpfCheckBox_; 

  pSomaConfig_t pSomaConfig_; 

};

#endif // WAVEVISPROPERTY_H
