#include "noisewaveproperty.h"
#include <sources/noisewave2.h>

NoiseWaveProperty::NoiseWaveProperty(pSomaConfig_t sc) :
  frame_("Fake Source"),
  amplitudeLabel_("amplitude:"), 
  adjustment_(1.0, 1.0, 102.0, 1.0, 5.0, 0.0),
  preloadLabel_("preload (min): "), 
  preloadadjustment_(0, 0, 60 * 12, 1.0, 5.0, 0.0), 
  classLabel_("signal class:"), 
  samplingRateLabel_("Sampling rate:"), 
  amplitudeSpinButton_(adjustment_), 
  preloadSpinButton_(preloadadjustment_), 
  pSomaConfig_(sc)
{

  PropertyWidgets::ComboBox<NoiseWave2::NoiseClass>::possiblevalues_t vals; 
 
  vals.push_back( std::make_pair("white noise", NoiseWave2::WhiteNoise)); 
  vals.push_back( std::make_pair("noisy sine", NoiseWave2::NoisySine)); 
  vals.push_back( std::make_pair("square wave", NoiseWave2::SquareWave)); 
  vals.push_back( std::make_pair("bimodal", NoiseWave2::BiModal)); 
  vals.push_back( std::make_pair("chirp", NoiseWave2::Chirp)); 
  vals.push_back( std::make_pair("carrier sine", NoiseWave2::CarrierSine)); 

  noiseClassComboBox_.setPossibleValues(vals); 
  
  
  PropertyWidgets::ComboBox<float>::possiblevalues_t svals; 
 
  svals.push_back( std::make_pair("250 Hz", 250.0));  
  svals.push_back( std::make_pair("1 kHz", 1000.0));
  svals.push_back( std::make_pair("2 kHz", 2000.0));
  svals.push_back( std::make_pair("8 kHz", 8000.0)); 
  svals.push_back( std::make_pair("32 kHz", 32000.0)); 

  samplingRateComboBox_.setPossibleValues(svals); 


  
  
  // GUI control
  pack_start(frame_); 
  frame_.add(propbox_); 
  propbox_.pack_start(amplitudeHBox_); 
  amplitudeHBox_.pack_start(amplitudeLabel_); 
  amplitudeHBox_.pack_start(amplitudeSpinButton_); 
  
  propbox_.pack_start(preloadHBox_); 
  preloadHBox_.pack_start(preloadLabel_); 
  preloadHBox_.pack_start(preloadSpinButton_); 


  propbox_.pack_start(classHBox_); 
  classHBox_.pack_start(classLabel_); 
  classHBox_.pack_start(noiseClassComboBox_); 

  propbox_.pack_start(samplingRateHBox_); 
  samplingRateHBox_.pack_start(samplingRateLabel_); 
  samplingRateHBox_.pack_start(samplingRateComboBox_); 


  show_all();
}

NoiseWaveProperty::~NoiseWaveProperty()
{


}

bool NoiseWaveProperty::addElement(elements::IElement * elt)
{
  // going to hell for this
  NoiseWave2 * ws = dynamic_cast<NoiseWave2*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  samplingRateComboBox_.addProperty(&ws->samplingrate); 
  noiseClassComboBox_.addProperty(&ws->noiseclass);
  amplitudeSpinButton_.addProperty(&ws->amplitude); 
  preloadSpinButton_.addProperty(&ws->preload); 
  return true; 
}

bool NoiseWaveProperty::delElement(elements::IElement * elt)
{
  NoiseWave2 * ws = dynamic_cast<NoiseWave2*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  
  noiseClassComboBox_.delProperty(&ws->noiseclass);
  amplitudeSpinButton_.delProperty(&ws->amplitude); 
  preloadSpinButton_.delProperty(&ws->preload); 
  samplingRateComboBox_.delProperty(&ws->samplingrate); 

  return true; 
}
