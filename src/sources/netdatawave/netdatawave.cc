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
  pSourcePad_(createSourcePad<pWaveBuffer_t>("default", 
					    boost::bind(&NetDataWave::get_src_data, this, _1), 
					    boost::bind(&NetDataWave::get_sequence, this))),
  pNetCodec_(pnc), 
  src(1), 
  srcnotify_(new netdatawave::PropertyNotify()), 
  // acqdatasource properties: 
  gain(0), 
  hpfen(false), 
  selchan(0), 
  // dsp proxy properties: 
  sampratenum(std::make_pair(0, 0)), 
  filterid(0),
  seqid_(0)
{

  src.add_watcher(srcnotify_); 
  
//   pNetCodec_->getDSPStateProxy(src_); 

//   src.signal().connect(sigc::mem_fun(*this, 
// 				     &NetDataWave::setSrc)); 
//   reconnectSource(src_); 

  srcnotify_->signal().connect(sigc::mem_fun(*this, 
					    &NetDataWave::on_src_change)); 


  datasource_t newsrc = src.get_req_value(); 
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
  if(selchan.pendingRequest()) {
    if(!selchan_pending_req_known_) { 
      // unknown, so we should attempt to set the property 
      pNetCodec_->getDSPStateProxy(src).acqdatasrc.setChanSel(selchan.get_req_value()); 
      selchan_pending_req_known_ = true; 
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
    src.set_value(newsrc); // will then trigger out-of-band signal
    seqid_++; 
  } 

  

}

void NetDataWave::reconnectSource(datasource_t src)
{
  /*
    Called when the underlying source we're connected to changes. It
    necessitates a reconnection to all of the somadspio sources. 
    g
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
  
  if(selchanconn_) 
    selchanconn_.disconnect(); 
  
  selchanconn_ = pNetCodec_->getDSPStateProxy(src).acqdatasrc.chansel().connect(
									  sigc::mem_fun(*this, 
											&NetDataWave::on_selchan_update)); 
  
  gain_pending_req_known_ = false; 
  hpfen_pending_req_known_ = false; 
  selchan_pending_req_known_ = false; 

  gain.set_value(pNetCodec_->getDSPStateProxy(src).acqdatasrc.getGain(CONTCHAN)); 

  hpfen.set_value(pNetCodec_->getDSPStateProxy(src).acqdatasrc.getHPFen(CONTCHAN)); 


  selchan.set_value(pNetCodec_->getDSPStateProxy(src).acqdatasrc.getChanSel()); 

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

void NetDataWave::on_selchan_update(int x) {
  std::cout << "Setting selchan update to " << x << std::endl;
  selchan_pending_req_known_ = false; 
  selchan.set_value(x); 
  
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


elements::datawindow_t<pWaveBuffer_t> NetDataWave::get_src_data(const elements::timewindow_t & tw)
{
  datasource_t src_now = src; // atomic get of source
  // now just get the data
  return pNetDataCache_->getNetWaveData(src_now, tw); 
  
}

size_t NetDataWave::get_sequence()
{
  return seqid_; 

}

