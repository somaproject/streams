#ifndef WAVESTREAMRENDERER_H
#define WAVESTREAMRENDERER_H

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

#include "wavestreamsource.h"
#include "wave.h"


inline bool compareTime(const GLWavePoint_t x,
			const GLWavePoint_t y)
{
  if (x.t < y.t) 
    return true; 

  return false; 
}

typedef std::map<float, int> timeindex_t; 

class WaveStreamRenderer
{
 public: 
  WaveStreamRenderer(); 
  ~WaveStreamRenderer(); 

  void draw(float t1, float t2, int pixels); 
  void append(GLWavePoint_t); 

/*   // triggering  */
/*   void resetTriggers();  */
/*   void appendTriggers(const TriggerTimeList_t & ttl);  */

/*   // and associated triggering signals */
/*   newTriggersSignal_t newTriggers();  */
/*   invalidateTriggersSignal_t invalidateTriggers();  */
  
  
/*   void setTriggerLevel(float tv);  */
  sigc::signal<void> & invalidateLastRenderSignal()
    { return invalidateLastRenderSignal_;
    }

 protected: 
 
  std::vector<GLWavePoint_t> rates_; 
  std::vector<GLWavePoint_t> ratesS2_; 
  std::vector<GLWavePoint_t> ratesS3_; 
  timeindex_t indexS2_, indexS3_; 

  float mostRecentRenderT1_, mostRecentRenderT2_; 
  sigc::signal<void> invalidateLastRenderSignal_; 

/*   TriggerTimeList_t trigTimeList_ ;  */

/*   // internal signals */
/*   newTriggersSignal_t newTriggerSignal_; */
/*   invalidateTriggersSignal_t invalidateTriggersSignal_;  */
  
/*   float   triggerLevel_ ;  */

};

#endif // WAVE_H
