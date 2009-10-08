#ifndef WAVEVIS2_TYPES
#define WAVEVIS2_TYPES

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <boost/filesystem.hpp>

#include <data/wave.h>

#include "irenderer.h"
#include <boost/thread.hpp>
namespace bf = boost::filesystem; 

typedef float wavetime_t; 

struct GLWavePoint_t
{
  wavetime_t t; 
  float x; 
}; 

struct GLPointBuffer_t
{
  static const int BUFFERN = 1024; // Maximum size in data points of a buffer
  size_t size; 
  boost::array<GLWavePoint_t, BUFFERN> data; 
  GLPointBuffer_t() : 
    size(0)
  {

  }

}; 


#endif
