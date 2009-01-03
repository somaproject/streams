#ifndef WAVEVISPROPERTY_H
#define WAVEVISPROPERTY_H

#include "propertypane.h"
#include <vis/wavevis.h>
#include <propwidgets/combobox.h>

class WaveVisProperty : public PropertyPane { 
public:
  WaveVisProperty(); 
  ~WaveVisProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Frame scaleFrame_;  
  Gtk::Label scaleLabel_; 
  Gtk::HBox scaleHBox_; 
  PropertyWidgets::ComboBox<float> scaleComboBox_;

};

#endif // WAVEVISPROPERTY_H
