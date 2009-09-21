#ifndef NETDATARAW_H
#define NETDATARAW_H
#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include <data/wave.h>
#include <core/element.h>
#include <core/sourcepad.h>
#include <string>

#include "timer.h" 

#include <core/queueview.h>
#include "properties.h"

#include "sourcebase.h"
#include "networkdatacache.h"
#include "isomanetcodec.h"

class NetDataRaw : public SourceBase
{
public:
  static const std::string TYPENAME; 
  NetDataRaw(std::string, pTimer_t, pNetworkDataCache_t, 
	     pISomaNetCodec_t, bf::path scratch); 
  ~NetDataRaw(); 
  
  
  void accept(pISourceVisitor_t sv) { 
    sv->visit(this); 
  }

  // primary property : data source
  Property<datasource_t> src; 
  std::list<datasource_t> getAvailableSources(); 

  // physical amplifier properties:
  PropertyProxy<int> gain; 
  std::list<int> getAvailableGains(); 
  Property<bool> hpfen; 
  Property<int> selchan; 

  // DSP-level properties
  typedef std::pair<int, int> samprate_t; 
  Property<samprate_t>  sampratenum; 
  std::list<samprate_t> getAvailableSampRates(); 
  
private:
  core::SourcePad<WaveBuffer_t> * pSourcePad_; 
  boost::ptr_list<WaveBuffer_t>  datalist_; 
  
  pTimer_t pTimer_; 
  pNetworkDataCache_t pNetDataCache_; 
  pISomaNetCodec_t pNetCodec_; 
  void nextData(); 
  void timeUpdate(streamtime_t t); 

  sigc::connection dataConn_; 
  
  core::QueueView<WaveBuffer_t> dataQueueView_; 
  
  void setSrc(datasource_t src); 
  void reconnectSource(datasource_t src); 
  datasource_t src_; 

  sigc::signal<void, int> gainSignal_; 
  void gainFilter(int chan, int gain); 

  void reconnectPropertyProxies(); 

  static const int CONTCHANNEL = 4; 
}; 

typedef boost::shared_ptr<NetDataRaw> pNetDataRaw_t; 

  
#endif // NETDATWAVE
