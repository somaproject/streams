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
#include "renderdownsample.h"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem; 

//typedef std::map<wavetime_t, int> timeindex_t; 

/*
  Handles all of the actual rendering, the storage and handling
  of the GL-renderable objects (including caching and downsampling)
  
  
*/

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
  WaveVisRenderer(bf::path scratchdir); 

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


  static const int BUFFERN = 4096; 
  static const int SCALECNT = 10; 
  static const int SCALEFACTOR = 2; 
  

  double scale_; 

//   TriggerTimeList_t trigTimeList_ ;
  float pixheight_; 
  Gdk::Color color_; 
  void setGLColor(float alpha); 

  // debugging
  void printStatus(); 

  // float value is the _largetst_ window size
  // that this level should be used for rendering
  typedef std::map<float, IRenderer*> rendermap_t; 
  
  rendermap_t renderers_; 

  bf::path scratchdir_;
};

#endif // WAVE_H
