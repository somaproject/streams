#ifndef WAVESTREAMSOURCE_H
#define WAVESTREAMSOURCE_H

#include <vector>
#include <sigc++/sigc++.h>
#include <somanetwork/wave.h>
#include <boost/shared_ptr.hpp>

#include "queueview.h"
#include "streamsource.h"


struct WaveBuffer_t
{
  float time; 
  float samprate; 
  std::vector<float> data; 
}; 

typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 

class Wave : public StreamSource
{
 public:
  // this is just a prototype source
  Wave(datasource_t ds, datatype_t dt);

  ~Wave(); 

  // emit updatedData
  
  void newDataPacket(pDataPacket_t dp); 
  
  // public data access
  
  QueueView<pWaveBuffer_t> getQueueView(); 

  // signal
  sigc::signal<void> newDataSignal() { return newDataSignal_;  }; 
  sigc::signal<void> invalidateDataSignal() { return invalidateDataSignal_; };

  float lastT_; 

  // our factory to produce our associated vis
  pStreamVis_t newVisFactory(std::string name); 
  
 private:
  datasource_t datasource_; 
  datatype_t datatype_; 
  sigc::signal<void> newDataSignal_; 
  sigc::signal<void> invalidateDataSignal_; 

  std::list<pWaveBuffer_t> data_; 

};

#endif // WAVESTREAMSOURCE_H
