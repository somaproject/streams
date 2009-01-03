#ifndef WAVEFILTERBASE_H
#define WAVEFILTERBASE_H

#include "filterbase.h"
#include "data/wave.h"

namespace soma { 
  namespace streams { 
    
    class WaveFilterBase : public FilterBase<WaveBuffer_t>
    {
    public:
      WaveFilterBase () {}; 

//       virtual void accept(IFilterVisitor & sv) { 
// 	sv.visit(this); 
//       }

    }; 
    typedef boost::shared_ptr<WaveFilterBase> pWaveFilterBase_t; 
    
  }
}




#endif // WAVEFILTERBASE_H
