#include "spectvisproperty.h"
#include "propwidgets/spinbutton.h"


SpectVisProperty::SpectVisProperty(pSomaConfig_t sc) : 
  fftPropertiesFrame_("FFT Computation"), 
  fftNLabel_(" points in FFT"),
  fftNComboBox_(), 
  windowsizeLabel_("window length"), 
  windowsizeComboBox_(), 
  overlapLabel_("overlap factor"), 
  overlapComboBox_(), 
  cmapMaxLabel_("Max value"), 
  cmapMaxHScale_(0.0, 100.0, 1.0), 
  pSomaConfig_(sc)
{
  pack_start(fftPropertiesFrame_); 
  fftPropertiesFrame_.add(fftVBox_); 
  
  fftVBox_.pack_start(fftNHBox_); 
  fftVBox_.pack_start(windowsizeHBox_); 
  fftVBox_.pack_start(overlapHBox_); 
  fftVBox_.pack_start(cmapMaxHBox_); 
  

  // FFTN: 
  fftNHBox_.pack_start(fftNLabel_); 
  fftNHBox_.pack_start(fftNComboBox_); 

  PropertyWidgets::ComboBox<int>::possiblevalues_t fftn_vals; 
  fftn_vals.push_back(std::make_pair("128", 128)); 
  fftn_vals.push_back(std::make_pair("256", 256)); 
  fftn_vals.push_back(std::make_pair("512", 512)); 
  fftn_vals.push_back(std::make_pair("1024", 1024)); 
  fftNComboBox_.setPossibleValues(fftn_vals); 
  
  // windowSize: 
  windowsizeHBox_.pack_start(windowsizeLabel_); 
  windowsizeHBox_.pack_start(windowsizeComboBox_); 
  
  PropertyWidgets::ComboBox<float>::possiblevalues_t win_vals; 
  win_vals.push_back(std::make_pair("125 ms", 0.125)); 
  win_vals.push_back(std::make_pair("250 ms", 0.25)); 
  win_vals.push_back(std::make_pair("500 ms", 0.5)); 
  win_vals.push_back(std::make_pair("1 sec", 1)); 
  win_vals.push_back(std::make_pair("2 sec", 2)); 
  
  windowsizeComboBox_.setPossibleValues(win_vals); 
  
  overlapHBox_.pack_start(overlapLabel_); 
  overlapHBox_.pack_start(overlapComboBox_); 
  
  PropertyWidgets::ComboBox<int>::possiblevalues_t ol_vals; 
  ol_vals.push_back(std::make_pair("1", 1)); 
  ol_vals.push_back(std::make_pair("2", 2)); 
  ol_vals.push_back(std::make_pair("4", 4)); 
  ol_vals.push_back(std::make_pair("8", 8)); 
  
  overlapComboBox_.setPossibleValues(ol_vals); 

  cmapMaxHBox_.pack_start(cmapMaxLabel_); 
  cmapMaxHBox_.pack_start(cmapMaxHScale_); 

}


SpectVisProperty::~SpectVisProperty()
{


}

bool SpectVisProperty::addElement(elements::IElement * elt)
{
  // going to hell for this
  SpectVis * ws = dynamic_cast<SpectVis*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  fftNComboBox_.addProperty(&ws->fftN); 
  windowsizeComboBox_.addProperty(&ws->windowsize); 
  overlapComboBox_.addProperty(&ws->overlapFactor); 
  cmapMaxHScale_.addProperty(&ws->cmap_max); 
  return true; 
}

bool SpectVisProperty::delElement(elements::IElement * elt)
{
  SpectVis * ws = dynamic_cast<SpectVis*>(elt); 
  if (ws == NULL) {
    return false; 
  }
  cmapMaxHScale_.delProperty(&ws->cmap_max); 
  overlapComboBox_.delProperty(&ws->overlapFactor); 
  windowsizeComboBox_.delProperty(&ws->windowsize); 
  fftNComboBox_.delProperty(&ws->fftN); 

  return true; 
}
