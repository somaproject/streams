#include "netdatawaveproperty.h"
#include "propwidgets/spinbutton.h"


NetDataWaveProperty::NetDataWaveProperty() : 
  scaleadj_(1.0, 100.0, 1.0, 1.0, 1.0), 
  scaleSpin_(scaleadj_)
{
  pack_start(scaleSpin_);
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
  
  //scaleSpin_.addProperty(&ws->scale); 
  return true; 
}

bool NetDataWaveProperty::delElement(core::IElement * elt)
{
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  //scaleSpin_.delProperty(&ws->scale); 
  return true; 
}
