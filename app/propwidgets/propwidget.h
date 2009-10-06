#ifndef PROPERTY_PROPWIDGET_H
#define PROPERTY_PROPWIDGET_H

#include <elements/property.h>

namespace PropertyWidgets
{

class WidgetPropertyNotify : public elements::IPropertyNotify
{
  /* Callback class; note that notify() will be called
     OUT OF THREAD . We turn it into an in-thread signal. 

  */ 
  
public:
  inline void notify() {
    dispatcher_.emit(); 
  }

  inline Glib::Dispatcher & signal() {
    return dispatcher_; 
  }
  
  
private:
  Glib::Dispatcher dispatcher_; 

}; 

typedef boost::shared_ptr<WidgetPropertyNotify> pWidgetPropertyNotify_t; 


}

#endif
