#ifndef ITRIGGERRENDERER_H
#define ITRIGGERRENDERER_H
#include "streamtime.h"

/*
  Interface for all vis elements that support rendering to the main 
  stream window. 

*/

typedef sigc::signal<void> invTriggerRenderSignal_t; 

class ITriggerRenderer {
public:
        // trigger waveform rendering
  virtual void renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels) = 0;
  virtual invTriggerRenderSignal_t & invTriggerRenderSignal() = 0;  
}; 

#endif // ITRIGGERRENDERER_H
