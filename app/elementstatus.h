#ifndef ELEMENTSTATUS_H
#define ELEMENTSTATUS_H
#include <gtkmm.h>
#include <core/ielement.h>
#include "propertypanemanager.h" 

/*
base class for all element's status windows

*/

class ElementStatus : public Gtk::EventBox
{
public:
  ElementStatus(); 
  ~ElementStatus(); 
  void setPropertyPaneManager(PropertyPaneManager * ppm); 
protected:
  bool selected_; 

  PropertyPaneManager * pPPM_; 
  bool on_eventbox_button_press(GdkEventButton*); 
  virtual core::IElement * getElement() = 0; 

  void clearCallback(); 
  sigc::connection conn_; 

}; 


#endif // ELEMENTSTATUS_H
