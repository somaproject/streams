#ifndef WAVEVIS_IRENDERER_H
#define WAVEVIS_IRENDERER_H

#include <data/wave.h>
#include "core/sinkpad.h"

namespace wavevis2 { 

class IRenderer
{
public: 
  virtual void renderStream(timeid_t t1, timeid_t t2, int pixels) =0; 
  virtual void newSample(pWaveBuffer_t ) = 0; 
  virtual void reset() =0; 
}; 

}

#endif // WAVEVIS_IRENDERER_H
