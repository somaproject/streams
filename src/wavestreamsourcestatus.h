#ifndef WAVESTREAMSOURCESTATUS_H
#define WAVESTREAMSOURCESTATUS_H

#include <gtkmm.h>
#include "streams.h"
#include "sourcestatus.h"

#include "wavestreamsource.h"

class WaveStreamSourceStatus : public SourceStatus
{
 public: 
  WaveStreamSourceStatus(streamSourcePtr_t wsv); 
  ~WaveStreamSourceStatus(); 

  // emit a selected signal
/*   sigc::signal<void, bool> clickedSignal() { */
/*     return clickedSignal_;  */
/*   } */
  
/*   void setSelected(bool state);  */
  streamSourcePtr_t getSourcePtr() { return pStreamSource_; }; 
  
 private:
/*   WaveStreamVis * pWaveStreamVis_;  */
/*   Gtk::EventBox eventBox_;  */
  Gtk::HBox hbox_;  
  Gtk::Label label_; 
/*   void on_size_allocate(Gtk::Allocation& allocation); */
/*   void updateVerticalScale(float volts);  */

/*   bool selected_;  */
/*   sigc::signal<void, bool> clickedSignal_;  */
/*   void toggleSelected();  */
/*   bool on_button_press_event(GdkEventButton* event);  */
  streamSourcePtr_t pStreamSource_; 

};


#endif // WAVESTREAMSOURCESTATUS_H
