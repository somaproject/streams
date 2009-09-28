#ifndef SPECTVISWIDGET_H
#define SPECTVISWIDGET_H

#include <gtkmm.h>
#include <vis/spectvis/spectvis.h>
#include "propertypanemanager.h" 
#include "elementstatus.h" 

class SpectVisWidget : public ElementStatus
{
public:
  SpectVisWidget(SpectVis *, pSomaConfig_t sc ); 
  ~SpectVisWidget(); 

private:
  SpectVis * spectvis_; 
  Gtk::Label label_; 
  Gtk::VBox vbox_; 
  
  void on_size_allocate(Gtk::Allocation & r); 
  void scaleUpdate(float scale); 
  core::IElement * getElement() { 
    return spectvis_; 
  }
  
}; 



#endif // WAVEVISWIDGET
