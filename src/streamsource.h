#ifndef STREAMSOURCE_H
#define STREAMSOURCE_H

#include <gtkmm.h>
#include <vector>

struct WaveBuffer_t
{
  float time; 
  std::vector<float> data; 
}; 


class StreamSource
{
 public:
  // this is just a prototype source
  StreamSource(); 
  ~StreamSource(); 

  // generic stream source interfaces
  Gtk::Bin & getControlBin(); 
  // emit updatedData
  
  // public data access
  std::list<WaveBuffer_t> data_; 
  void generateFakeData(); 
  // signal
  sigc::signal<void> newDataSignal() { return newDataSignal_;  }; 
  
  
 private:
  Gtk::Button testBin_; 
  sigc::signal<void> newDataSignal_; 

};

#endif // STREAMSOURCE_H
