#include "wavestreamsource.h"
#include <iostream>

WaveStreamSource::WaveStreamSource():
  lastT_(0.0)
{


}

WaveStreamSource::~WaveStreamSource()
{
  

}

QueueView<WaveBuffer_t *>  WaveStreamSource::getQueueView()
{
  return QueueView<WaveBuffer_t *>(data_); 
  
}

void WaveStreamSource::generateFakeData(int T = 100)
{

  int bufsize = 256; 
  
  float t  = 0.0; 
  float fs = 2000.0; 

  for (int i = 0 ; i < T; i++) 
    {
    
      WaveBuffer_t * wb = new WaveBuffer_t; 
      t = lastT_ + i * bufsize / fs; 
      wb->time = t; 
      wb->samprate = fs; 
      for (int j = 0; j < bufsize; j++)
	{
	  float x; 
	  x = (float)rand() /(RAND_MAX + 1.0) * 2.0 - 1.0; 
	  wb->data.push_back(x*40); 

	}
      data_.push_back(wb); 
    }
  lastT_ += float(T) * bufsize / fs; 
  newDataSignal_.emit(); 
  
}
