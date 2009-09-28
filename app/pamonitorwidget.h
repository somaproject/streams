#ifndef __PULSEAUDIOMONITORWAVE_WIDGET_H__
#define __PULSEAUDIOMONITORWAVE_WIDGET_H__

#include <gtkmm.h>
#include <sources/pamonitor.h>
#include "elementstatus.h"

class PulseAudioMonitorWaveWidget : public ElementStatus
{
public:
  PulseAudioMonitorWaveWidget(PulseAudioMonitorWave *, pSomaConfig_t sc ); 
  ~PulseAudioMonitorWaveWidget(); 


private:
  PulseAudioMonitorWave * pamonWave_; 
  Gtk::Label label_; 

  core::IElement * getElement() {
    return pamonWave_; 
  }

}; 



#endif // NOISEWAVEWIDGET
