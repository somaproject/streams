#ifndef SOMANETCODEC_H
#define SOMANETCODEC_H

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <somanetwork/networkinterface.h>
#include <boost/enable_shared_from_this.hpp>
#include <sigc++/sigc++.h>
#include <somadspio/dspcontrol.h>

#include <gtkmm.h>
#include "streamtime.h"
#include "isomanetcodec.h"

class SomaNetCodec; 
class DataWaveCodec;

using namespace somanetwork; 
class SomaNetCodec :  public ISomaNetCodec, 
		      public boost::enable_shared_from_this<SomaNetCodec>
{
public:
  SomaNetCodec(pNetworkInterface_t pni); 
  
  sigc::signal<void, pDataPacket_t> & newDataSignal(); 
  sigc::signal<void, somatime_t> & signalTimeUpdate(); 

  void enableDataRX(datasource_t src, datatype_t typ); 
  
  somadspio::StateProxy & getDSPStateProxy(datasource_t src); 

private:
  pNetworkInterface_t pNetwork_; 
    
  bool dataRXCallback(Glib::IOCondition io_condition);
  bool eventRXCallback(Glib::IOCondition io_condition); 

  void parseEvent(const Event_t & evt); 
  void processNewEvents(pEventList_t pEventList);
  void sendEvents(const EventTXList_t & ); 

  sigc::signal<void, pDataPacket_t> newDataSignal_; 
  sigc::signal<void, somatime_t> signalTimeUpdate_;

  // wave state caching
  // ideally we'd fracture this out, but whatever, there are really only a few
  // data types
  
  static const int MAXDATASRC=64; 

  boost::ptr_vector<somadspio::StateProxy> dspStateProxies_; 

  // time subsampler
  static const somatime_t SOMATIME_DELAY = 1000; 
  somatime_t lastsomatime_; 
  
}; 

typedef boost::shared_ptr<SomaNetCodec> pSomaNetCodec_t; 

#endif // SOMANETCODEC_H
