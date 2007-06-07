#ifndef WAVESTREAMTRIGGER_H
#define WAVESTREAMTRIGGER_H

#include <assert.h>
#include "queueview.h"
#include "wave.h"



class WaveStreamTrigger
{
  // right now we just have a simple threshold-crossing trigger
 public:
  WaveStreamTrigger(); 
  ~WaveStreamTrigger(); 
  
  void append(GLWavePoint_t); 


  // trigger control
  void setTriggerValue(float); 
  void enableTrigger(bool value); 
  
  // downstream update
  newTriggersSignal_t & newTriggersSignal();  
  invalidateTriggersSignal_t & invalidateTriggersSignal(); 
  QueueView<float>  getTriggerQueueView(); 

 private:
  std::vector<GLWavePoint_t> samples_; 
  newTriggersSignal_t newTriggersSignal_;
  invalidateTriggersSignal_t invalidateTriggersSignal_;  
  
  float   triggerLevel_ ;  
  bool triggerEnabled_; 
  std::list<float> triggers_; 

  bool triggerFunc(GLWavePoint_t p1,  GLWavePoint_t p2);



};



#endif // WAVESTREAMTRIGGER_H
