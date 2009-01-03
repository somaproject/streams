#ifndef WAVE_H
#define WAVE_H

#define GL_GLEXT_PROTOTYPES
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/weak_ptr.hpp>
#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>
#include <boost/format.hpp>
#include <math.h>
#include <streamtime.h>


typedef float wavetime_t; 

typedef std::vector<float> TriggerTimeList_t; 


// helper functions
inline std::string voltsToString(float x)
{
  float absx = fabs(x); 
  if ( absx < 1e-6) {
    // nanovolts
    std::string st = boost::str(boost::format("%1% nV") % (x/1e-9)); 
    return st;

  } else if (absx < 1e-3) {
    // microvolts
    std::string st = boost::str(boost::format("%1% uV") % (x/1e-6)); 
    return st;

  } else if ( absx  < 1.0) {
    // millivolts 
    std::string st = boost::str(boost::format("%1% mV") % (x/1e-3)); 
    return st;

  } else {
    // volts
    std::string st = boost::str(boost::format("%1% V") % (x)); 
    return st;
    
  }
    
  
}

class SpectBlock_t {
public:
  streamtime_t starttime; 
  streamtime_t endtime; 
  int width; 
  int height; 
  std::vector<unsigned char> data; 
}; 

typedef  boost::ptr_vector<SpectBlock_t> SpectBlockpVector_t; 


struct SpectBlockTextureCacheItem_t {
  bool hastexture; 
  GLuint textureName;
}; 


inline bool compareStartTime(const SpectBlock_t x,
			     const SpectBlock_t y)
{
  if (x.starttime < y.starttime) 
    return true; 

  return false; 
}

inline bool compareEndTime(const SpectBlock_t x,
			     const SpectBlock_t y)
{
  if (x.endtime < y.endtime) 
    return true; 

  return false; 
}

#endif // WAVE_H
