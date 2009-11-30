#include "netdatawaveproperty.h"
#include "propwidgets/spinbutton.h"
#include <boost/format.hpp>

NetDataWaveProperty::NetDataWaveProperty(pSomaConfig_t sc) : 
  sourceFrame_("Acq Data Source Properties"), 
  gainLabel_("gain"), 
  hpfCheckBox_("High Pass Filter Enable"),
  filterFrame_("Filter Properties"), 
  pSomaConfig_(sc),
  channel1_("Chan 1"),
  channel2_("Chan 2"), 
  channel3_("chan 3"), 
  channel4_("chan 4")
{
  pack_start(sourceComboBox_);
  pack_start(sourceFrame_); 
  
  sourceFrame_.add(sourceVBox_); 
  gainHBox_.pack_start(gainLabel_); 
  gainHBox_.pack_start(gainComboBox_); 
  sourceVBox_.pack_start(gainHBox_);
  sourceVBox_.pack_start(hpfCheckBox_);
  sourceVBox_.pack_start(chanHBox_); 
  
  chanHBox_.pack_start(channel1_); 
  chanHBox_.pack_start(channel2_); 
  chanHBox_.pack_start(channel3_); 
  chanHBox_.pack_start(channel4_); 

  channelRadioGroup_.addRadioButton(0, &channel1_); 
  channelRadioGroup_.addRadioButton(1, &channel2_); 
  channelRadioGroup_.addRadioButton(2, &channel3_); 
  channelRadioGroup_.addRadioButton(3, &channel4_); 


  pack_start(filterFrame_); 


  // set possible values
  
  PropertyWidgets::ComboBox<int>::possiblevalues_t vals; 
  vals.push_back(std::make_pair("0", 0)); 
  vals.push_back(std::make_pair("100", 100));
  vals.push_back(std::make_pair("200", 200));
  vals.push_back(std::make_pair("500", 500));
  vals.push_back(std::make_pair("1000", 1000));
  vals.push_back(std::make_pair("2000", 2000));
  vals.push_back(std::make_pair("5000", 5000));
  vals.push_back(std::make_pair("10000", 10000));


  gainComboBox_.setPossibleValues(vals); 

}

NetDataWaveProperty::~NetDataWaveProperty()
{


}

bool NetDataWaveProperty::addElement(elements::IElement * elt)
{
  // going to hell for this
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  PropertyWidgets::ComboBox<datasource_t>::possiblevalues_t vals; 
  std::list<datasource_t> availablesrc = ws->getAvailableSources(); 

  for(  std::list<datasource_t>::iterator i = availablesrc.begin(); 
	i != availablesrc.end(); i++) {
    std::string s = pSomaConfig_->getSourceName(*i); 
    vals.push_back( std::make_pair(s, *i)); 
  }

  sourceComboBox_.setPossibleValues(vals); 
  sourceComboBox_.addProperty(&ws->src); 

  channelRadioGroup_.addProperty(&ws->selchan); 


  gainComboBox_.addProperty(&ws->gain); 
  hpfCheckBox_.addProperty(&ws->hpfen);   

  return true; 
}

bool NetDataWaveProperty::delElement(elements::IElement * elt)
{
  NetDataWave * ws = dynamic_cast<NetDataWave*>(elt); 
  if (ws == NULL) {
    return false; 
  }

  hpfCheckBox_.delProperty(&ws->hpfen);   
  gainComboBox_.delProperty(&ws->gain); 
  channelRadioGroup_.delProperty(&ws->selchan); 
  sourceComboBox_.delProperty(&ws->src); 

  return true; 
}
