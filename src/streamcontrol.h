#ifndef STREAMCONTROL_H
#define STREAMCONTROL_H


#include <boost/shared_ptr.hpp>
#include <somanetwork/network.h>

#include <string>

#include "streams.h"
#include "streamsource.h"

#include "streamvis.h"


class StreamControl
{
 public:
  
  StreamControl(NetworkInterface *); 
  
  ~StreamControl(); 
  
  
  streamSourcePtr_t newSourceFactory(std::string name); 
  
  streamVisPtr_t newVisFactory(streamSourcePtr_t src, std::string name); 

  void remove(streamSourcePtr_t source); 
  
  void remove(streamVisPtr_t vis); 
  
  std::list<streamVisPtr_t> visPtrList; 

 private:
  sourcePtrList_t sourceList_; 
  visPtrMap_t visMap_; 

  visPtrMap_t::iterator findVis(streamVisPtr_t v); 
  
  NetworkInterface * pNetworkInterface_; 

};

#endif // STREAMCONTROL_H
