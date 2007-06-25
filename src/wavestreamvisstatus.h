#ifndef WAVESTREAMVISSTATUS_H
#define WAVESTREAMVISSTATUS_H

#include <gtkmm.h>
#include "streams.h"
#include "wavestreamvis.h"


class WaveStreamVisStatus : public Gtk::HBox
{
 public: 
  WaveStreamVisStatus(streamVisPtr_t wsv); 
  ~WaveStreamVisStatus(); 

  // emit a selected signal
  sigc::signal<void, bool> clickedSignal() {
    return clickedSignal_; 
  }
  
  void setSelected(bool state); 

  
 private:
  waveStreamVisPtr_t pWaveStreamVis_; 
  Gtk::EventBox eventBox_; 
  Gtk::HBox hbox_; 
  Gtk::Label label_; 
  void on_size_allocate(Gtk::Allocation& allocation);
  void updateVerticalScale(float volts); 

  bool selected_; 
  sigc::signal<void, bool> clickedSignal_; 
  void toggleSelected(); 
  bool on_button_press_event(GdkEventButton* event); 
  
};


#endif // WAVESTREAMVISSTATUS_H
