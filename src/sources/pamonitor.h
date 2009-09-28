#ifndef PAMONITOR_H
#define PAMONITOR_H

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
#include "pasource.h"

class PulseAudioMonitorWave : public SourceBase
{
public:
  static const std::string TYPENAME;
  // this is just a prototype source
  PulseAudioMonitorWave(std::string, pTimer_t, bf::path); 
  
  ~PulseAudioMonitorWave(); 
  
//   // emit updatedData
//   void setFs(double fs); 
//   Property<float> amplitude; 
//   enum NoiseClass {WhiteNoise, NoisySine, SquareWave, BiModal}; 
//   Property<NoiseClass> noiseclass; 

  void accept(pISourceVisitor_t sv) { 
    if (sv != NULL) 
      sv->visit(this); 
  }
  
  


private:

  core::SourcePad<WaveBuffer_t> * pSourcePad_; 
  boost::ptr_list<WaveBuffer_t> dataList_; 
  double fs_;

  pTimer_t pTimer_; 
  void nextData(const float * f, unsigned l); 
  
//   void timeUpdate(streamtime_t t); 
//   streamtime_t lastTime_; 
  PASource pasource_; 
  boost::posix_time::ptime start_time; 
  size_t samppos_; 
};

typedef boost::shared_ptr<PulseAudioMonitorWave> pPulseAudioMonitorWave_t; 

#endif // FAKEWAVE_H
