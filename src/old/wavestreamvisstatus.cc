#include "wavestreamvisstatus.h"
#include <iostream>
#include "boost/format.hpp"

using boost::format;


WaveVisStatus::WaveVisStatus(pStreamVis_t wsv) :
  vbox_(false, 5), 
  selected_(false)
{

  // dynamic downcast
  pWaveVis_ = boost::dynamic_pointer_cast<WaveVis>(wsv); 
  

  pack_start(eventBox_); 
  eventBox_.add(vbox_); 

  
  vbox_.set_size_request(100, 50); 
  vbox_.pack_start(label_); 
  
  // now the necessary downcast
  // connect primary signal
  pWaveVis_->verticalScaleSignal().connect(sigc::mem_fun(*this, 
						   &WaveVisStatus::updateVerticalScale)); 
  
  updateVerticalScale(pWaveVis_->getVerticalScale()); 
  
  pWaveVis_->colorSignal().connect(sigc::mem_fun(*this, 
						   &WaveVisStatus::updateColor)); 
  
  updateColor(pWaveVis_->getColor()); 
  
  add_events(Gdk::BUTTON_PRESS_MASK ); 

  signal_button_press_event().connect( sigc::mem_fun(*this,
						     &WaveVisStatus::on_button_press_event)); 
  show_all(); 

}

void WaveVisStatus::updateVerticalScale(float volts)
{
  label_.set_text(std::string("full-scale : ") + voltsToString(volts)); 


}

WaveVisStatus::~WaveVisStatus()
{
  
}

void WaveVisStatus::on_size_allocate(Gtk::Allocation& allocation)
{
  Gtk::HBox::on_size_allocate(allocation); 

  pWaveVis_->setYOffset(allocation.get_y() + allocation.get_height()/2); 
  pWaveVis_->setYHeight(allocation.get_height()); 

}
void WaveVisStatus::setSelected(bool state)
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

void WaveVisStatus::toggleSelected()
{
  setSelected(!selected_); 
}

bool WaveVisStatus::on_button_press_event(GdkEventButton* event)
{
  if (event->state & GDK_SHIFT_MASK) {
    clickedSignal_.emit(true); 
  } else {
    clickedSignal_.emit(false);
  }

  return true; 
}

void WaveVisStatus::updateColor(Gdk::Color c)
{
  
}
