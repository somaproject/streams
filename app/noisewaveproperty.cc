#include "noisewaveproperty.h"


NoiseWaveProperty::NoiseWaveProperty() :
  frame_("Fake Source"),
  amplitudeLabel_("amplitude:"), 
  adjustment_(1.0, 1.0, 102.0, 1.0, 5.0, 0.0),
  classLabel_("signal class:"), 
  amplitudeSpinButton_(adjustment_)
{

  PropertyWidgets::ComboBox<NoiseWave::NoiseClass>::possiblevalues_t vals; 
 
  vals.push_back( std::make_pair("white noise", NoiseWave::WhiteNoise)); 
  vals.push_back( std::make_pair("noisy sine", NoiseWave::NoisySine)); 
  vals.push_back( std::make_pair("square wave", NoiseWave::SquareWave)); 
  vals.push_back( std::make_pair("bimodal", NoiseWave::BiModal)); 

  noiseClassComboBox_.setPossibleValues(vals); 
  
  
  // GUI control
  pack_start(frame_); 
  frame_.add(propbox_); 
  propbox_.pack_start(amplitudeHBox_); 
  amplitudeHBox_.pack_start(amplitudeLabel_); 
  amplitudeHBox_.pack_start(amplitudeSpinButton_); 
  
  propbox_.pack_start(classHBox_); 
  classHBox_.pack_start(classLabel_); 
  classHBox_.pack_start(noiseClassComboBox_); 
  show_all();
}

NoiseWaveProperty::~NoiseWaveProperty()
{


}

bool NoiseWaveProperty::addElement(core::IElement * elt)
{
  // going to hell for this
  NoiseWave * ws = dynamic_cast<NoiseWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  noiseClassComboBox_.addProperty(&ws->noiseclass);
  amplitudeSpinButton_.addProperty(&ws->amplitude); 
  return true; 
}

bool NoiseWaveProperty::delElement(core::IElement * elt)
{
  NoiseWave * ws = dynamic_cast<NoiseWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  
  noiseClassComboBox_.delProperty(&ws->noiseclass);
  amplitudeSpinButton_.delProperty(&ws->amplitude); 

  return true; 
}
