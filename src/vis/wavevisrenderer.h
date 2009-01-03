#ifndef WAVEVISRENDERER_H
#define WAVEVISRENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>

#include <data/wave.h>
#include "core/sinkpad.h"


//typedef std::map<wavetime_t, int> timeindex_t; 

/*
  Handles all of the actual rendering, the storage and handling
  of the GL-renderable objects (including caching and downsampling)
  
  
*/

typedef float wavetime_t; 

struct GLWavePoint_t
{
  wavetime_t t; 
  float x; 
}; 

struct GLWaveQuadStrip_t
{
  wavetime_t tmax; 
  float xmax; 
  wavetime_t tmin; 
  float xmin; 
}; 

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



class WaveVisRenderer
{

 public: 
  WaveVisRenderer(); 

  ~WaveVisRenderer(); 
  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 

  void newSample(const WaveBuffer_t & ); 
  void invalidateSamples(void); 
  void setColor(Gdk::Color c); 
  
//   // triggering
//   void updateTriggers(bool); 
//   void setTriggerSource(const QueueView<wavetime_t> & tqv);

  sigc::signal<void> & invWaveSignal();
  
  void setScale(float scale, float pixheight); 
  
protected: 
  std::vector<GLWavePoint_t> pSamples_; 
  //std::vector<GLWaveQuadStrip_t> ratesS2_; 
  //timeindex_t indexS2_, indexS3_; 

  streamtime_t mostRecentRenderT1_, mostRecentRenderT2_; 
  sigc::signal<void> invWaveSignal_; 

  //std::list<wavetime_t> emptyTriggerList_; 
  
  //QueueView<_t> triggerQueueView_; 

  typedef std::vector<GLWavePoint_t> GLPointBuffer_t;
  typedef std::list<GLPointBuffer_t * > pGLPointBufferList_t;  
  static const int BUFFERN = 4096; 
  static const int SCALECNT = 10; 
  static const int SCALEFACTOR = 2; 
  
  std::vector<pGLPointBufferList_t> bufferScales_; 
  std::vector<int> bufferPos_; 
  std::vector<int> bufferScaleThresholds_; 

//   TriggerTimeList_t trigTimeList_ ;
  float scale_;
  float pixheight_; 
  Gdk::Color color_; 
  void setGLColor(float alpha); 

  // debugging
  void printStatus(); 
  
};

#endif // WAVE_H
