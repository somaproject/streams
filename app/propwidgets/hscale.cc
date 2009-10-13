#include "hscale.h"

namespace PropertyWidgets { 

HScale::HScale(double min, double max, double step) :
  Gtk::HScale(min, max, step),
  state_(NORMAL), 
  notify_(new WidgetPropertyNotify)
{
  
  notify_->signal().connect(sigc::mem_fun(*this, 
					  &HScale::refreshProperty)); 

   signal_change_value().connect(sigc::mem_fun(*this,
					      &HScale::on_signal_change_value)); 

   signal_value_changed().connect(sigc::mem_fun(*this,
						&HScale::on_my_value_changed)); 
   
   set_update_policy(Gtk::UPDATE_DISCONTINUOUS); 
}

HScale::~HScale()
{
  

}

bool HScale::on_signal_change_value(Gtk::ScrollType,double val) 
{
  //  setState(PENDING); 

  return false;
}

void HScale::addProperty(pProperty_t spinProperty) 
{

  if (propertySet_.find(spinProperty) == propertySet_.end()) {
    
    propertySet_.insert(spinProperty); 

    size_t rethandle = spinProperty->add_watcher(notify_); 
    notifyMap_[spinProperty] = rethandle; 
    refreshProperty(); 

  }
  


}


void HScale::delProperty(pProperty_t spinProperty)
{
  if (propertySet_.find(spinProperty) != propertySet_.end()) {
    propertySet_.erase(spinProperty); 

    spinProperty->remove_watcher(notifyMap_[spinProperty]); 
    notifyMap_.erase(spinProperty); 

  }

}

void HScale::on_my_value_changed()
{

  std::cout << "on_My_value_cahnged" << std::endl;

  double value = get_value();
  setState(PENDING); 

  for (propset_t::iterator  pi = propertySet_.begin(); 
       pi != propertySet_.end(); pi++) {
    *(*pi) = value; 
  }  

}

void HScale::refreshProperty() { 

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
    set_value(value_); 
    setState(NORMAL); 
  }
  
  
}
  
void HScale::setState(State st) {
  
  if (st == NORMAL) {
    set_sensitive(true); 
  } else if (st == PENDING) { 
    set_sensitive(false); 
  } else if (st == CONFLICTED) {
    set_sensitive(true); 
  }
  state_ = st; 
  
}


}
