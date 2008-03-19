#ifndef SPECTVISRENDERER_H
#define SPECTVISRENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>

#include "wave.h"
#include "queueview.h"
#include <boost/numeric/interval.hpp>



typedef std::map<wavetime_t, int> timeindex_t; 

typedef boost::numeric::interval_lib::rounded_arith_exact< int > rounding_policy;
typedef boost::numeric::interval_lib::checking_no_nan< int > checking_policy;
typedef boost::numeric::interval_lib::policies< rounding_policy, checking_policy > interval_policies;
typedef boost::numeric::interval< int, interval_policies > interval_t;


class SpectVisRenderer
{
 public: 
  SpectVisRenderer(SpectBlockpVector_t * pSpectBlocks); 

  ~SpectVisRenderer(); 

  void draw(wavetime_t t1, wavetime_t t2, int pixels); 
  void newSample(void); 
  void invalidateSamples(void); 
  void setColor(Gdk::Color c); 

  // triggering
  void updateTriggers(bool); 
  void setTriggerSource(const QueueView<wavetime_t> & tqv);

  sigc::signal<void> & invWaveSignal();
  
  void setScale(float scale, float pixheight); 

 protected: 
  SpectBlockpVector_t * pSpectBlocks_; 

  std::vector<GLWaveQuadStrip_t> ratesS2_; 
  timeindex_t indexS2_, indexS3_; 

  wavetime_t mostRecentRenderT1_, mostRecentRenderT2_; 
  sigc::signal<void> invWaveSignal_; 

  std::list<wavetime_t> emptyTriggerList_; 

  QueueView<wavetime_t> triggerQueueView_; 

  TriggerTimeList_t trigTimeList_ ;
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
