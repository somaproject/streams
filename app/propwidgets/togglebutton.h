#ifndef PROPERTY_TOGGLEBUTTON_H
#define PROPERTY_TOGGLEBUTTON_H

#include <gtkmm.h>
#include <properties.h>
#include <set>
#include <list>

namespace PropertyWidgets
{
  class ToggleButton : public Gtk::ToggleButton 
  {
  public:
    ToggleButton(std::string text); 
    ~ToggleButton(); 
    enum State {NORMAL, PENDING, CONFLICTED}; 

  public:
    typedef Property<bool> *  pProperty_t; 
    void addProperty(pProperty_t toggleProperty); 
    void delProperty(pProperty_t  toggleProperty); 
    
  private:
    typedef std::set<pProperty_t >  propset_t ; 
    typedef std::map<pProperty_t, sigc::connection>  sigconnmap_t; 
    propset_t propertySet_; 
    sigconnmap_t connMap_; 
    void refreshProperty(bool value); 

    State state_; 
    bool value_; 
    void setState(State); 
    void on_my_click(); 

  }; 
  
}

#endif // TOGGLEBUTTON_H
