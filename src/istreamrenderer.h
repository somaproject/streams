#ifndef ISTREAMRENDERER_H
#define ISTREAMRENDERER_H
#include <sigc++/sigc++.h>
#include "streamtime.h"
/*
  Interface for all vis elements that support rendering to the main 
  stream window. 

*/


typedef sigc::signal<void> invStreamRenderSignal_t; 

class IStreamRenderer {
public:
  // main stream waveform rendering
  virtual void renderStream(streamtime_t t1, streamtime_t t2, int pixels) = 0;
  virtual invStreamRenderSignal_t & invStreamRenderSignal() = 0;  
}; 

#endif // ISTREAMRENDERER_H
