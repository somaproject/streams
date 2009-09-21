#ifndef NOISEWAVE_H
#define NOISEWAVE_H

#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include <data/wave.h>
#include <core/element.h>
#include <core/sourcepad.h>
#include <properties.h>
#include "timer.h" 

#include <core/queueview.h>
#include "sourcebase.h"

class NoiseWave : public SourceBase
{
public:
  static const std::string TYPENAME;
  // this is just a prototype source
  NoiseWave(std::string, pTimer_t, bf::path); 
  
  ~NoiseWave(); 
  
  // emit updatedData
  void setFs(double fs); 
  Property<float> amplitude; 
  enum NoiseClass {WhiteNoise, NoisySine, SquareWave, BiModal}; 
  Property<NoiseClass> noiseclass; 

  void accept(pISourceVisitor_t sv) { 
    sv->visit(this); 
  }
  


private:

  core::SourcePad<WaveBuffer_t> * pSourcePad_; 
  boost::ptr_list<WaveBuffer_t> dataList_; 
  double fs_;

  pTimer_t pTimer_; 
  void nextData(streamtime_t); 
  
  void timeUpdate(streamtime_t t); 
  streamtime_t lastTime_; 

};

typedef boost::shared_ptr<NoiseWave> pNoiseWave_t; 

#endif // FAKEWAVE_H
