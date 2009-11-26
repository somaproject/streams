#ifndef NOISEWAVEPROPERTY_H
#define NOISEWAVEPROPERTY_H

#include "propertypane.h"
#include <sources/noisewave2.h>
#include "propwidgets/combobox.h"
#include "propwidgets/spinbutton.h"
#include "somaconfig.h"

class NoiseWaveProperty : public PropertyPane { 
public:
  NoiseWaveProperty(pSomaConfig_t sc); 
  ~NoiseWaveProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

  inline std::string name() { 
    return "Noise Wave Source"; 
  }
private: 
  Gtk::Frame frame_; 
  Gtk::VBox propbox_; 
  
  Gtk::HBox amplitudeHBox_; 
  Gtk::Label amplitudeLabel_; 

  Gtk::Adjustment adjustment_; 

  Gtk::HBox preloadHBox_; 
  Gtk::Label preloadLabel_; 

  Gtk::HBox samplingRateHBox_; 
  Gtk::Label samplingRateLabel_; 


  Gtk::Adjustment preloadadjustment_; 

  PropertyWidgets::SpinButton amplitudeSpinButton_; 
  PropertyWidgets::SpinButton preloadSpinButton_; 
  
  Gtk::HBox classHBox_; 
  Gtk::Label classLabel_; 

  PropertyWidgets::ComboBox<NoiseWave2::NoiseClass> noiseClassComboBox_; 

  PropertyWidgets::ComboBox<float> samplingRateComboBox_; 

  pSomaConfig_t pSomaConfig_; 
  //  PropertyWidgets::SpinButton scaleSpin_; 

};

#endif // WAVEVISPROPERTY_H
