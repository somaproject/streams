#ifndef WAVESTREAMTRIGGERRENDERER_H
#define WAVESTREAMTRIGGERRENDERER_H

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

typedef std::map<wavetime_t, int> timeindex_t; 

class WaveStreamTriggerRenderer
{
 public: 
  WaveStreamTriggerRenderer(  std::vector<GLWavePoint_t> * pSamples); 
  ~WaveStreamTriggerRenderer(); 

  void drawTriggers(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint); 
  void newSample(); 
  void invalidateSamples(); 
  
/*   // triggering  */
  void updateTriggers(bool);
  void setTriggerSource(const QueueView<wavetime_t> & tqv);

    
/*   void setTriggerLevel(float tv);  */
  sigc::signal<void> & invWaveSignal();
  
 protected: 
  std::vector<GLWavePoint_t> * pSamples_; 
  
  sigc::signal<void> invWaveSignal_; 

  std::list<wavetime_t> emptyTriggerList_; 

  QueueView<wavetime_t> triggerQueueView_; 

  TriggerTimeList_t trigTimeList_ ;

};

#endif // WAVE_H
