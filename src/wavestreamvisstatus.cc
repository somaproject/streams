#include "wavestreamvisstatus.h"
#include <iostream>
#include "boost/format.hpp"
using boost::format;


WaveStreamVisStatus::WaveStreamVisStatus(WaveStreamVis* wsv) :
  
  pWaveStreamVis_(wsv),
  hbox_(false, 5), 
  selected_(false)
{
  pack_start(eventBox_); 
  eventBox_.add(hbox_); 

  
  hbox_.set_size_request(100, 50); 
  hbox_.pack_start(label_); 
  
  // connect primary signal
  wsv->verticalScaleSignal().connect(sigc::mem_fun(*this, 
						   &WaveStreamVisStatus::updateVerticalScale)); 
  
  updateVerticalScale(wsv->getVerticalScale()); 

  add_events(Gdk::BUTTON_PRESS_MASK ); 
	     
  

  signal_button_press_event().connect( sigc::mem_fun(*this,
						     &WaveStreamVisStatus::on_button_press_event)); 
  show_all(); 

}

void WaveStreamVisStatus::updateVerticalScale(float volts)
{
  label_.set_text(str(format("full-scale: %d mV") % (volts * 1000))); 
}

WaveStreamVisStatus::~WaveStreamVisStatus()
{
  
}

void WaveStreamVisStatus::on_size_allocate(Gtk::Allocation& allocation)
{
  Gtk::HBox::on_size_allocate(allocation); 
  pWaveStreamVis_->setYOffset(allocation.get_y() + allocation.get_height()/2); 
  pWaveStreamVis_->setYHeight(allocation.get_height()); 


}

void WaveStreamVisStatus::setSelected(bool state)
{
  if (state == selected_) {
    // do nothing)
  } else if (state == true) {
    Gdk::Color c; 
    c.parse("gray"); 
    eventBox_.modify_bg(Gtk::STATE_NORMAL, c); 
    selected_ = true; 

  } else if (state == false) {
    Gdk::Color c; 
    c.parse("white"); 
    eventBox_.modify_bg(Gtk::STATE_NORMAL, c); 
    selected_ = false; 
  }
}

void WaveStreamVisStatus::toggleSelected()
{
  setSelected(!selected_); 
}

bool WaveStreamVisStatus::on_button_press_event(GdkEventButton* event)
{
  if (event->state & GDK_SHIFT_MASK) {
    clickedSignal_.emit(true); 
  } else {
    clickedSignal_.emit(false);
  }

  return true; 
}
