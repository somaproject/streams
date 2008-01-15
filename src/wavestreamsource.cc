#include "wavestreamsource.h"
#include "wavestreamvis.h" 

#include <iostream>
#include <gtkmm.h>


WaveStreamSource::WaveStreamSource(datasource_t ds, datatype_t dt):
  datasource_(ds), 
  datatype_(dt), 
  lastT_(0.0)
{
  
									 
}

WaveStreamSource::~WaveStreamSource()
{
  

}

QueueView<pWaveBuffer_t>  WaveStreamSource::getQueueView()
{
  return QueueView<pWaveBuffer_t>(data_); 
  
}

bool WaveStreamSource::generateFakeData(int T = 100)
{

  return true; 
}

pStreamVis_t WaveStreamSource::newVisFactory(std::string name)
{
  
  pStreamVis_t x; 

  if (name == "wave")
    {
      x = pStreamVis_t(new WaveStreamVis(this)); 
    }
  
  if (x) {
    return x; 

  } else {
    throw std::range_error("Unknown vis type " + name); 

  }

}

void WaveStreamSource::newDataPacket(pDataPacket_t  dp)
{
  // turn a DataPacket_t into a Wave_t, 
  // which we then turn into a WaveBuffer_t

  Wave_t wave = rawToWave(dp); 
  
  pWaveBuffer_t pwb(new WaveBuffer_t); 
  pwb->time = double(wave.time) / 50e3; 
  pwb->samprate = wave.samprate; 
  pwb->data.reserve(WAVEBUF_LEN); 

  for (int i = 0; i < WAVEBUF_LEN; i++)
    {
      pwb->data.push_back(double(wave.wave[i])/1e9);
      
    }
  data_.push_back(pwb); 
  
  
  newDataSignal_.emit(); 
  
}
