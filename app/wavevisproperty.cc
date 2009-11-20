#include "wavevisproperty.h"
#include "propwidgets/spinbutton.h"


WaveVisProperty::WaveVisProperty(pSomaConfig_t sc) : 
  scaleFrame_("Vertical Scale"), 
  scaleLabel_("per pixel"), 
  scaleComboBox_(), 
  renderFrame_("Rendering properties"),
  renderLabel_("Render enable"),
  renderModeToggle_("Enabled"),
  renderMode2Toggle_("Render Mode 2"),
  renderAllDataToggle_("Render All Data"),
  pSomaConfig_(sc)
{
  pack_start(box_); 
  box_.pack_start(scaleFrame_); 
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
  vals.push_back(std::make_pair("5 mV", 5e-3)); 
  vals.push_back(std::make_pair("10 mV", 10e-3)); 
  vals.push_back(std::make_pair("50 mV", 50e-3)); 
  vals.push_back(std::make_pair("100 mV", 100e-3)); 
  vals.push_back(std::make_pair("1 V", 1.0)); 

  scaleComboBox_.setPossibleValues(vals); 

  box_.pack_start(renderFrame_);
  renderFrame_.add(renderVBox_); 
  renderVBox_.pack_start(renderHBox_); 
  renderHBox_.pack_start(renderLabel_); 
  renderHBox_.pack_start(renderModeToggle_); 
  renderHBox_.pack_start(renderMode2Toggle_); 
  renderVBox_.pack_start(renderAllDataToggle_); 

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
  renderModeToggle_.addProperty(&ws->renderMode); 
  renderMode2Toggle_.addProperty(&ws->renderMode2); 
  renderAllDataToggle_.addProperty(&ws->renderAllData); 
  return true; 
}

bool WaveVisProperty::delElement(elements::IElement * elt)
{
  WaveVis2 * ws = dynamic_cast<WaveVis2*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  renderAllDataToggle_.delProperty(&ws->renderAllData); 
  renderMode2Toggle_.delProperty(&ws->renderMode2); 
  renderModeToggle_.delProperty(&ws->renderMode); 
  scaleComboBox_.delProperty(&ws->scale); 
  return true; 
}
