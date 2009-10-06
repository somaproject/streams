#ifndef NOISEWAVEPROPERTY_H
#define NOISEWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/noisewave.h>
#include "propwidgets/combobox.h"
#include "propwidgets/spinbutton.h"
#include "somaconfig.h"

class NoiseWaveProperty : public PropertyPane { 
public:
  NoiseWaveProperty(pSomaConfig_t sc); 
  ~NoiseWaveProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

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

  pSomaConfig_t pSomaConfig_; 
  //  PropertyWidgets::SpinButton scaleSpin_; 

};

#endif // WAVEVISPROPERTY_H
