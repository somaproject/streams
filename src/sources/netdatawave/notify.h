#ifndef __NETDATAWAVE_NOTIFY_H__
#define __NETDATAWAVE_NOTIFY_H__

#include <elements/property.h>
#include <gtkmm.h>

namespace netdatawave { 

class PropertyNotify : public elements::IPropertyNotify
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

typedef boost::shared_ptr<PropertyNotify> pPropertyNotify_t; 

}
#endif
