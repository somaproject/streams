#ifndef SOURCESTATE_H
#define SOURCESTATE_H

#include <boost/shared_ptr.hpp>
#include "timer.h"
#include "networkdatacache.h"

/*
  Encapsulates all the state available to sources, such as
  a pointer to a network object, or an audio device. 
  
  Used to centralize the singleton-like objects
  without actually having to make them singletons. 
  
*/
struct SourceState {
  pTimer_t timer; 
  pNetworkDataCache_t netdatacache; 
  pSomaNetCodec_t somanetcodec; 
}; 

typedef boost::shared_ptr<SourceState> pSourceState_t; 

#endif // SOURCESTATE_H
