#ifndef TRIGGERSINK_H
#define TRIGGERSINK_H

namespace soma { 
  namespace streams {
    
    
    class TriggerSink {
      
      // trigger inputs
      virtual void updateTriggers(bool reset) = 0; 
      virtual void setTriggerSource(const QueueView<wavetime_t> & tqv) = 0;
      

    }

  }

}


#endif // TRIGGERSINK_H
