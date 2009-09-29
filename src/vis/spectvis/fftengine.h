#ifndef __SPECTVIS_FFTENGINE_H__
#define __SPECTVIS_FFTENGINE_H__

namespace spectvis {

class FFTEngine 
{
  /*
    FFTEngine is responsible for keeping the FFT DB up-to-date, 
    and performs the FFT computations in a separate thread. 
    
    From the input (thread-safe) stream and putting them into the output
    database. 

    .reset() : reset and begin computing again
    
  */

public:
  void set_fftN(int); 
  void set_windowsize(int); 
  void set_overlapFactor(int); 
  
}

}
