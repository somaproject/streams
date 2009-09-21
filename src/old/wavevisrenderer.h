#ifndef WAVEVISRENDERER_H
#define WAVEVISRENDERER_H

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


typedef std::map<wavetime_t, int> timeindex_t; 

class WaveVisRenderer
{
 public: 
  WaveVisRenderer(std::vector<GLWavePoint_t> * pSamples); 

  ~WaveVisRenderer(); 

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
  std::vector<GLWavePoint_t> * pSamples_; 
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
  
};

#endif // WAVE_H
