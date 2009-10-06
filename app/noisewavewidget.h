#ifndef NOISEWAVEWIDGET_H
#define NOISEWAVEWIDGET_H

#include <gtkmm.h>
#include <sources/noisewave2.h>
#include "elementstatus.h"

class NoiseWaveWidget : public ElementStatus
{
public:
  NoiseWaveWidget(NoiseWave2 *, pSomaConfig_t sc ); 
  ~NoiseWaveWidget(); 


private:
  NoiseWave2 * noiseWave_; 
  Gtk::Label label_; 

  elements::IElement * getElement() {
    return noiseWave_; 
  }

}; 



#endif // NOISEWAVEWIDGET
