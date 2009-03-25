#ifndef PROPERTYPANEMANAGER_H
#define PROPERTYPANEMANAGER_H

#include <core/ielement.h>
#include <set>
#include "propertypane.h"
#include "somaconfig.h"

#include <gtkmm.h>

/* must be able to handle all elements, with a giant-ass
   list of overloaded methods

   we keep a set of Ielement base pointers which are 
   dispatched appropriately to visitors for creation

   Workflow we want to, well, work correctly: 

   1. add element of Type A. See PaneA
   2. add element of Type B. See nothing
   3. remove element of type B. See PaneA

   and
   
   1. add element of Type A. See PaneA
   2. add element of Type B. See nothing
   3. remove element of type A. See PaneB

*/ 

class PropertyPaneManager {

public:
  PropertyPaneManager(Gtk::Window & parent, 
		      pSomaConfig_t somaconfig); 
  ~PropertyPaneManager(); 
  void setElement(core::IElement *); 
  void appendElement(core::IElement *); 
  
  void removeElement(core::IElement *); 
  sigc::signal<void> & clearSignal() 
  {
    return signalClearAllElements_;
  }

private:
  std::set<core::IElement *> elements_; 
  bool inConflict_; 
  PropertyPane * proppane_;   
  Gtk::Window window_; 
  // we are a visitor! 
  
  PropertyPane * createPropPane(core::IElement* elt); 
  sigc::signal<void> signalClearAllElements_; 

  pSomaConfig_t pSomaConfig_; 
}; 



#endif // PROPERTYPANEMANAGER_H
