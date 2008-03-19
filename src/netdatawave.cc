#include "netdatawave.h"
#include <iostream>
#include <gtkmm.h>


NetDataWave::NetDataWave(datasource_t ds, datatype_t dt):
  datasource_(ds), 
  datatype_(dt)
{
  
									 
}

NetDataWave::~NetDataWave()
{
  

}

void NetDataWave::newDataPacket(pDataPacket_t  dp)
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
