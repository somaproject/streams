#ifndef NETDATAWAVEWIDGET_H
#define NETADATAWAVEWIDGET_H

#include <gtkmm.h>
#include <sources/netdatawave.h>
#include <elementstatus.h>
#include <somaconfig.h>

class NetDataWaveWidget : public ElementStatus
{
public:
  NetDataWaveWidget(pNetDataWave_t, pSomaConfig_t ); 
  ~NetDataWaveWidget(); 
  

private:

  pNetDataWave_t netDataWave_; 
  
  Gtk::VBox vboxMain_; 
  Gtk::HBox hboxAnalog_; 
  Gtk::VBox vboxSource_; 

  Gtk::Label labelSource_; 
  Gtk::Label labelSourceValue_; 

  void sourceCallback(int src); 

  Gtk::VBox analogProps_; 
  
  Gtk::HBox  hboxGain_; 
  Gtk::Label labelGain_; 
  Gtk::Label labelGainValue_; 
  void gainCallback(int gain); 

  Gtk::HBox hboxHPF_; 
  Gtk::Label labelHPF_; 
  Gtk::Label labelHPFValue_; 
  void hpfCallback(bool hpf); 
  
  elements::pIElement_t getElement() {
    return netDataWave_; 
  }

}; 



#endif // NETDATAWAVEWIDGET
