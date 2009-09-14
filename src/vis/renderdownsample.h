#ifndef RENDERDOWNSAMPLE_H
#define RENDERDOWNSAMPLE_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <data/wave.h>
#include "core/sinkpad.h"
#include "irenderer.h"


typedef float wavetime_t; 

struct GLWavePoint_t
{
  wavetime_t t; 
  float x; 
}; 

struct GLWaveQuadStrip_t
{
  wavetime_t tmax; 
  float xmax; 
  wavetime_t tmin; 
  float xmin; 
}; 


class RenderDownSample : public IRenderer
{

public:
  /* detail: in general, a pixel along the x (t) axis will be 
     about this time duration. 0 means "as fine as possible"

  */
  
  RenderDownSample(float detail);
  ~RenderDownSample(); 

  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  void newSample(const WaveBuffer_t & ); 

private:
  typedef std::vector<GLWavePoint_t> GLPointBuffer_t;
  typedef std::map<double, GLPointBuffer_t * > timeBufferMap_t;  
  
  static const int BUFFERN = 4096; // Maximum size in data points of a buffer

  float detail_; 
  const float NEWBUFTIME_; 

  timeBufferMap_t tbm_; 

  timeBufferMap_t::iterator currentTBiterator_; 
  
  void newDataPoint(double streamtime, float data); 

  

}; 

#endif 
