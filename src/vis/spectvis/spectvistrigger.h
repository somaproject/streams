#ifndef SPECTVISTRIGGER_H
#define SPECTVISTRIGGER_H

#include <vector>
#include <assert.h>
#include "queueview.h"
#include "streamvis.h"
#include "wave.h"

class SpectVisTrigger
{
  // right now we just have a simple threshold-crossing trigger
 public:
  SpectVisTrigger(SpectBlockpVector_t * pSpectBlocks); 
  ~SpectVisTrigger(); 
  
  void newSample(); 
  void invalidateSamples(); 

  // trigger control
  void setTriggerValue(float); 
  void enableTrigger(bool value); 
  
  // downstream update
  updateTriggersSignal_t & updateTriggersSignal(); 

  QueueView<float>  getTriggerQueueView(); 

 private:
  SpectBlockpVector_t * pSpectBlocks_; 

  updateTriggersSignal_t updateTriggersSignal_;
  
  float   triggerLevel_ ;  
  bool triggerEnabled_; 
  std::list<float> triggers_; 

  bool triggerFunc(GLWavePoint_t p1,  GLWavePoint_t p2);


};



#endif // WAVEVISTRIGGER_H
