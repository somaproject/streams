#include "elementstatus.h"

ElementStatus::ElementStatus(pSomaConfig_t sc) : 
  Gtk::EventBox(), 
  selected_(false), 
  pSomaConfig_(sc)
{

  add_events(Gdk::BUTTON_PRESS_MASK);
  signal_button_press_event().connect(
				      sigc::mem_fun(*this, &ElementStatus::on_eventbox_button_press) );

}


ElementStatus::~ElementStatus()
{



}

void ElementStatus::setPropertyPaneManager(PropertyPaneManager * ppm)
{
  pPPM_ = ppm; 
  pPPM_->clearSignal().connect(sigc::mem_fun(*this, 
					     &ElementStatus::clearCallback)); 
}

bool ElementStatus::on_eventbox_button_press(GdkEventButton* eb)
{
  if (pPPM_) {
    if (eb->type == GDK_2BUTTON_PRESS) { // double click, select all of this class
      if (eb->state & GDK_SHIFT_MASK) {
	std::cout << "Double clicked" << std::endl; 
      }
    } else {
      
      if (!selected_) {
	if (eb->state & GDK_SHIFT_MASK) {
	  // append to any existing structure
	  std::cout << "appending element" << std::endl; 
	  pPPM_->appendElement(getElement()); 
	} else {
	  // only this element
	  std::cout << "setting element" << std::endl; 
	  pPPM_->setElement(getElement()); 
	}
	selected_ = true; 
	set_state(Gtk::STATE_SELECTED); 
      } else {
	selected_ = false; 
	set_state(Gtk::STATE_NORMAL); 
	pPPM_->removeElement(getElement()); 
      }
    }
    
  }
  return true;
  
}

void ElementStatus::clearCallback()
{
  if (selected_) {
    selected_ = false; 
    set_state(Gtk::STATE_NORMAL); 
    pPPM_->removeElement(getElement()); 
  }
}
