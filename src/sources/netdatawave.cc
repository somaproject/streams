#include "netdatawave.h"

const std::string NetDataWave::TYPENAME = "NetDataWave"; 
using namespace dspiolib; 

NetDataWave::NetDataWave(std::string name, pTimer_t timer, 
			 pNetworkDataCache_t ndc,
			 pSomaNetCodec_t pnc) :
  SourceBase(name), 
  pTimer_(timer), 
  pNetDataCache_(ndc), 
  pSourcePad_(createSourcePad<WaveBuffer_t>(datalist_, "default")), 
  pNetCodec_(pnc), 
  src_(0)
{
  
  
  pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
						    &NetDataWave::timeUpdate)); 
  
  pNetCodec_->getDSPStateProxy(src_); 
  pNetCodec_->getDSPStateProxy(src_).acqdatasrc.gain().connect
    (sigc::mem_fun(*this, &NetDataWave::gainFilter)); 
  reconnectPropertyProxies(); 

}

void NetDataWave::gainFilter(int chan, int gain) 
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


NetDataWave::~NetDataWave()
{

}

bool NetDataWave::setSrc(datasource_t src)
{
  if (src == src_) {
    return true; 
  } else { 
    //setSource(src);  FIXME
    
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

void NetDataWave::setSource(datasource_t src)
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

  gain.resetProxy(sigc::bind(sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc,
				&AcqDataSource::setGain), CONTCHANNEL), 
 		  sigc::bind(sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc, 
				&AcqDataSource::getGain), CONTCHANNEL), 
 		  gainSignal_); 
    


}
