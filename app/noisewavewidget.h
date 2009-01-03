#ifndef NOISEWAVEWIDGET_H
#define NOISEWAVEWIDGET_H

#include <gtkmm.h>
#include <sources/noisewave.h>
#include "elementstatus.h"

class NoiseWaveWidget : public ElementStatus
{
public:
  NoiseWaveWidget(NoiseWave * ); 
  ~NoiseWaveWidget(); 


private:
  NoiseWave * noiseWave_; 
  Gtk::Label label_; 

  core::IElement * getElement() {
    return noiseWave_; 
  }

}; 



#endif // NOISEWAVEWIDGET
