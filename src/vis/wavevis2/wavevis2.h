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
#include "wavevisrenderer.h"
#include <elements/property.h>

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
  elements::SinkPad<WaveBuffer_t>::pSinkPad_t pSinkPad_; 

  void newData(); 
  void invalidateData(); 
  
  
  int pixelHeight_; 
  bf::path scratchdir_; 
  WaveVisRenderer streamRenderer_; 
};

#endif // WAVESTREAMVIS_H
