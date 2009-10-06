#ifndef PROPERTYPANE_H
#define PROPERTYPANE_H
#include <gtkmm.h>
#include <elements/ielement.h>

/*
   A property pane is a short-lived widget connected to a 
   "set" of elements of a particular element type. For example,
   we can speak of a "WaveVis" property pane or a 
   "NoiseSource" property pane. 

   Property panes, somewhat crucially, can handle aggregating 
   properties across a selection of elements, such that we can select
   and modify the properties of several elements at once. 

   When you instantiate a property pane, it has no associated elements. 
   You must add elements to its internal property set. 

   The property pane consists of widgets that 
   are property set aware, the "PropertySetWidgets"
   
   How do things go down when a new element is added or removed to the property
   pane? 
     1. addNewElement(elt)
     2. for each property in element elt, add it to the associated
         (appropriate) widget's property set
     3. in a sense, it's not the pane that has a property set but rather
        each widget

   The propertyPaneManager has the possibility to accept "add" and "remove"
   elemenets from all the elements. 
   
   It needs to be able to answer the question "can the current property pane
   handle the addition of this event" and if the answer is no, 
   do something else

   It's time for Dynamic Cast. 

*/

class PropertyPane : public Gtk::VBox
{
public:
  virtual bool addElement(elements::IElement * elt) = 0; 
  virtual bool delElement(elements::IElement * elt) = 0; 
private:

  PropertyPane * createPropPane(elements::IElement * elt); 
  
}; 

#endif // PROPERTYPANE_H
