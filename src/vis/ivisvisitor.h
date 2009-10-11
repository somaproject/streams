#ifndef IVISVISITOR_H
#define IVISVISITOR_H

//#include <sources/noisewave.h>
#include "data/wave.h"
//#include <vis/sinevis.h>
//#include <vis/wavevis.h>

class VisBase; 
// class SineVis; 
// class WaveVis; 
class WaveVis2; 
class SpectVis; 

class IVisVisitor {
public:
  virtual void visit(VisBase * ) = 0; 
  //virtual void visit(SineVis * ) = 0; 
  virtual void visit(WaveVis2 * ) = 0; 
  virtual void visit(SpectVis * ) = 0; 
  
}; 


typedef boost::shared_ptr<IVisVisitor> pIVisVisitor_t; 

#endif //IVISVISITOR_H
