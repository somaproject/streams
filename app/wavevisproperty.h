#ifndef WAVEVISPROPERTY_H
#define WAVEVISPROPERTY_H

#include "propertypane.h"
#include <vis/wavevis2/wavevis2.h>
#include <propwidgets/combobox.h>
#include "somaconfig.h"

class WaveVisProperty : public PropertyPane { 
public:
  WaveVisProperty(pSomaConfig_t sc); 
  ~WaveVisProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

private: 
  Gtk::Frame scaleFrame_;  
  Gtk::Label scaleLabel_; 
  Gtk::HBox scaleHBox_; 
  PropertyWidgets::ComboBox<float> scaleComboBox_;
  pSomaConfig_t pSomaConfig_; 
};

#endif // WAVEVISPROPERTY_H
