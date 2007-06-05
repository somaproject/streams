#ifndef WAVESTREAMVIS_H
#define WAVESTREAMVIS_H

#include <gtkmm.h>
#include "wave.h"

class WaveStreamVis

{
 public: 
  WaveStreamVis(); 
  ~WaveStreamVis(); 
  Gtk::Bin & getControlBin(); 
  WaveStreamRenderer * getStreamRendererPtr() { return pwr_; }; 
  void setStreamSource(StreamSource * ss); 

 private:
  Gtk::HBox hBox_; 
  Gtk::Button button_; 
  WaveStreamRenderer * pwr_; 
  StreamSource_ * streamSource_; 

};

#endif // WAVESTREAMVIS_H
