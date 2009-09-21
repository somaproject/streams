#include "netdataraw.h"

const std::string NetDataRaw::TYPENAME = "NetDataRaw"; 
using namespace somadspio; 

NetDataRaw::NetDataRaw(std::string name, pTimer_t timer, 
		       pNetworkDataCache_t ndc,
		       pISomaNetCodec_t pnc, bf::path scratch) :
  SourceBase(name), 
  pTimer_(timer), 
  pNetDataCache_(ndc), 
  pSourcePad_(createSourcePad<WaveBuffer_t>(datalist_, "default")), 
  pNetCodec_(pnc), 
  src(0), 
  src_(0)
{
  
  
  pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
						    &NetDataRaw::timeUpdate)); 
  
  pNetCodec_->getDSPStateProxy(src_); 
  pNetCodec_->getDSPStateProxy(src_).acqdatasrc.gain().connect
    (sigc::mem_fun(*this, &NetDataRaw::gainFilter)); 
  reconnectPropertyProxies(); 

  src.signal().connect(sigc::mem_fun(*this, 
				     &NetDataRaw::setSrc)); 
  std::cout << "NetDataRaw done constructing." << std::endl;
  reconnectSource(src_); 

}

void NetDataRaw::gainFilter(int chan, int gain) 
  /* 
     The propertyProxy we use for gain expects a signal with a single
     gain, whereas the acqdatasource emits signals for all gain changes. 
     Thus we must filter out for the continuous channel
  */
{
  if (chan == CONTCHANNEL) {
    gainSignal_.emit(gain);     
  }
}


NetDataRaw::~NetDataRaw()
{

}

void NetDataRaw::setSrc(datasource_t src)
{

  if (src == src_) {
    //return true; 
  } else { 
    reconnectSource(src);    
  }

}

void NetDataRaw::nextData()
{
  while(!dataQueueView_.empty()) {
    // We have to copy the buffer here; this is a wee bit unfortunate,
    // but in general we'll be doing more than a pure copy. 
    WaveBuffer_t *  newbuf = new WaveBuffer_t(dataQueueView_.front()); 
    datalist_.push_back(newbuf); 
    dataQueueView_.pop(); 
  } 
  pSourcePad_->newData(); 

}

void NetDataRaw::timeUpdate(streamtime_t t)
{
  
  

}

void NetDataRaw::reconnectSource(datasource_t src)
{
  /*
    1. disconnect from current new data handler. 
    2. flush the current data buffer. 
    3. consume all new data 
    4. emit invalidate on sink pad
    5. reconnect to new data handler. 
  */ 

  if (dataConn_.connected()) {
    dataConn_.disconnect(); 
  }

  src_ = src; 

  datalist_.clear(); 
  dataQueueView_ = pNetDataCache_->getNetRawSource(src_); 

  while( !  dataQueueView_.empty() )
    {
      WaveBuffer_t *  newbuf = new WaveBuffer_t(dataQueueView_.front()); 
      datalist_.push_back(newbuf); 
      dataQueueView_.pop(); 
    }
  
  pSourcePad_->invalidateData(); 

  dataConn_ = 
    pNetDataCache_->rawSignals_[src_].connect(sigc::mem_fun(*this,
							     &NetDataRaw::nextData)); 
}

void NetDataRaw::reconnectPropertyProxies()
{
  sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc,
		&AcqDataSource::setGain); 

//   gain.resetProxy(sigc::bind(sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc,
// 				&AcqDataSource::setGain), CONTCHANNEL), 
//  		  sigc::bind(sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc, 
// 				&AcqDataSource::getGain), CONTCHANNEL), 
//  		  gainSignal_); 
    

}

std::list<datasource_t> NetDataRaw::getAvailableSources()
{
  std::list<datasource_t> srcs; 
  // FIXME : Dynamically read from soma
  for(int i = 0; i < 64; i++) {
    srcs.push_back(i); 
  }
  return srcs; 

}
