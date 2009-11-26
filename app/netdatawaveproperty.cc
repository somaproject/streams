#include "netdatawaveproperty.h"
#include "propwidgets/spinbutton.h"
#include <boost/format.hpp>

NetDataWaveProperty::NetDataWaveProperty(pSomaConfig_t sc) : 
  sourceFrame_("Acq Data Source Properties"), 
  gainLabel_("gain"), 
  hpfCheckBox_("High Pass Filter Enable"),
  filterFrame_("Filter Properties"), 
  pSomaConfig_(sc)
{
  pack_start(sourceComboBox_);
  pack_start(sourceFrame_); 
  
  sourceFrame_.add(sourceVBox_); 
  gainHBox_.pack_start(gainLabel_); 
  gainHBox_.pack_start(gainComboBox_); 
  sourceVBox_.pack_start(gainHBox_);
  sourceVBox_.pack_start(hpfCheckBox_);
 

  pack_start(filterFrame_); 


  // set possible values
  
  PropertyWidgets::ComboBox<int>::possiblevalues_t vals; 
  vals.push_back(std::make_pair("0", 0)); 
  vals.push_back(std::make_pair("100", 100));
  vals.push_back(std::make_pair("200", 200));
  vals.push_back(std::make_pair("500", 500));
  vals.push_back(std::make_pair("1000", 1000));
  vals.push_back(std::make_pair("2000", 2000));
  vals.push_back(std::make_pair("5000", 5000));
  vals.push_back(std::make_pair("10000", 10000));


  gainComboBox_.setPossibleValues(vals); 

}

NetDataWaveProperty::~NetDataWaveProperty()
{


}

bool NetDataWaveProperty::addElement(elements::IElement * elt)
{
  // going to hell for this
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  PropertyWidgets::ComboBox<datasource_t>::possiblevalues_t vals; 
  std::list<datasource_t> availablesrc = ws->getAvailableSources(); 

  for(  std::list<datasource_t>::iterator i = availablesrc.begin(); 
	i != availablesrc.end(); i++) {
    std::string s = pSomaConfig_->getSourceName(*i); 
    vals.push_back( std::make_pair(s, *i)); 
  }

  sourceComboBox_.setPossibleValues(vals); 
  sourceComboBox_.addProperty(&ws->src); 


  std::cout << "Gain combo box adding property" << std::endl;
  gainComboBox_.addProperty(&ws->gain); 
  hpfCheckBox_.addProperty(&ws->hpfen);   

  return true; 
}

bool NetDataWaveProperty::delElement(elements::IElement * elt)
{
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  sourceComboBox_.delProperty(&ws->src); 
  gainComboBox_.delProperty(&ws->gain); 
  hpfCheckBox_.delProperty(&ws->hpfen);   

  return true; 
}
