#ifndef WAVESTREAMVISSTATUS_H
#define WAVESTREAMVISSTATUS_H

#include <gtkmm.h>
#include "streams.h"
#include "wavestreamvis.h"
#include "visstatus.h"
#include "visproperty.h"

class WaveStreamVisStatus : public VisStatus
{
 public: 
  WaveStreamVisStatus(streamVisPtr_t wsv); 
  ~WaveStreamVisStatus(); 

  // emit a selected signal
  sigc::signal<void, bool> clickedSignal() {
    return clickedSignal_; 
  }
  
  void setSelected(bool state); 
  
  streamVisPtr_t getVisPtr() { return pWaveStreamVis_; }; 
  
 private:
  waveStreamVisPtr_t pWaveStreamVis_; 
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


#endif // WAVESTREAMVISSTATUS_H
