#include "pamonitorwidget.h"

PulseAudioMonitorWaveWidget::PulseAudioMonitorWaveWidget(PulseAudioMonitorWave * pa, pSomaConfig_t sc) :
  ElementStatus(sc),
  pamonWave_(pa)
{
  add(label_); 
  label_.set_text("PulseAudioMonitor widget"); 
  show_all(); 
  
}


PulseAudioMonitorWaveWidget::~PulseAudioMonitorWaveWidget()
{
  
  
}
