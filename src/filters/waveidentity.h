#ifndef WAVEIDENTITY_H
#define WAVEIDENTITY_H

#include "filters/wavefilterbase.h"
#include "sourcebase.h" 

namespace soma { 
  namespace streams { 
    
    class WaveIdentity : public  WaveFilterBase {
    public:
      WaveIdentity(); 
      ~WaveIdentity(); 
      static const std::string NAME;

      

      void connect(SourceBase<WaveBuffer_t>  & source); 

    private:
      void newData(); 
      void invalidateData(); 
      
      QueueView<pWaveBuffer_t> sourceQueueView_; 
      
    }; 

  }
}


#endif // WAVEIDENTITY_H
