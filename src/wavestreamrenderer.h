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

inline bool compareTime2(const GLWaveQuadStrip_t x,
			const GLWaveQuadStrip_t y)
{
  if (x.tmax < y.tmax) 
    return true; 

  return false; 
}

typedef std::map<wavetime_t, int> timeindex_t; 

class WaveStreamRenderer
{
 public: 
  WaveStreamRenderer(std::vector<GLWavePoint_t> * pSamples); 

  ~WaveStreamRenderer(); 

  void draw(wavetime_t t1, wavetime_t t2, int pixels); 
  void newSample(void); 
  void invalidateSamples(void); 

  // triggering
  void updateTriggers(bool); 
  void setTriggerSource(const QueueView<wavetime_t> & tqv);

  sigc::signal<void> & invWaveSignal();
  
  void setScale(float); 

 protected: 
  std::vector<GLWavePoint_t> * pSamples_; 
  std::vector<GLWaveQuadStrip_t> ratesS2_; 
  timeindex_t indexS2_, indexS3_; 

  wavetime_t mostRecentRenderT1_, mostRecentRenderT2_; 
  sigc::signal<void> invWaveSignal_; 

  std::list<wavetime_t> emptyTriggerList_; 

  QueueView<wavetime_t> triggerQueueView_; 

  TriggerTimeList_t trigTimeList_ ;
  float scale_;
};

#endif // WAVE_H
