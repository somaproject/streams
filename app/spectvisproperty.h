#ifndef SPECTVISPROPERTY_H
#define SPECTVISPROPERTY_H

#include "propertypane.h"
#include <vis/spectvis/spectvis.h>
#include <propwidgets/combobox.h>
#include "somaconfig.h"

class SpectVisProperty : public PropertyPane { 
public:
  SpectVisProperty(pSomaConfig_t sc); 
  ~SpectVisProperty(); 
  
  virtual bool addElement(elements::IElement * elt); 
  virtual bool delElement(elements::IElement * elt); 

private: 
  Gtk::Frame fftPropertiesFrame_;  
  Gtk::VBox fftVBox_; 

  Gtk::Label fftNLabel_; 
  Gtk::HBox fftNHBox_; 
  PropertyWidgets::ComboBox<int> fftNComboBox_;

  Gtk::Label windowsizeLabel_; 
  Gtk::HBox windowsizeHBox_; 
  PropertyWidgets::ComboBox<float> windowsizeComboBox_; 

  Gtk::Label overlapLabel_; 
  Gtk::HBox overlapHBox_; 
  PropertyWidgets::ComboBox<int> overlapComboBox_; 
  
  pSomaConfig_t pSomaConfig_; 
};

#endif 
