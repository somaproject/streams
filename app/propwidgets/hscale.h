#ifndef PROPERTY_HSCALE_H
#define PROPERTY_HSCALE_H

#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>
#include "propwidget.h"

namespace PropertyWidgets
{
/*
  The HScale has a minimum, a maximum, and a step value. 

  potentailly there's a render visualization




 */

class HScale : public Gtk::HScale
{

  enum State {NORMAL, PENDING, CONFLICTED}; 

public:
  HScale(double min, double max, double step); 
  ~HScale(); 
  
  typedef elements::Property<float> *  pProperty_t; 
  void addProperty(pProperty_t scaleProperty); 
  void delProperty(pProperty_t scaleProperty); 
    
private:
  typedef std::set<pProperty_t >  propset_t ; 
  typedef std::map<pProperty_t, size_t>  notifymap_t; 
  propset_t propertySet_; 
  notifymap_t notifyMap_; 
  void refreshProperty(); 

  State state_; 

  float value_; 

  void setState(State); 

  pWidgetPropertyNotify_t notify_; 

  bool on_signal_change_value(Gtk::ScrollType,double val); 
  void on_my_value_changed(); 
  
}; 


}


#endif
