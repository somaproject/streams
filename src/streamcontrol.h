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

typedef std::map<datapair_t, pStreamSource_t> dataDispatchMap_t; 


class StreamControl
{
 public:
  
  StreamControl(NetworkInterface *); 
  
  ~StreamControl(); 
  
  
  pStreamSource_t newSourceFactory(std::string name, datasource_t ds); 
  
  pStreamVis_t newVisFactory(pStreamSource_t src, std::string name); 

  void remove(pStreamSource_t source); 
  
  void remove(pStreamVis_t vis); 
  
  std::list<pStreamVis_t> visPtrList; 

  void datadispatch(pDataPacket_t pdp); 
  void eventdispatch(pEventList_t pel); 
  
  bool dataRXCallback(Glib::IOCondition io_condition); 
  bool eventRXCallback(Glib::IOCondition io_condition); 
  
  sigc::signal<void, float> & timeSignal() {
    return timeSignal_; 
  }

 private:
  sourcePtrList_t sourceList_; 
  visPtrMap_t visMap_; 
  void dispatch(pDataPacket_t pdp ); 

  dataDispatchMap_t dataDispatchMap_; 

  visPtrMap_t::iterator findVis(pStreamVis_t v); 
  
  NetworkInterface * pNetworkInterface_; 

  void setTime(float); 
  
  sigc::signal<void, float> timeSignal_; 

};

#endif // STREAMCONTROL_H
