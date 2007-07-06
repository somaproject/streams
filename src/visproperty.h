#ifndef VISPROPERTY_H
#define VISPROPERTY_H

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <set>
#include "streams.h"
#include "streamvis.h"
#include "wavestreamrenderer.h"

class VisProperty: public Gtk::Window
{

 public:
  VisProperty(std::set<streamVisPtr_t> *);
  void updateSet();
  void updateProperty(streamVisPtr_t p, bool force); 

 private:
  Glib::RefPtr<Gnome::Glade::Xml> pGladeXml_;
  
  std::set<streamVisPtr_t> * pSelSet_; 
  void colorSet(); 
  Gtk::Adjustment hScaleAdj_; 
  void  hScaleAdjValueChanged(); 
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(waveColor_); add(colorName_); }

    Gtk::TreeModelColumn<WaveColor> waveColor_;
    Gtk::TreeModelColumn<Glib::ustring> colorName_;
  };

  ModelColumns colorColumns_; 
  Glib::RefPtr<Gtk::ListStore> pColorTreeModel_;
  
  Gtk::Adjustment ampScaleAdj_; 
  
  
};

#endif // VISPROPERTY_H
