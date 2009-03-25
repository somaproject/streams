#ifndef WAVEVISPROPERTY_H
#define WAVEVISPROPERTY_H

#include "propertypane.h"
#include <vis/wavevis.h>
#include <propwidgets/combobox.h>
#include "somaconfig.h"

class WaveVisProperty : public PropertyPane { 
public:
  WaveVisProperty(pSomaConfig_t sc); 
  ~WaveVisProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Frame scaleFrame_;  
  Gtk::Label scaleLabel_; 
  Gtk::HBox scaleHBox_; 
  PropertyWidgets::ComboBox<float> scaleComboBox_;
  pSomaConfig_t pSomaConfig_; 
};

#endif // WAVEVISPROPERTY_H
