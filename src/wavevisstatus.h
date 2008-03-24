#ifndef WAVEVISSTATUS_H
#define WAVEVISSTATUS_H

#include <gtkmm.h>
#include "streams.h"
#include "streamvis.h"
#include "wavevis.h"
#include "visstatus.h"
//#include "visproperty.h"

class WaveVisStatus : public VisStatus
{
 public: 
  WaveVisStatus(pStreamVisBase_t wsv); 
  ~WaveVisStatus(); 

  // emit a selected signal
  sigc::signal<void, bool> clickedSignal() {
    return clickedSignal_; 
  }
  
  void setSelected(bool state); 
  
  pStreamVisBase_t getVisPtr() { return pWaveVis_; }; 
  
 private:
  pWaveVis_t pWaveVis_; 
  Gtk::EventBox eventBox_; 
  Gtk::VBox vbox_; 
  Gtk::Label label_; 
  
  void on_size_allocate(Gtk::Allocation& allocation);
  void updateVerticalScale(float volts); 
  void updateColor(Gdk::Color c); 
  
  bool selected_; 
  sigc::signal<void, bool> clickedSignal_; 
  void toggleSelected(); 
  bool on_button_press_event(GdkEventButton* event); 

};


#endif // WAVEVISSTATUS_H
