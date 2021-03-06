#ifndef WAVEVISPROPERTY_H
#define WAVEVISPROPERTY_H

#include "propertypane.h"
#include <vis/wavevis2/wavevis2.h>
#include <propwidgets/combobox.h>
#include <propwidgets/togglebutton.h>
#include "somaconfig.h"

class WaveVisProperty : public PropertyPane { 
public:
  WaveVisProperty(pSomaConfig_t sc); 
  ~WaveVisProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

  inline std::string name() { 
    return "Wave Vis"; 
  }


private: 
  Gtk::VBox box_; 

  Gtk::Frame scaleFrame_;  
  Gtk::Label scaleLabel_; 
  Gtk::HBox scaleHBox_; 
  PropertyWidgets::ComboBox<float> scaleComboBox_;
  
  Gtk::Frame renderFrame_; 
  Gtk::VBox renderVBox_; 

  Gtk::Label renderLabel_; 
  Gtk::HBox renderHBox_; 
  PropertyWidgets::ToggleButton renderModeToggle_; 
  PropertyWidgets::ToggleButton renderMode2Toggle_; 
  PropertyWidgets::ToggleButton renderAllDataToggle_; 

  
  pSomaConfig_t pSomaConfig_; 
};

#endif // WAVEVISPROPERTY_H
