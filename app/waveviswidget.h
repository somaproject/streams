#ifndef WAVEVISWIDGET_H
#define WAVEVISWIDGET_H

#include <gtkmm.h>
#include <vis/wavevis/wavevis.h>
#include "propertypanemanager.h" 
#include "elementstatus.h" 

class WaveVisWidget : public ElementStatus
{
public:
  WaveVisWidget(WaveVis *, pSomaConfig_t sc ); 
  ~WaveVisWidget(); 

private:
  WaveVis * wavevis_; 
  Gtk::Label label_; 
  Gtk::VBox vbox_; 
  
  void on_size_allocate(Gtk::Allocation & r); 
  void scaleUpdate(float scale); 
  core::IElement * getElement() { 
    return wavevis_; 
  }
  
}; 



#endif // WAVEVISWIDGET
