#ifndef SPECTVISRENDERER_H
#define SPECTVISRENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>

#include "data/wave.h"
#include "properties.h"
#include "spectdownsample.h"
#include "lrucache.h"

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
  SpectVisRenderer(FFTEngine & eng, DownsampleCache & ds); 

  ~SpectVisRenderer(); 

  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 

  Property<Gdk::Color> color; 
  Property<float> scale; 

 protected: 

  //float pixheight_; 
  
  typedef LRUCache<size_t, GLuint> texturecache_t; 

  texturecache_t texturecache_; 
//   std::vector<SpectBlockTextureCacheItem_t> texCache_; 
  FFTEngine & fftengine_; 
  DownsampleCache & dscache_; 
  void render_high_res_stream(timeid_t t1, timeid_t t2, int pix); 
  void render_low_res_stream(timeid_t t1, timeid_t t2, int pix); 

  texturecache_t dstexturecache_; 
  
  static const size_t MAX_TEXTURE = 3000; 
  
};

}

#endif 
