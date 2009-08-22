#ifndef NETDATARAWWIDGET_H
#define NETADATARAWWIDGET_H

#include <gtkmm.h>
#include <sources/netdataraw.h>
#include <elementstatus.h>
#include <somaconfig.h>

class NetDataRawWidget : public ElementStatus
{
public:
  NetDataRawWidget(NetDataRaw *, pSomaConfig_t ); 
  ~NetDataRawWidget(); 
  

private:

  NetDataRaw * netDataWave_; 
  
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
  
  core::IElement * getElement() {
    return netDataWave_; 
  }

}; 



#endif // NETDATAWAVEWIDGET
