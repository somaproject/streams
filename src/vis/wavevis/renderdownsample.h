#ifndef RENDERDOWNSAMPLE_H
#define RENDERDOWNSAMPLE_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <boost/filesystem.hpp>
#include <db_cxx.h>

#include <data/wave.h>

#include "core/sinkpad.h"
#include "vis/irenderer.h"

namespace bf = boost::filesystem; 

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

struct GLPointBuffer_t
{
  static const int BUFFERN = 2048; // Maximum size in data points of a buffer
  size_t size; 
  boost::array<GLWavePoint_t, BUFFERN> data; 
  GLPointBuffer_t() : 
    size(0)
  {

  }

}; 

class RenderDownSample : public IRenderer
{

public:
  /* detail: in general, a pixel along the x (t) axis will be 
     about this time duration. 0 means "as fine as possible"

  */
  
  RenderDownSample(float detail, bf::path scrachdir);
  ~RenderDownSample(); 

  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  void newSample(const WaveBuffer_t & ); 

private:
  



  //  typedef std::map<double, GLPointBuffer_t * > timeBufferMap_t;  


  float detail_; 
  const float NEWBUFTIME_; 
  
//   timeBufferMap_t tbm_; 

//   timeBufferMap_t::iterator currentTBiterator_; 
  
  GLPointBuffer_t buffer_; 
  double bufstarttime_; 

  void newDataPoint(double streamtime, float data); 

  Db * pdb_; 

  void renderGLPointBuffer(double origintime, GLPointBuffer_t  * buf); 
}; 

#endif 
