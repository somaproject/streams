#ifndef STREAMVIS_H
#define STREAMVIS_H

#include "wave.h"
#include "queueview.h"

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

}; 


#endif // STREAMVIS_H
