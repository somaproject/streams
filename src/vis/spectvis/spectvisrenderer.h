#ifndef SPECTVISRENDERER_H
#define SPECTVISRENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>

#include "data/wave.h"
#include "queueview.h"
#include "properties.h"
#include "vis/spectvis/data.h"
#include <vis/spectvis/fftengine.h> 
// typedef std::map<streamtime_t, int> timeindex_t; 

// typedef boost::numeric::interval_lib::rounded_arith_exact< int > rounding_policy;
// typedef boost::numeric::interval_lib::checking_no_nan< int > checking_policy;
// typedef boost::numeric::interval_lib::policies< rounding_policy, checking_policy > interval_policies;
// typedef boost::numeric::interval< int, interval_policies > interval_t;


// struct SpectBlockTextureCacheItem_t {
//   bool hastexture; 
//   GLuint textureName;
// }; 

namespace spectvis { 

class SpectVisRenderer
{
 public: 
  SpectVisRenderer(FFTEngine & eng); 

  ~SpectVisRenderer(); 

  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 

  Property<Gdk::Color> color; 
  Property<float> scale; 

 protected: 

  //float pixheight_; 
  

//   std::vector<SpectBlockTextureCacheItem_t> texCache_; 
  FFTEngine fftengine_; 

};

}

#endif 
