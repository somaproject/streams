#ifndef VISPROPERTY_H
#define VISPROPERTY_H

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <set>
#include "streams.h"
#include "streamvis.h"

class VisProperty: public Gtk::Window
{

 public:
  VisProperty(std::set<streamVisPtr_t> *);
  void updateSet();

 private:
  Glib::RefPtr<Gnome::Glade::Xml> pGladeXml_;
  
  std::set<streamVisPtr_t> * pSelSet_; 
  void colorSet(); 
  Gtk::Adjustment hScaleAdj_; 
  void  hScaleAdjValueChanged(); 
};

#endif // VISPROPERTY_H
