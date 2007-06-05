
#include "rendercontrol.h"

RenderControl::RenderControl() :
  hBox_(), 
  button_("Test text"), 
  pwr_(new WaveRenderer())

{
  hBox_.pack_start(button_); 
  
}


RenderControl::~RenderControl()
{


}

Gtk::Bin & RenderControl::getControlBin()
{
  return (Gtk::Bin&)hBox_; 

}

