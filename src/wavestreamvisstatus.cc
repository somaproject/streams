#include "wavestreamvisstatus.h"
#include <iostream>
#include "boost/format.hpp"

using boost::format;


WaveStreamVisStatus::WaveStreamVisStatus(pStreamVis_t wsv) :
  vbox_(false, 5), 
  selected_(false)
{

  // dynamic downcast
  pWaveStreamVis_ = boost::dynamic_pointer_cast<WaveStreamVis>(wsv); 
  


  pack_start(eventBox_); 
  eventBox_.add(vbox_); 

  
  vbox_.set_size_request(100, 50); 
  vbox_.pack_start(label_); 
  
  // now the necessary downcast
  // connect primary signal
  pWaveStreamVis_->verticalScaleSignal().connect(sigc::mem_fun(*this, 
						   &WaveStreamVisStatus::updateVerticalScale)); 
  
  updateVerticalScale(pWaveStreamVis_->getVerticalScale()); 
  
  pWaveStreamVis_->colorSignal().connect(sigc::mem_fun(*this, 
						   &WaveStreamVisStatus::updateColor)); 
  
  updateColor(pWaveStreamVis_->getColor()); 
  


  add_events(Gdk::BUTTON_PRESS_MASK ); 
	     
  

  signal_button_press_event().connect( sigc::mem_fun(*this,
						     &WaveStreamVisStatus::on_button_press_event)); 
  show_all(); 

}

void WaveStreamVisStatus::updateVerticalScale(float volts)
{
  label_.set_text(std::string("full-scale : ") + voltsToString(volts)); 


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

void WaveStreamVisStatus::updateColor(Gdk::Color c)
{
  
}
