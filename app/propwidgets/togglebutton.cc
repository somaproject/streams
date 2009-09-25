#include "togglebutton.h"

using namespace PropertyWidgets;

/*
  There are 

  1. normal: all connected properties have this value
  2. conflicted: some of the proerties have this value, others have different values
  3. We have recently set the property, and are waiting to hear back. 

*/
ToggleButton::ToggleButton(std::string name) :
  Gtk::ToggleButton(name.c_str()),
  state_(NORMAL)
{

  signal_clicked().connect(sigc::mem_fun(*this, 
					       &ToggleButton::on_my_click)); 

}

ToggleButton::~ToggleButton() {
  for(propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
    { 
      delProperty(*pi); 
    }

}


void ToggleButton::addProperty(pProperty_t toggleProperty) 
{

  if (propertySet_.find(toggleProperty) == propertySet_.end()) {
    
    propertySet_.insert(toggleProperty); 
    sigc::connection conn = toggleProperty->signal().connect(sigc::mem_fun(*this, 
									 &ToggleButton::refreshProperty)); 
    
    connMap_[toggleProperty] = conn; 
    refreshProperty(0.0); 

  }
  


}


void ToggleButton::delProperty(pProperty_t toggleProperty)
{
  if (propertySet_.find(toggleProperty) != propertySet_.end()) {
    propertySet_.erase(toggleProperty); 
    connMap_[toggleProperty].disconnect(); 
    connMap_.erase(toggleProperty); 
  }

}

void ToggleButton::refreshProperty(bool value) {
  /* 
     For each property, compute current value, and update
     
     Note: This is an asymptotic hate crime. Every time we get 
     any change, we iterate through. We're banking on: 
        - The actual number of elements will be very small (~10 worst case)
	- This is a fast operation

  */
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


void ToggleButton::setState(State st) {
  
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

void ToggleButton::on_my_click()
{
  bool value = get_active(); 
  setState(PENDING); 
  for (propset_t::iterator  pi = propertySet_.begin(); 
       pi != propertySet_.end(); pi++) {
    *(*pi) = value; 
  }  
  
}
