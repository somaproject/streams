#ifndef SPECTVIS_H
#define SPECTVIS_H

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

#include "visbase.h"

#include "data/wave.h"
#include "spectvisrenderer.h"
#include "properties.h"



class SpectVis; 

typedef boost::shared_ptr<SpectVis> pSpectVis_t; 

typedef float wavetime_t; 

class SpectVisRenderer; 

class SpectVis : public VisBase
{
  /*
    Spect vis looks at a window of some width, 
    with some overlap, and then queries the incoming wave buffers. 
    
    Windowsize is in seconds

    As data arrives, we see if we can compute a FFT, and if so, 
    do it. 

    
    
   */ 
 public: 
  SpectVis(std::string name, bf::path scratchdir); 
  ~SpectVis(); 

  static const std::string TYPENAME; 

  // The generic StreamVis Interface :
  
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

  //  Property<Gdk::Color> color; 
  Property<float> scale; 
    
  Property<int> fftN; 
  Property<float> windowsize; 
  Property<float> overlapFactor; 
  
  void process(elements::timeid_t tid); 

 private:
  elements::SinkPad<WaveBuffer_t>::pSinkPad_t pSinkPad_; 

  SpectVisRenderer * streamRenderer_; 

  int pixelHeight_; 
//   float yoffset_; 
  float yheight_; 

  float verticalScale_; 
//   sigc::signal<void, float> verticalScaleSignal_; 

//  Gdk::Color color_; 
//   sigc::signal<void, Gdk::Color> colorSignal_; 

//   GLWavePoint_t filterNextPoint(GLWavePoint_t wp);
  bf::path scratchdir_; 
  
  pDb_t spectblockdb_; 
  
  std::map<timeid_t, WaveBuffer_t> waveBufferCache_; 


  
};

#endif // WAVESTREAMVIS_H
