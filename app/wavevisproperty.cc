#include "wavevisproperty.h"
#include "propwidgets/spinbutton.h"


WaveVisProperty::WaveVisProperty(pSomaConfig_t sc) : 
  scaleFrame_("Vertical Scale"), 
  scaleLabel_("per pixel"), 
  scaleComboBox_(), 
  pSomaConfig_(sc)
{
  pack_start(scaleFrame_); 
  scaleFrame_.add(scaleHBox_); 
  scaleHBox_.pack_start(scaleComboBox_);
  scaleHBox_.pack_start(scaleLabel_); 
  PropertyWidgets::ComboBox<float>::possiblevalues_t vals; 
  vals.push_back(std::make_pair("1 uV", 1e-6)); 
  vals.push_back(std::make_pair("2 uV", 2e-6)); 
  vals.push_back(std::make_pair("5 uV", 5e-6)); 
  vals.push_back(std::make_pair("10 uV", 10e-6)); 
  vals.push_back(std::make_pair("20 uV", 20e-6)); 
  vals.push_back(std::make_pair("50 uV", 50e-6)); 
  vals.push_back(std::make_pair("100 uV", 100e-6)); 
  vals.push_back(std::make_pair("1 mV", 1e-3)); 
  vals.push_back(std::make_pair("10 mV", 10e-3)); 
  vals.push_back(std::make_pair("100 mV", 100e-3)); 
  vals.push_back(std::make_pair("1 V", 1.0)); 

  scaleComboBox_.setPossibleValues(vals); 
}

WaveVisProperty::~WaveVisProperty()
{


}

bool WaveVisProperty::addElement(elements::IElement * elt)
{
  // going to hell for this
  WaveVis2 * ws = dynamic_cast<WaveVis2*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  
  scaleComboBox_.addProperty(&ws->scale); 
  return true; 
}

bool WaveVisProperty::delElement(elements::IElement * elt)
{
  WaveVis2 * ws = dynamic_cast<WaveVis2*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  scaleComboBox_.delProperty(&ws->scale); 
  return true; 
}
