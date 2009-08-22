#include "netdatarawproperty.h"
#include "propwidgets/spinbutton.h"
#include <boost/format.hpp>

NetDataRawProperty::NetDataRawProperty(pSomaConfig_t sc) : 
  scaleadj_(1.0, 100.0, 1.0, 1.0, 1.0), 
  scaleSpin_(scaleadj_), 
  pSomaConfig_(sc)
{
  pack_start(scaleSpin_);
  pack_start(sourceComboBox_);

  
}

NetDataRawProperty::~NetDataRawProperty()
{


}

bool NetDataRawProperty::addElement(core::IElement * elt)
{
  // going to hell for this
  NetDataRaw * ws = dynamic_cast<NetDataRaw*>(elt); 
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
  

  //scaleSpin_.addProperty(&ws->scale); 
  return true; 
}

bool NetDataRawProperty::delElement(core::IElement * elt)
{
  NetDataRaw * ws = dynamic_cast<NetDataRaw*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  sourceComboBox_.delProperty(&ws->src); 

  //scaleSpin_.delProperty(&ws->scale); 
  return true; 
}
