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
  state_(NORMAL),
  notify_(new WidgetPropertyNotify)
{

  signal_value_changed().connect(sigc::mem_fun(*this, 
 					       &SpinButton::on_my_value_changed)); 
  signal_input().connect(sigc::mem_fun(*this, 
				       &SpinButton::on_my_input)); 
  notify_->signal().connect(sigc::mem_fun(*this, 
					  &SpinButton::refreshProperty)); 

}

SpinButton::~SpinButton() {
  for(propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
    { 
      delProperty(*pi); 
    }

}

int SpinButton::on_my_input(double* new_value)
{
  std::cout << "on my input" << *new_value << std::endl; 
  std::cout << "The text was " << get_text() << std::endl;
  
  return Gtk::INPUT_ERROR; 

}

void SpinButton::addProperty(pProperty_t spinProperty) 
{

  if (propertySet_.find(spinProperty) == propertySet_.end()) {
    
    propertySet_.insert(spinProperty); 

    size_t rethandle = spinProperty->add_watcher(notify_); 
    notifyMap_[spinProperty] = rethandle; 
    refreshProperty(); 

  }
  


}


void SpinButton::delProperty(pProperty_t spinProperty)
{
  if (propertySet_.find(spinProperty) != propertySet_.end()) {
    propertySet_.erase(spinProperty); 

    spinProperty->remove_watcher(notifyMap_[spinProperty]); 
    notifyMap_.erase(spinProperty); 

  }

}

void SpinButton::refreshProperty() {
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
  std::cout << "SpinButtonm::refreshProperty " << newvalue << std::endl;
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
    std::cout << "setting adjustment  " << value_ 
	      << " which is now " << get_adjustment()->get_value() << std::endl;
    
    set_sensitive(true); 
  } else if (st == PENDING) {
    std::cout << "SpinButton::setState :  setting state to PENDING" 
	      << std::endl; 
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
  std::cout << "SpinButton::on_my_value_changed" << std::endl;

  double value = get_value(); 
  setState(PENDING); 
  for (propset_t::iterator  pi = propertySet_.begin(); 
       pi != propertySet_.end(); pi++) {
    *(*pi) = value; 
  }  
  
  std::cout << "value changed to " << value << std::endl; 

}
