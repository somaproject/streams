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
  inline std::string name() { 
    return "Network Data Wave Source"; 
  }

private: 

  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 

  Gtk::Frame sourceFrame_; 
  Gtk::VBox sourceVBox_; 
  Gtk::HBox gainHBox_; 
  Gtk::Label gainLabel_; 
  PropertyWidgets::ComboBox<int > gainComboBox_; 
  PropertyWidgets::CheckBox hpfCheckBox_; 
  
  Gtk::Frame filterFrame_; 
  
  pSomaConfig_t pSomaConfig_; 

};

#endif // WAVEVISPROPERTY_H
