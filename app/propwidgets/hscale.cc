#include "hscale.h"

namespace PropertyWidgets { 

HScale::HScale(double min, double max, double step) :
  Gtk::HScale(min, max, step),
  state_(NORMAL), 
  notify_(new WidgetPropertyNotify)
{
  
  notify_->signal().connect(sigc::mem_fun(*this, 
					  &SpinButton::refreshProperty)); 

  

}


void HScale::refreshProperty() {
  

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

}
