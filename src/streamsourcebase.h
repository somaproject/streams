#ifndef STREAMSOURCEBASE_H
#define STREAMSOURCEBASE_H

#include <somanetwork/network.h>

#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>

#include <list>

#include "streams.h"

class StreamSourceBase
{

 public:
  
  // signal
  virtual sigc::signal<void> newDataSignal() { return newDataSignal_;  }; 
  virtual sigc::signal<void> invalidateDataSignal() { return invalidateDataSignal_; };
  

protected: 
  sigc::signal<void> newDataSignal_; 
  sigc::signal<void> invalidateDataSignal_; 
  

  
};

typedef boost::shared_ptr<StreamSourceBase> pStreamSourceBase_t; 


#endif // STREAMSOURCEBASE_H

