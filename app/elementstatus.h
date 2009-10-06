#ifndef ELEMENTSTATUS_H
#define ELEMENTSTATUS_H
#include <gtkmm.h>
#include <elements/ielement.h>
#include "propertypanemanager.h" 
#include "somaconfig.h"

/*
base class for all element's status windows

*/

class ElementStatus : public Gtk::EventBox
{
public:
  ElementStatus(pSomaConfig_t sc); 
  ~ElementStatus(); 
  void setPropertyPaneManager(PropertyPaneManager * ppm); 
protected:
  bool selected_; 

  PropertyPaneManager * pPPM_; 
  bool on_eventbox_button_press(GdkEventButton*); 
  virtual elements::IElement * getElement() = 0; 

  void clearCallback(); 
  sigc::connection conn_; 
  pSomaConfig_t pSomaConfig_; 

}; 


#endif // ELEMENTSTATUS_H
