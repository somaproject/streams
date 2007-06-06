#ifndef WAVESTREAMSOURCE_H
#define WAVESTREAMSOURCE_H

#include <vector>
#include <sigc++/sigc++.h>
#include "queueview.h"

struct WaveBuffer_t
{
  float time; 
  float samprate; 
  std::vector<float> data; 
}; 

class WaveStreamSource
{
 public:
  // this is just a prototype source
  WaveStreamSource(); 
  ~WaveStreamSource(); 

  // emit updatedData
  
  // public data access
  std::list<WaveBuffer_t *> data_; 
  
  QueueView<WaveBuffer_t *> getQueueView(); 

  // signal
  sigc::signal<void> newDataSignal() { return newDataSignal_;  }; 
  sigc::signal<void> invalidateDataSignal() { return invalidateDataSignal_; };

  void generateFakeData(int T);   
  float lastT_; 

 private:

  sigc::signal<void> newDataSignal_; 
  sigc::signal<void> invalidateDataSignal_; 

};

#endif // WAVESTREAMSOURCE_H
