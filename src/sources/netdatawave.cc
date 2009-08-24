#include "netdatawave.h"

const std::string NetDataWave::TYPENAME = "NetDataWave"; 
using namespace somadspio; 

NetDataWave::NetDataWave(std::string name, pTimer_t timer, 
			 pNetworkDataCache_t ndc,
			 pISomaNetCodec_t pnc) :
  SourceBase(name), 
  pTimer_(timer), 
  pNetDataCache_(ndc), 
  pSourcePad_(createSourcePad<WaveBuffer_t>(datalist_, "default")), 
  pNetCodec_(pnc), 
  src(0), 
  src_(0)
{
  
  
  pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
						    &NetDataWave::timeUpdate)); 
  
  pNetCodec_->getDSPStateProxy(src_); 
  pNetCodec_->getDSPStateProxy(src_).acqdatasrc.gain().connect
    (sigc::mem_fun(*this, &NetDataWave::gainFilter)); 
  reconnectPropertyProxies(); 

  src.signal().connect(sigc::mem_fun(*this, 
				     &NetDataWave::setSrc)); 
  std::cout << "NetDataWave done constructing." << std::endl;
  reconnectSource(src_); 
}

void NetDataWave::gainFilter(int chan, int gain) 
  /* 
     The propertyProxy we use for gain expects a signal with a single
     gain, whereas the acqdatasource emits signals for all gain changes. 
     Thus we must filter out for the continuous channel
  */
{
  std::cout << "NetDataWave::gainFilter " << chan << " " << gain << std::endl; 
  if (chan == CONTCHANNEL) {
    std::cout << "emitting gain signal" << std::endl; 
    gainSignal_.emit(gain);     
  }
}


NetDataWave::~NetDataWave()
{

}

void NetDataWave::setSrc(datasource_t src)
{

  if (src == src_) {
    //return true; 
  } else { 
    reconnectSource(src);    
  }

}

void NetDataWave::nextData()
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

void NetDataWave::timeUpdate(streamtime_t t)
{
  
  

}

void NetDataWave::reconnectSource(datasource_t src)
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
  dataQueueView_ = pNetDataCache_->getNetWaveSource(src_); 

  while( !  dataQueueView_.empty() )
    {
      WaveBuffer_t *  newbuf = new WaveBuffer_t(dataQueueView_.front()); 
      datalist_.push_back(newbuf); 
      dataQueueView_.pop(); 
    }
  
  pSourcePad_->invalidateData(); 

  dataConn_ = 
    pNetDataCache_->waveSignals_[src_].connect(sigc::mem_fun(*this,
							     &NetDataWave::nextData)); 
}

void NetDataWave::reconnectPropertyProxies()
{
  sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc,
		&AcqDataSource::setGain); 

  gain.resetProxy(sigc::mem_fun(*this, &NetDataWave::setGainProxy), 
		  sigc::mem_fun(*this, &NetDataWave::getGainProxy), 
 		  gainSignal_); 
  

}

int NetDataWave::getGainProxy() 
{

  boost::optional<int> b = pNetCodec_->getDSPStateProxy(src_).acqdatasrc.getGain(CONTCHANNEL) ;
  int val = 0; 

  if (b) {
    return val = *b; 
  }; 
  
  std::cout << "NetDataWave::getGainProxy() " << val << std::endl;  
  return val; 
  
}

void NetDataWave::setGainProxy(int x)
{
  std::cout << "NetDataWave::setGainProxy() " << x  << std::endl;

  pNetCodec_->getDSPStateProxy(src_).acqdatasrc.setGain(CONTCHANNEL, x);

}

std::list<datasource_t> NetDataWave::getAvailableSources()
{
  std::list<datasource_t> srcs; 
  // FIXME : Dynamically read from soma
  for(int i = 0; i < 64; i++) {
    srcs.push_back(i); 
  }
  return srcs; 

}
