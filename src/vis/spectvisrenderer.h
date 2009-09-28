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
#include <boost/numeric/interval.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


typedef std::map<streamtime_t, int> timeindex_t; 

typedef boost::numeric::interval_lib::rounded_arith_exact< int > rounding_policy;
typedef boost::numeric::interval_lib::checking_no_nan< int > checking_policy;
typedef boost::numeric::interval_lib::policies< rounding_policy, checking_policy > interval_policies;
typedef boost::numeric::interval< int, interval_policies > interval_t;



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

class SpectVisRenderer
{
 public: 
  SpectVisRenderer(SpectBlockpVector_t * pSpectBlocks); 

  ~SpectVisRenderer(); 

  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  void newSample(void); 
  void invalidateSamples(void); 
  void setColor(Gdk::Color c); 

//   // triggering
//   void updateTriggers(bool); 
//   void setTriggerSource(const QueueView<streamtime_t> & tqv);

  sigc::signal<void> & invWaveSignal();
  
  void setScale(float scale, float pixheight); 

 protected: 
  SpectBlockpVector_t * pSpectBlocks_; 

  timeindex_t indexS2_, indexS3_; 

  streamtime_t mostRecentRenderT1_, mostRecentRenderT2_; 
  sigc::signal<void> invWaveSignal_; 

//   std::list<streamtime_t> emptyTriggerList_; 

//   QueueView<streamtime_t> triggerQueueView_; 

//   TriggerTimeList_t trigTimeList_ ;
  float scale_;
  float pixheight_; 
  Gdk::Color color_; 
  void setGLColor(float alpha); 

  // debugging
  void printStatus(); 
 
  // Texture Cache: 
  std::vector<SpectBlockTextureCacheItem_t> texCache_; 
  interval_t lastRenderInterval_; 
    
};

#endif // WAVE_H
