#ifndef NETDATARAWPROPERTY_H
#define NETDATARAWPROPERTY_H

#include "propertypane.h"
#include <sources/netdataraw.h>
#include <propwidgets/spinbutton.h>
#include <propwidgets/combobox.h>
#include "somaconfig.h"


class NetDataRawProperty : public PropertyPane { 
public:
  NetDataRawProperty(pSomaConfig_t sc); 
  ~NetDataRawProperty(); 
  
  virtual bool addElement(core::IElement * elt); 
  virtual bool delElement(core::IElement * elt); 

private: 
  Gtk::Adjustment scaleadj_; 
  PropertyWidgets::SpinButton scaleSpin_; 
  PropertyWidgets::ComboBox<datasource_t> sourceComboBox_; 

  pSomaConfig_t pSomaConfig_; 

};

#endif // WAVEVISPROPERTY_H
