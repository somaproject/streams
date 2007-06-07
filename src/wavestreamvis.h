#ifndef WAVESTREAMVIS_H
#define WAVESTREAMVIS_H

#include <gtkmm.h>
#include "wave.h"
#include "wavestreamtrigger.h"
#include "wavestreamrenderer.h"


class WaveStreamVis
{
 public: 
  WaveStreamVis(WaveStreamSource * ); 
  ~WaveStreamVis(); 

  WaveStreamRenderer * getStreamRendererPtr() { return &streamRenderer_; }; 
  void drawMainWave(float t1, float t2, int pixels);

  sigc::signal<void> & invalidateLastRenderSignal()
    { return streamRenderer_.invalidateLastRenderSignal(); 
    }
 
  // the trigger genmerator interface
  newTriggersSignal_t &  newTriggersSignal();  
  invalidateTriggersSignal_t & invalidateTriggersSignal(); 
  QueueView<float> getTriggerQueueView(); 
  
  void setTriggerValue(float); 
  void enableTrigger(bool value); 

  // trigger inputs
  void resetTriggers();  
  void newTriggers(); 
  void setTriggerSource(const QueueView<float> & tqv);

 private:
  void newData(); 
  void invalidateData(); 

  WaveStreamRenderer streamRenderer_; 
  WaveStreamTrigger  streamTrigger_; 
  WaveStreamSource * streamSource_; 

  QueueView<WaveBuffer_t *> inDataQueue_; 
  
  
};

#endif // WAVESTREAMVIS_H
