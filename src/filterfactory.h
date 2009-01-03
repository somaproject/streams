#ifndef FILTERFACTORY_H
#define FILTERFACTORY_H

#include <list>
#include <vector>
#include "sourcebase.h"
#include "data/wave.h"
#include "filterbase.h"
#include "filters/wavefilterbase.h" 

namespace soma { 
  namespace streams { 

    class FilterFactory 
    {
      // pure factory functions for generating kids
    public:
      static std::list<std::string> availableFilters(SourceBase<WaveBuffer_t> *); 
      static pWaveFilterBase_t createFilter(SourceBase<WaveBuffer_t> *, std::string name); 
    private:
      //static wireUp(pISource_t 
    }; 
    
  } 
}

#endif // FILTERFACTORY_H
