#include "netdatawave.h"
#include <boost/format.hpp>


const std::string NetDataWave::TYPENAME = "NetDataWave"; 
using namespace somadspio; 

NetDataWave::NetDataWave(std::string name, pTimer_t timer, 
			 pNetworkDataCache_t ndc,
			 pISomaNetCodec_t pnc, bf::path scratch) :
  SourceBase(name), 
  pTimer_(timer), 
  pNetDataCache_(ndc), 
  pSourcePad_(createSourcePad<WaveBuffer_t>("default")), 
  pNetCodec_(pnc), 
  src(1), 
  srcnotify_(new netdatawave::PropertyNotify()), 
  // acqdatasource properties: 
  gain(0), 
  hpfen(false), 
  selchan(0), 
  // dsp proxy properties: 
  sampratenum(std::make_pair(0, 0)), 
  filterid(0)
{

  src.add_watcher(srcnotify_); 
  
//   pNetCodec_->getDSPStateProxy(src_); 

//   src.signal().connect(sigc::mem_fun(*this, 
// 				     &NetDataWave::setSrc)); 
//   reconnectSource(src_); 

  srcnotify_->signal().connect(sigc::mem_fun(*this, 
					    &NetDataWave::on_src_change)); 


  datasource_t newsrc = src.get_req_value(); 
  pDataQueueView_ = pNetDataCache_->getNetWaveSource(newsrc); 
  pSourcePad_->reset(); 
  src.set_value(newsrc); // will then trigger out-of-band signal
  Glib::signal_timeout().connect(sigc::mem_fun(*this, 
					       &NetDataWave::on_check_property_change), 
				 150); 
  
}

bool NetDataWave::on_check_property_change()
{
  if(gain.pendingRequest()) {
    if(!gain_pending_req_known_) { 
      std::cout << " check property change: attempting to set to " << gain.get_req_value()
		<< " while current val = " << gain << std::endl; 
      // unknown, so we should attempt to set the property 
      pNetCodec_->getDSPStateProxy(src).acqdatasrc.setGain(CONTCHAN, gain.get_req_value()); 
      gain_pending_req_known_ = true; 
    }
  }
  if(hpfen.pendingRequest()) {
    if(!hpfen_pending_req_known_) { 
      // unknown, so we should attempt to set the property 
      pNetCodec_->getDSPStateProxy(src).acqdatasrc.setHPFen(CONTCHAN, hpfen.get_req_value()); 
      hpfen_pending_req_known_ = true; 
    }
  }
  return true; 
}



NetDataWave::~NetDataWave()
{

}

void NetDataWave::on_src_change()
{

  reconnectSource(src);    

}

void NetDataWave::process(elements::timeid_t tid)
{
  /* 
     WARNING: IS CALLED FROM OTHER THREADS. 

     The only property we pay attention to here is the switched-sources
     property. That process is as follows: 

     1. is there a source switch request? if so: 
     2. connect to the different queue, reset, and start sending downstream data
     3. change the source property value, which will then (in the main loop) cause an "other properties" reconnect
     
  */ 

  if (src.pendingRequest() ) { 
    datasource_t newsrc = src.get_req_value(); 
    pDataQueueView_ = pNetDataCache_->getNetWaveSource(newsrc); 
    pSourcePad_->reset(); 

    src.set_value(newsrc); // will then trigger out-of-band signal

  } 

  int MAXCNT = 30; 
  int cnt = 0; 
  while (cnt < MAXCNT) {
    if (!pDataQueueView_->empty()) { 
      // send the datum
//       pSourcePad_->newData(lasttime_, lasttime_+ newdata.first, 
// 			   *(sb->data())); // FIXME double-copy? 
      WaveBuffer_t  wb = pDataQueueView_->front(); 
      pSourcePad_->newData(wb.time, wb.time + timeid_t((1./wb.samprate)*wb.data.size()*1e9), 
			   wb); 
      pDataQueueView_->pop(); 
    } else {
      break; 
    }
    cnt++; 
  }
  

}

void NetDataWave::reconnectSource(datasource_t src)
{
  /*
    Called when the underlying source we're connected to changes. It
    necessitates a reconnection to all of the somadspio sources. 
    
  */ 
  if(gainconn_) 
    gainconn_.disconnect(); 
  
  gainconn_ = pNetCodec_->getDSPStateProxy(src).acqdatasrc.gain().connect(
									  sigc::mem_fun(*this, 
											&NetDataWave::on_gain_update)); 
  
  if(hpfenconn_) 
    hpfenconn_.disconnect(); 
  
  hpfenconn_ = pNetCodec_->getDSPStateProxy(src).acqdatasrc.hpfen().connect(
									  sigc::mem_fun(*this, 
											&NetDataWave::on_hpfen_update)); 
  
  gain_pending_req_known_ = false; 
  hpfen_pending_req_known_ = false; 

  gain.set_value(pNetCodec_->getDSPStateProxy(src).acqdatasrc.getGain(CONTCHAN)); 

//   sigc::mem_fun(pNetCodec_->getDSPStateProxy(src_).acqdatasrc,
// 		&AcqDataSource::setGain); 


}

void NetDataWave::on_gain_update(int x, int y) {
  if (x == CONTCHAN) { 
    std::cout << "Setting gain update to " << y << std::endl;
    gain_pending_req_known_ = false; 
    gain.set_value(y); 
  }
}

void NetDataWave::on_hpfen_update(int x, bool y) {
  if (x == CONTCHAN) { 
    std::cout << "Setting hpfen update to " << y << std::endl;
    hpfen_pending_req_known_ = false; 
    hpfen.set_value(y); 
  }
}

void NetDataWave::reconnectPropertyProxies()
{

  std::cout << "NetDataWave::reconnectPropertyProxies()" << std::endl; 

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