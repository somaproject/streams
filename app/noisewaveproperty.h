#ifndef NOISEWAVEPROPERTY_H
#define NOISEWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/noisewave.h>
#include "propwidgets/combobox.h"
#include "propwidgets/spinbutton.h"


class NoiseWaveProperty : public PropertyPane { 
public:
  NoiseWaveProperty(); 
  ~NoiseWaveProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Frame frame_; 
  Gtk::VBox propbox_; 
  
  Gtk::HBox amplitudeHBox_; 
  Gtk::Label amplitudeLabel_; 

  Gtk::Adjustment adjustment_; 

  PropertyWidgets::SpinButton amplitudeSpinButton_; 
  
  Gtk::HBox classHBox_; 
  Gtk::Label classLabel_; 

  PropertyWidgets::ComboBox<NoiseWave::NoiseClass> noiseClassComboBox_; 


//  PropertyWidgets::SpinButton scaleSpin_; 

};

#endif // WAVEVISPROPERTY_H
