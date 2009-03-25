#include "noisewavewidget.h"

NoiseWaveWidget::NoiseWaveWidget(NoiseWave * nw, pSomaConfig_t sc) :
  ElementStatus(sc),
  noiseWave_(nw)
{
  add(label_); 
  label_.set_text("Noise Wave Hello World"); 
  show_all(); 
  
}


NoiseWaveWidget::~NoiseWaveWidget()
{
  
  
}
