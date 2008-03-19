#ifndef WAVEVISTRIGGER_H
#define WAVEVISTRIGGER_H

#include <vector>
#include <assert.h>
#include "queueview.h"
#include "streamvis.h"
#include "wave.h"


class WaveVisTrigger
{
  // right now we just have a simple threshold-crossing trigger
 public:
  WaveVisTrigger(std::vector<GLWavePoint_t> * pSamples); 
  ~WaveVisTrigger(); 
  
  void newSample(); 
  void invalidateSamples(); 

  // trigger control
  void setTriggerValue(float); 
  void enableTrigger(bool value); 
  
  // downstream update
  updateTriggersSignal_t & updateTriggersSignal(); 

  QueueView<float>  getTriggerQueueView(); 

 private:
  std::vector<GLWavePoint_t> * pSamples_;
  updateTriggersSignal_t updateTriggersSignal_;
  
  float   triggerLevel_ ;  
  bool triggerEnabled_; 
  std::list<float> triggers_; 

  bool triggerFunc(GLWavePoint_t p1,  GLWavePoint_t p2);


};



#endif // WAVEVISTRIGGER_H
