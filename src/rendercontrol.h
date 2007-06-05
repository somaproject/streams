#ifndef RENDERCONTROL_H
#define RENDERCONTROL_H

#include <gtkmm.h>
#include "wave.h"

class RenderControl

{
 public: 
  RenderControl(); 
  ~RenderControl(); 
  Gtk::Bin & getControlBin(); 
  WaveRenderer * getWaveRendererPtr() { return pwr_; }; 
 private:
  Gtk::HBox hBox_; 
  Gtk::Button button_; 
  WaveRenderer * pwr_; 

};

#endif // RENDERCONTROL_H
