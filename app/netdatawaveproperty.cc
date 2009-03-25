#include "netdatawaveproperty.h"
#include "propwidgets/spinbutton.h"
#include <boost/format.hpp>

NetDataWaveProperty::NetDataWaveProperty() : 
  scaleadj_(1.0, 100.0, 1.0, 1.0, 1.0), 
  scaleSpin_(scaleadj_)
{
  pack_start(scaleSpin_);
  pack_start(sourceComboBox_);
  pack_start(sourceTypeComboBox_); 

  PropertyWidgets::ComboBox<NetDataWave::SourceType>::possiblevalues_t vals; 
 
  vals.push_back( std::make_pair("raw source", NetDataWave::Raw)); 
  vals.push_back( std::make_pair("wave source", NetDataWave::Wave)); 
  sourceTypeComboBox_.setPossibleValues(vals); 
  
}

NetDataWaveProperty::~NetDataWaveProperty()
{


}

bool NetDataWaveProperty::addElement(core::IElement * elt)
{
  // going to hell for this
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  sourceComboBox_.addProperty(&ws->src); 

  PropertyWidgets::ComboBox<datasource_t>::possiblevalues_t vals; 
  std::list<datasource_t> availablesrc = ws->getAvailableSources(); 
  for(  std::list<datasource_t>::iterator i = availablesrc.begin(); 
	i != availablesrc.end(); i++) {
    std::string s = boost::str(boost::format("source %1%") % (unsigned int)*i); 
    vals.push_back( std::make_pair(s, *i)); 
  }
  sourceComboBox_.setPossibleValues(vals); 
  

  //scaleSpin_.addProperty(&ws->scale); 
  return true; 
}

bool NetDataWaveProperty::delElement(core::IElement * elt)
{
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  sourceComboBox_.delProperty(&ws->src); 

  //scaleSpin_.delProperty(&ws->scale); 
  return true; 
}
