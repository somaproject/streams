#ifndef FAKEWAVE_H
#define FAKEWAVE_H

#include <vector>
#include <sigc++/sigc++.h>
#include <somanetwork/wave.h>
#include <boost/shared_ptr.hpp>
#include "timer.h" 

#include "queueview.h"
#include "streamsource.h"
#include "waves.h" 

class FakeWave : public StreamSource<WaveBuffer_t>
{
 public:
  // this is just a prototype source
  FakeWave(pTimer_t); 

  ~FakeWave(); 

  // emit updatedData
  void setFs(double fs); 
  void setVariance(double var); 
  

 private:
  double fs_;
  double variance_;
  pTimer_t pTimer_; 
  void nextData(); 
  
  void timeUpdate(streamtime_t t); 
  streamtime_t lastTime_; 

};

#endif // FAKEWAVE_H
