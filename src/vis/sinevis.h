#ifndef SINEVIS_H
#define SINEVIS_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>


#include "data/wave.h"
#include "visbase.h"

/*
  A debugging element that simply renders a sine wave, with effectively
  no internal state beyond the frequency. 
  
*/

class SineVis : public VisBase
{
public:
  SineVis(std::string name, bf::path scratchdir); 
  ~SineVis(); 
  static const std::string TYPENAME; 
  
  // StreamRenderer interface:
  void renderStream(streamtime_t t1, streamtime_t t2, int pixels); 
  
  // TriggerRenderer interface:
  void renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels); 
  int getPixelHeight(); 
  void setPixelHeight(int x); 

  void accept(pIVisVisitor_t sv) { 
    sv->visit(this); 
  }

private:
  int pixelHeight_; 
  
}; 
typedef boost::shared_ptr<SineVis>  pSineVis_t; 


#endif // SINEVIS_H
