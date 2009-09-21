#ifndef WAVESTREAMVISCONTROL_H
#define WAVESTREAMVISCONTROL_H

#include <gtkmm.h>

#include "wavestreamvis.h"

class WaveStreamVisControl : public Gtk::HBox
{
 public: 
  WaveStreamVisControl(WaveStreamVis* wsv); 
  ~WaveStreamVisControl(); 

 private:
  WaveStreamVis * pWaveStreamVis_; 
  Gtk::HBox hbox_; 
  Gtk::Label label_; 
  void on_size_allocate(Gtk::Allocation& allocation);
  void updateVerticalScale(float volts); 
 
};


#endif // WAVESTREAMVISCONTROL_H
