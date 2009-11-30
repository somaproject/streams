#ifndef NETDATAWAVE_H
#define NETDATAWAVE_H
#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include <data/wave.h>
#include <core/element.h>
#include <core/sourcepad.h>
#include <string>

#include "timer.h" 

#include <elements/property.h>

#include <sources/sourcebase.h>
#include "networkdatacache.h"
#include "isomanetcodec.h"
#include "notify.h"

class NetDataWave : public SourceBase
{
  /* 
     NetDataWave, for interfacing with Soma's Wave Sink Source
 
     Acquisition properties like src, gain, hpf-en
     Filtering properties like downsampling ratio, filter selection
     
     Note that it gets most of its data from the network data cache, 
     and directly communicates with the soma net codec and underlying
     dsp state proxy. 
     
     The properties here are a bit complex: Most are just pass-throughs
     for the soma net codec. That is, they in no way invalidate
     existing data or trigger a reset, but instead change the hardware
     properties, which impact all subsequent data. 
     
     
     This means that most of the property manipulation takes place
     outside of the primary process loop, and is instead a propertyProxy
     that connects to the acqdatasource. 

     The only property that actually impacts the data we're receiving
     is the "select a channel" property. 
     
   */ 

public:
  static const std::string TYPENAME; 
  NetDataWave(std::string, pTimer_t, pNetworkDataCache_t, 
	      pISomaNetCodec_t, bf::path scratch); 
  ~NetDataWave(); 
  
  
  void accept(pISourceVisitor_t sv) { 
    sv->visit(this); 
  }

  // primary property : data source
  elements::Property<datasource_t> src; 
  std::list<datasource_t> getAvailableSources(); 

  // physical amplifier properties:
  elements::Property<int > gain; 
  std::list<int> getAvailableGains(); 
  elements::Property<bool> hpfen; 
  elements::Property<int> selchan; 

  // DSP-level properties
  typedef std::pair<int, int> samprate_t; 
  elements::Property<samprate_t>  sampratenum; 
  std::list<samprate_t> getAvailableSampRates(); 
  elements::Property<uint32_t> filterid; 
  std::list<std::pair<uint32_t, std::string> > getAvailableFilterIDs(); 

  elements::datawindow_t<pWaveBuffer_t> get_src_data(const elements::timewindow_t & tw); 
  size_t get_sequence(); 

  
private:
  pTimer_t pTimer_; 
  pNetworkDataCache_t pNetDataCache_; 

  elements::SourcePad<pWaveBuffer_t>::pSourcePad_t  pSourcePad_; 
  
  pISomaNetCodec_t pNetCodec_; 
  void nextData(); 
  void timeUpdate(streamtime_t t); 

  sigc::connection dataConn_; 
  
  
  void setSrc(datasource_t src); 
  void reconnectSource(datasource_t src); 

  void reconnectPropertyProxies(); 

  static const int CONTCHANNEL = 4; 

  netdatawave::pPropertyNotify_t srcnotify_; 
  void process(elements::timeid_t tid);

  void on_src_change(); 
  bool on_check_property_change();

  sigc::connection gainconn_; 
  bool gain_pending_req_known_;
  void on_gain_update(int, int);


  sigc::connection hpfenconn_; 
  bool hpfen_pending_req_known_;
  void on_hpfen_update(int, bool);


  sigc::connection selchanconn_; 
  bool selchan_pending_req_known_;
  void on_selchan_update(int);


  static const int CONTCHAN = 4; 
  size_t seqid_; 

}; 

typedef boost::shared_ptr<NetDataWave> pNetDataWave_t; 

  
#endif // NETDATWAVE
