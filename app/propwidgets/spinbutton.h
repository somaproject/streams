#ifndef PROPERTY_SPINBUTTON_H
#define PROPERTY_SPINBUTTON_H

#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>
#include "propwidget.h"

namespace PropertyWidgets
{
  class SpinButton : public Gtk::SpinButton 
  {
  public:
    SpinButton(Gtk::Adjustment & adj); 
    ~SpinButton(); 
    enum State {NORMAL, PENDING, CONFLICTED}; 

  public:
    typedef elements::Property<float> *  pProperty_t; 
    void addProperty(pProperty_t spinProperty); 
    void delProperty(pProperty_t  spinProperty); 
    
  private:
    typedef std::set<pProperty_t >  propset_t ; 
    typedef std::map<pProperty_t, size_t>  notifymap_t; 
    propset_t propertySet_; 
    notifymap_t notifyMap_; 
    void refreshProperty(); 

    State state_; 
    float value_; 
    void setState(State); 
    void on_my_value_changed(); 

    pWidgetPropertyNotify_t notify_; 
    int on_my_input(double* new_value) ; 

  }; 
  
}

#endif // SPINBUTTON_H
