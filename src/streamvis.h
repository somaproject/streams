#ifndef STREAMVIS_H
#define STREAMVIS_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>


#include "wave.h"
#include "queueview.h"
#include "streams.h"


// this is where we typedef the relevent typedefs

typedef sigc::signal<void> invWaveSignal_t; 
typedef sigc::signal<void, bool> updateTriggersSignal_t; 

class StreamVis
{

 public:
  // main wave rendering
  virtual void drawMainWave(wavetime_t t1, wavetime_t t2, int pixels) = 0;
  virtual invWaveSignal_t & invMainWaveSignal() = 0;  

  // trigger wave rendering
  virtual void drawTriggerWave(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint) = 0; 
  virtual invWaveSignal_t & invTriggerWaveSignal() = 0; 
  
  // trigger inputs
  virtual void updateTriggers(bool reset) = 0; 
  virtual void setTriggerSource(const QueueView<wavetime_t> & tqv) = 0;
  
  // trigger outputs
  virtual updateTriggersSignal_t & updateTriggersSignal() = 0; 
  virtual QueueView<wavetime_t> getTriggerQueueView() = 0; 

  // render position in the Y-axis, in pixels
  virtual float getYOffset() = 0; 

  
  // delete signal
  sigc::signal<void> & disconnectSignal() { return del_; }; 
  
  void disconnect(){
    // disconnect all objects monitoring this one
    del_.emit(); 
  }

 private:
  sigc::signal<void> del_; 

}; 


#endif // STREAMVIS_H
