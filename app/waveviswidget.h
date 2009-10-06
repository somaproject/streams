#ifndef WAVEVISWIDGET_H
#define WAVEVISWIDGET_H

#include <gtkmm.h>
#include <vis/wavevis2/wavevis2.h>
#include "propertypanemanager.h" 
#include "elementstatus.h" 

class WaveVisWidget : public ElementStatus
{
public:
  WaveVisWidget(WaveVis2 *, pSomaConfig_t sc ); 
  ~WaveVisWidget(); 

private:
  WaveVis2 * wavevis_; 
  Gtk::Label label_; 
  Gtk::VBox vbox_; 
  
  void on_size_allocate(Gtk::Allocation & r); 
  void scaleUpdate(float scale); 
  elements::IElement * getElement() { 
    return wavevis_; 
  }
  
}; 



#endif // WAVEVISWIDGET
