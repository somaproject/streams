#ifndef VISFACTORY_H
#define VISFACTORY_H

#include <list>
#include <vector>
#include "filterbase.h"
#include "data/wave.h"
#include "visbase.h"
#include "vis/wavevisbase.h" 

namespace soma { 
  namespace streams { 

    class VisFactory 
    {
      // pure factory functions for generating kids
    public:
      static std::list<std::string> availableVis(FilterBase<WaveBuffer_t> *); 
      static pWaveVisBase_t createVis(FilterBase<WaveBuffer_t> *, std::string name); 
    private:
      //static wireUp(pISource_t 
    }; 
    
  } 
}

#endif // VISFACTORY_H
