#ifndef WAVESTREAMSOURCE_H
#define WAVESTREAMSOURCE_H

#include <vector>
#include <sigc++/sigc++.h>
#include "queueview.h"
#include "streamsource.h"

struct WaveBuffer_t
{
  float time; 
  float samprate; 
  std::vector<float> data; 
}; 

class WaveStreamSource : public StreamSource
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

  bool generateFakeData(int T);   
  float lastT_; 

  // our factory to produce our associated vis
  streamVisPtr_t newVisFactory(std::string name); 
  

 private:

  sigc::signal<void> newDataSignal_; 
  sigc::signal<void> invalidateDataSignal_; 
  sigc::connection timeoutConn_; 
};

#endif // WAVESTREAMSOURCE_H
