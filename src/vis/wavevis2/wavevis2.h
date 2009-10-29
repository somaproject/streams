#ifndef __STREAMS_VIS_WAVEVIS2_H__
#define __STREAMS_VIS_WAVEVIS2_H__

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>



#include <vis/visbase.h>
#include <data/wave.h>
#include "renderall.h"
#include "renderdownsample.h"
#include <elements/property.h>
#include <boost/thread/shared_mutex.hpp>
#include <boost/itl/interval_set.hpp>

class WaveVis2; 

typedef boost::shared_ptr<WaveVis2> pWaveVis2_t; 

class WaveVis2 : public VisBase
{
 public: 
  WaveVis2(std::string Name, bf::path scratchdir); 
  ~WaveVis2(); 

  static const std::string TYPENAME; 

  // StreamRenderer interface:
  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  
  // TriggerRenderer interface:
  void renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels); 
  int getPixelHeight(); 
  void setPixelHeight(int); 
  void setScale(float); 
  
  void accept(pIVisVisitor_t sv) { 
    if(sv)
      sv->visit(this); 
  }

  //  elements::Property<Gdk::Color> color; 
  elements::Property<float> scale; 
  
  void process(elements::timeid_t tid); 

private:
  elements::SinkPad<pWaveBuffer_t>::pSinkPad_t pSinkPad_; 

//   void newData(); 
//   void invalidateData(); 
  
  
  int pixelHeight_; 
  bf::path scratchdir_; 

  wavevis2::RenderAll renderall_; 
  typedef std::map<timeid_t, wavevis2::RenderDownSample*> dsmap_t; 
  dsmap_t downsampledRenderers_; 
  void reset(); 
  elements::timewindow_t most_recent_render_window_; 

  typedef boost::itl::interval_set<timeid_t> intervals_t; 
  intervals_t observed_intervals_; 
  
  size_t most_recent_series_; 

};

#endif // WAVESTREAMVIS_H
