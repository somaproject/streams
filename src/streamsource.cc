#include "streamsource.h"


StreamSource::SteamSource()
{


}

StreamSource::~StreamSource()
{
  

}

Gtk::Bin & StreamSource::getControlBin()
{

  return (Gtk::Bin &) testBin_; 

}

void StreamSource::generateFakeData()
{

  int T = 1000; 
  int bufsize = 256; 
  
  float t  = 0.0; 
  float fs = 2000.0; 

  for (int i = 0 ; i < T; i++) 
    {
    
      WaveBuffer_t wb; 
      t = i * bufsize / T; 
      wb.time = t; 

      for (int j = 0; j < bufsize; j++)
	{
	  float x; 
	  x = (rand()/(RAND_MAX + 1)) * 2 - 1.0; 
	  wb.data.push_back(x); 

	}
      data_.push_back(wb); 
    }

  newDataSignal_.emit(); 
  
}
