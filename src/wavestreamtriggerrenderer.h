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

typedef std::map<float, int> timeindex_t; 

class WaveStreamTriggerRenderer
{
 public: 
  WaveStreamTriggerRenderer(); 
  ~WaveStreamTriggerRenderer(); 

  void drawTriggers(float tbefore, float tafter, float timepoint); 
  void append(GLWavePoint_t); 
  
/*   // triggering  */
  void resetTriggers();  
  void newTriggers(); 
  void setTriggerSource(const QueueView<float> & tqv);

    
/*   void setTriggerLevel(float tv);  */
  sigc::signal<void> & invalidateTriggerRenderSignal();
  
 protected: 
  
  std::vector<GLWavePoint_t> rates_; 
  
  sigc::signal<void> invalidateTriggerRenderSignal_; 

  std::list<float> emptyTriggerList_; 

  QueueView<float> triggerQueueView_; 

  TriggerTimeList_t trigTimeList_ ;

};

#endif // WAVE_H
