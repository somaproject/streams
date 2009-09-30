#ifndef WAVEVIS_IRENDERER_H
#define WAVEVIS_IRENDERER_H

#include <data/wave.h>
#include "core/sinkpad.h"



class IRenderer
{
public: 
  virtual void renderStream(streamtime_t t1, streamtime_t t2, int pixels) =0; 
  virtual void newSample(const WaveBuffer_t & ) = 0; 
  virtual void reset() =0; 
}; 



#endif // WAVEVIS_IRENDERER_H
