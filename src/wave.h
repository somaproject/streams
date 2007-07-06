#ifndef WAVE_H
#define WAVE_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>
#include <boost/format.hpp>
#include <math.h>

typedef float wavetime_t; 

typedef std::vector<float> TriggerTimeList_t; 

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

#endif // WAVE_H
