#include "spinbutton.h"

using namespace PropertyWidgets;

/*
  There are 

  1. normal: all connected properties have this value
  2. conflicted: some of the proerties have this value, others have different values
  3. We have recently set the property, and are waiting to hear back. 

*/
SpinButton::SpinButton(Gtk::Adjustment & adj) :
  Gtk::SpinButton(adj), 
  state_(NORMAL)
{

  signal_value_changed().connect(sigc::mem_fun(*this, 
					       &SpinButton::on_my_value_changed)); 

}

SpinButton::~SpinButton() {
  for(propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
    { 
      delProperty(*pi); 
    }

}


void SpinButton::addProperty(pProperty_t spinProperty) 
{

  if (propertySet_.find(spinProperty) == propertySet_.end()) {
    
    propertySet_.insert(spinProperty); 
    sigc::connection conn = spinProperty->signal().connect(sigc::mem_fun(*this, 
									 &SpinButton::refreshProperty)); 
    
    connMap_[spinProperty] = conn; 
    refreshProperty(0.0); 

  }
  


}


void SpinButton::delProperty(pProperty_t spinProperty)
{
  if (propertySet_.find(spinProperty) != propertySet_.end()) {
    propertySet_.erase(spinProperty); 
    connMap_[spinProperty].disconnect(); 
    connMap_.erase(spinProperty); 
  }

}

void SpinButton::refreshProperty(float value) {
  /* 
     For each property, compute current value, and update
     
     Note: This is an asymptotic hate crime. Every time we get 
     any change, we iterate through. We're banking on: 
        - The actual number of elements will be very small (~10 worst case)
	- This is a fast operation

  */
  float newvalue = 0.0; 
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


void SpinButton::setState(State st) {
  
  if (st == NORMAL) {
    get_adjustment()->set_value(value_); 
    set_sensitive(true); 
  } else if (st == PENDING) { 
    get_adjustment()->set_value(value_);     
    set_sensitive(false); 
    
  } else if (st == CONFLICTED) {
    
    set_sensitive(true); 
    set_text(""); 
  }
    state_ = st; 

}

void SpinButton::on_my_value_changed()
{
  double value = get_value(); 
  setState(PENDING); 
  for (propset_t::iterator  pi = propertySet_.begin(); 
       pi != propertySet_.end(); pi++) {
    *(*pi) = value; 
  }  
  
  std::cout << "value changed" << std::endl; 

}
