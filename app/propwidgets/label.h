#ifndef PROPERTY_LABEL_H
#define PROPERTY_LABEL_H


#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>

#include "propwidget.h"

namespace PropertyWidgets {

/* Passive observer widget */ 



template<T> 
class Label : public Gtk::Label
{
public:
  Label() { 
    
    notify_->signal().connect(sigc::mem_fun(*this, 
					    &Label::refreshProperty)); 
    
  }
  
  ~Label() {
    for(propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
    { 
      delProperty(*pi); 
    }

  }

  // signal for formatting

  typedef sigc::signal<std::string, T> signal_t; 
  
  signal_t formatSignal() { 
    return formatsignal_; 
  }

  enum State {NORMAL, PENDING, CONFLICTED}; 
  
  typedef elements::Property<T> *  pProperty_t; 

  void addProperty(pProperty_t valProperty) { 

    if (propertySet_.find(valProperty) == propertySet_.end()) {
      
      propertySet_.insert(valProperty); 
      size_t rethandle = valProperty->add_watcher(notify_); 
      notifyMap_[valProperty] = rethandle; 
      refreshProperty(); 
    }
    
  }

  void delProperty(pProperty_t valProperty) {
    if (propertySet_.find(valProperty) != propertySet_.end()) {
      propertySet_.erase(valProperty); 
      valProperty->remove_watcher(notifyMap_[valProperty]); 
      notifyMap_.erase(valProperty); 
    }
  }
  
private:
  typedef std::set<pProperty_t >  propset_t ; 
  typedef std::map<pProperty_t, size_t>  notifymap_t; 
  propset_t propertySet_; 
  notifymap_t notifyMap_; 

  signal_t formatsignal_; 
  void refreshProperty() { 
    bool newvalue = false; 
    bool seen = false; 
    bool conflict = false; 
    for (propset_t::iterator  pi = propertySet_.begin(); 
	 pi != propertySet_.end(); pi++) {
      if (!seen) {
	newvalue = *( *pi); 
	seen = true; 
      } else {
	if (newvalue != *(*pi)) {
	  conflict = true; 
	} 
      }
    }
    
    if (conflict) {
      setState(CONFLICTED); 
    } else {
      value_ = newvalue; 
      setState(NORMAL); 
    }

  }
  
  State state_; 
  T value_; 
  void setState(State) {
    
    if (st == NORMAL) {
      set_active(value_); 
      set_sensitive(true); 
    } else if (st == PENDING) { 
      set_active(value_);     
      set_sensitive(false); 
      
    } else if (st == CONFLICTED) {
      
      set_sensitive(true); 
    }
    state_ = st; 
    
  }

  pWidgetPropertyNotify_t notify_; 

}; 



}


