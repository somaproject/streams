#ifndef WAVEVIS_H
#define WAVEVIS_H

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>



#include "visbase.h" 
#include "data/wave.h"
#include "wavevisrenderer.h"
#include "properties.h"

class WaveVis; 

typedef boost::shared_ptr<WaveVis> pWaveVis_t; 

class WaveVis : public VisBase
{
 public: 
  WaveVis(std::string Name, bf::path scratchdir); 
  ~WaveVis(); 

  static const std::string TYPENAME; 

  // StreamRenderer interface:
  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  
  // TriggerRenderer interface:
  void renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels); 
  int getPixelHeight(); 
  void setPixelHeight(int); 
  void setScale(float); 
  
  void accept(pIVisVisitor_t sv) { 
    sv->visit(this); 
  }

  Property<Gdk::Color> color; 
  Property<float> scale; 
  
private:
  core::SinkPad<WaveBuffer_t> * pSinkPad_; 

  void newData(); 
  void invalidateData(); 
  
  WaveVisRenderer streamRenderer_; 
  
  int pixelHeight_; 
  bf::path scratchdir_; 
};

#endif // WAVESTREAMVIS_H
