#ifndef PROPERTY_CHECKBOX_H
#define PROPERTY_CHECKBOX_H

#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>

#include "propwidget.h"

namespace PropertyWidgets
{
  class CheckBox : public Gtk::CheckButton
  {
  public:
    CheckBox(std::string text); 
    ~CheckBox(); 
    enum State {NORMAL, PENDING, CONFLICTED}; 

  public:
    typedef elements::Property<bool> *  pProperty_t; 
    void addProperty(pProperty_t checkProperty); 
    void delProperty(pProperty_t checkProperty); 
    
  private:
    typedef std::set<pProperty_t >  propset_t ; 
    typedef std::map<pProperty_t, size_t>  notifymap_t; 
    propset_t propertySet_; 
    notifymap_t notifyMap_; 
    void refreshProperty(); 

    State state_; 
    bool value_; 
    void setState(State); 
    void on_my_click(); 

    pWidgetPropertyNotify_t notify_; 

  }; 
  
}

#endif // CHECKBUTTON_H
