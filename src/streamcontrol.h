#ifndef STREAMCONTROL_H
#define STREAMCONTROL_H


#include <boost/shared_ptr.hpp>
#include <somanetwork/network.h>

#include <string>
#include <map>
#include "streams.h"
#include "streamsource.h"

#include "streamvis.h"
typedef std::pair<datasource_t, datatype_t> datapair_t; 

typedef std::map<datapair_t, streamSourcePtr_t> dataDispatchMap_t; 


class StreamControl
{
 public:
  
  StreamControl(NetworkInterface *); 
  
  ~StreamControl(); 
  
  
  streamSourcePtr_t newSourceFactory(std::string name, datasource_t ds); 
  
  streamVisPtr_t newVisFactory(streamSourcePtr_t src, std::string name); 

  void remove(streamSourcePtr_t source); 
  
  void remove(streamVisPtr_t vis); 
  
  std::list<streamVisPtr_t> visPtrList; 

  void datadispatch(DataPacket_t * pdp); 
  void eventdispatch(EventList_t * pel); 
  
  bool dataRXCallback(Glib::IOCondition io_condition); 
  bool eventRXCallback(Glib::IOCondition io_condition); 
  
  sigc::signal<void, float> & timeSignal() {
    return timeSignal_; 
  }

 private:
  sourcePtrList_t sourceList_; 
  visPtrMap_t visMap_; 
  void dispatch(DataPacket_t * pdp ); 

  dataDispatchMap_t dataDispatchMap_; 

  visPtrMap_t::iterator findVis(streamVisPtr_t v); 
  
  NetworkInterface * pNetworkInterface_; 

  void setTime(float); 
  
  sigc::signal<void, float> timeSignal_; 

};

#endif // STREAMCONTROL_H
