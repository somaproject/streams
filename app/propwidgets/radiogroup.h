#ifndef PROPERTY_RADIOGROUP_H
#define PROPERTY_RADIOGROUP_H

#include <boost/foreach.hpp>
#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>
#include "propwidget.h"

#include <map>

namespace PropertyWidgets
{

template<class valuetype_t>
class RadioGroupManager
{
public:
  
  enum State {NORMAL, PENDING, CONFLICTED}; 
  Gtk::RadioButton::Group group_; 
  
  valuetype_t value_; 
  
  RadioGroupManager() :
    notify_(new WidgetPropertyNotify)

  {
    

  }
  
  typedef elements::Property<valuetype_t> *  pProperty_t; 
  
  void addRadioButton(const valuetype_t v, Gtk::RadioButton * rb )
  {
    if(rbmap_.find(v) != rbmap_.end()) {
      throw std::runtime_error("Value already in map"); 
    }
    
    // connect to the signal
    // create the map
    if(rbmap_.empty()) {
      group_ = rb->get_group(); 
    } else { 
      rb->set_group(group_);       
    }
    
    
    rbmap_.insert(std::make_pair(v, rb)); 
    rb->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, 
					       &RadioGroupManager::on_button_toggled), v)); 
   
    notify_->signal().connect(sigc::mem_fun(*this, 
 					    &RadioGroupManager::refreshProperty)); 
 
  }
  

  void addProperty(pProperty_t radioProperty) 
  {
    
    if (propertySet_.find(radioProperty) == propertySet_.end()) {
      
      propertySet_.insert(radioProperty); 
      
      size_t rethandle = radioProperty->add_watcher(notify_); 
      notifyMap_[radioProperty] = rethandle; 
      
      refreshProperty(); 
    }
    
    
    
  }
  
  void delProperty(pProperty_t radioProperty)
  {
    if (propertySet_.find(radioProperty) != propertySet_.end()) {
      propertySet_.erase(radioProperty); 
      
    radioProperty->remove_watcher(notifyMap_[radioProperty]); 
    notifyMap_.erase(radioProperty); 
    
    }
    
  }
  

private:
  typedef std::map<valuetype_t, Gtk::RadioButton *> rbmap_t; 
  rbmap_t rbmap_; 

  typedef std::set<pProperty_t >  propset_t ; 
  typedef std::map<pProperty_t, size_t>  notifymap_t; 
  propset_t propertySet_; 
  notifymap_t notifyMap_; 
  
  pWidgetPropertyNotify_t notify_; 
  bool inSetState_; 

  State state_; 

  void refreshProperty() {
    /* 
       For each property, compute current value, and update
    
       Note: This is an asymptotic hate crime. Every time we get 
       any change, we iterate through. We're banking on: 
       - The actual number of elements will be very small (~10 worst case)
       - This is a fast operation
    
    */
    std::cout << "RadioGroup:: refreshProperty" << std::endl; 
    valuetype_t newvalue; 
    bool seen = false; 
    bool conflict = false; 
    for (typename propset_t::iterator  pi = propertySet_.begin(); 
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
  
  void setState(State st) {
    typedef typename rbmap_t::value_type t;
      
    inSetState_ = true; 
    std::string statestring; 
    if (st == NORMAL) statestring = "NORMAL"; 
    if (st == PENDING) statestring = "PENDING"; 
    if (st == CONFLICTED) statestring = "CONFLICTED"; 
    std::cout << "radio group setstate = " << statestring
	      << " value = " << value_ << std::endl; 

    if (st == NORMAL) {
      BOOST_FOREACH(t v, rbmap_) {
	if(v.first == value_) {
	  v.second->set_active(true); 
	} else {
	  v.second->set_active(false); 
	}
	v.second->set_sensitive(true); 
	v.second->set_inconsistent(false); 
      }

      //set_sensitive(true); 
    } else if (st == PENDING) { 
      BOOST_FOREACH(t v, rbmap_) {
	v.second->set_sensitive(false); 
      }


    } else if (st == CONFLICTED) {
      BOOST_FOREACH(t v, rbmap_) {
	v.second->set_sensitive(true); 
	v.second->set_inconsistent(true); 
      }
    }
    state_ = st; 

    inSetState_ = false; 
  }

  void on_button_toggled(valuetype_t v) {
    if (inSetState_)
      return; 
    // called when any button changes
    std::cout << "button was toggled" << std::endl; 
    // is this a set-to
    if(rbmap_[v]->get_active()) {
      std::cout << "the value is " << v << std::endl;
      // this is the one we just made active
      BOOST_FOREACH(pProperty_t p, propertySet_) { 
	if (*p != v) { 
	  *p = v; 
	}
      }
      setState(PENDING); 
      
    }

  }
  
};



}

#endif 
