
#include "wavestreamvis.h"

WaveStreamVis::WaveStreamVis() :
  hBox_(), 
  button_("Test text"), 
  
{
  hBox_.pack_start(button_); 
  
}


WaveStreamVis::~WaveStreamVis()
{


}

void WaveStreamVis::setStreamSource(StreamSource * ss)
{
  streamSource_ = ss;
  pwr_ = new WaveStreamRenderer(ss); 
  

}

Gtk::Bin & WaveStreamVis::getControlBin()
{
  return (Gtk::Bin&)hBox_; 

}

