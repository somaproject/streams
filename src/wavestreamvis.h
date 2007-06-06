#ifndef WAVESTREAMVIS_H
#define WAVESTREAMVIS_H

#include <gtkmm.h>
#include "wave.h"
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
 
 private:
  void newData(); 
  void invalidateData(); 

  WaveStreamRenderer streamRenderer_; 
  WaveStreamSource * streamSource_; 
  QueueView<WaveBuffer_t *> inDataQueue_; 
};

#endif // WAVESTREAMVIS_H
