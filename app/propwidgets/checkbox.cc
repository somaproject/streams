#include "checkbox.h"

using namespace PropertyWidgets;

/*
  There are 

  1. normal: all connected properties have this value
  2. conflicted: some of the proerties have this value, others have different values
  3. We have recently set the property, and are waiting to hear back. 

*/
CheckBox::CheckBox(std::string name) :
  Gtk::CheckButton(name.c_str()),
  state_(NORMAL), 
  notify_(new WidgetPropertyNotify)
{

  signal_clicked().connect(sigc::mem_fun(*this, 
					       &CheckBox::on_my_click)); 
  notify_->signal().connect(sigc::mem_fun(*this, 
					  &CheckBox::refreshProperty)); 
			    
}

CheckBox::~CheckBox() {
  for(propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
    { 
      delProperty(*pi); 
    }

}


void CheckBox::addProperty(pProperty_t checkProperty) 
{

  if (propertySet_.find(checkProperty) == propertySet_.end()) {
    
    propertySet_.insert(checkProperty); 
    size_t rethandle = checkProperty->add_watcher(notify_); 
    notifyMap_[checkProperty] = rethandle; 
    refreshProperty(); 
  }
  


}


void CheckBox::delProperty(pProperty_t checkProperty)
{
  if (propertySet_.find(checkProperty) != propertySet_.end()) {
    propertySet_.erase(checkProperty); 
    checkProperty->remove_watcher(notifyMap_[checkProperty]); 
    notifyMap_.erase(checkProperty); 
  }

}

void CheckBox::refreshProperty() {
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


void CheckBox::setState(State st) {
  
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

void CheckBox::on_my_click()
{
  bool value = get_active(); 
  setState(PENDING); 
  for (propset_t::iterator  pi = propertySet_.begin(); 
       pi != propertySet_.end(); pi++) {
    *(*pi) = value; 
  }  
  
}
