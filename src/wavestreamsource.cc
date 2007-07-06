#include "wavestreamsource.h"
#include "wavestreamvis.h" 

#include <iostream>
#include <gtkmm.h>


WaveStreamSource::WaveStreamSource():
  lastT_(0.0)
{
  
  timeoutConn_ = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this,
									 &WaveStreamSource::generateFakeData), 1), 130); 

  
									 
}

WaveStreamSource::~WaveStreamSource()
{
  

}

QueueView<WaveBuffer_t *>  WaveStreamSource::getQueueView()
{
  return QueueView<WaveBuffer_t *>(data_); 
  
}

bool WaveStreamSource::generateFakeData(int T = 100)
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
	  wb->data.push_back(x/2.0 / 100.0); // -5 to 5 mV

	}
      data_.push_back(wb); 
    }
  lastT_ += float(T) * bufsize / fs; 
  newDataSignal_.emit(); 

  return true; 
}

streamVisPtr_t WaveStreamSource::newVisFactory(std::string name)
{
  
  streamVisPtr_t x; 

  if (name == "wave")
    {
      x = streamVisPtr_t(new WaveStreamVis(this)); 
    }
  
  if (x) {
    return x; 

  } else {
    throw std::range_error("Unknown vis type " + name); 

  }

}
