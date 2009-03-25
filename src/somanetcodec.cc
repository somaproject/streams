#include "somanetcodec.h"

SomaNetCodec::SomaNetCodec(pNetworkInterface_t pni) :
  pNetwork_(pni)
{
  
  Glib::signal_io().connect(sigc::mem_fun(*this, &SomaNetCodec::dataRXCallback), 
			    pNetwork_->getDataFifoPipe(), Glib::IO_IN); 

  Glib::signal_io().connect(sigc::mem_fun(*this, &SomaNetCodec::eventRXCallback), 
			    pNetwork_->getEventFifoPipe(), Glib::IO_IN); 
  // create the data wave codecs
  for(int i = 0; i < 64; i++) { // FIXME : extract out constant
    dspStateProxies_.push_back(new dspiolib::StateProxy(datasource_t(i), 
							sigc::mem_fun(*this, 
								      &SomaNetCodec::sendEvent))); 
  }
  std::cout << "constructor this = " << this << std::endl; 

}

dspiolib::StateProxy & SomaNetCodec::getDSPStateProxy(datasource_t src)
{
  assert(src < 64); 
  return dspStateProxies_[src]; 

}

void SomaNetCodec::sendEvent(const EventTX_t & evt)
{
  EventTXList_t el;
  el.push_back(evt); 
  pNetwork_->sendEvents(el); 

}

bool SomaNetCodec::dataRXCallback(Glib::IOCondition io_condition)
{
  if ((io_condition & Glib::IO_IN) == 0) {
    std::cerr << "Invalid fifo response" << std::endl;
    return false; 
  }
  else 
    {
      std::cout << "HERE 1.5 this = " << this << std::endl; 
      char x; 
      read(pNetwork_->getDataFifoPipe(), &x, 1); 
      std::cout << "HERE 2" << std::endl; 
      pDataPacket_t rdp = pNetwork_->getNewData(); 
      std::cout << "HERE 3" << std::endl; 
      
      newDataSignal_.emit(rdp); 
      std::cout << "HERE 4" << std::endl; 
      
    }
  return true; 


}

sigc::signal<void, pDataPacket_t> & SomaNetCodec::newDataSignal()
{

  return newDataSignal_; 

}

sigc::signal<void, somatime_t> & SomaNetCodec::signalTimeUpdate()
{
  return  signalTimeUpdate_;

} 


void SomaNetCodec::enableDataRX(datasource_t src, datatype_t typ)
{

  pNetwork_->enableDataRX(src, typ); 

}

bool SomaNetCodec::eventRXCallback(Glib::IOCondition io_condition)
{
  
  if ((io_condition & Glib::IO_IN) == 0) {
    std::cerr << "Invalid fifo response" << std::endl;
    return false; 
  }
  else 
    {
      char x; 
      read(pNetwork_->getEventFifoPipe(), &x, 1); 
      pEventList_t pel = pNetwork_->getNewEvents()->events; 
      processNewEvents(pel); 
    }
  return true; 
}


void SomaNetCodec::parseEvent(const Event_t & evt)
{

  if (evt.src == 0x00 && evt.cmd == 0x10 )
    {
      // this is the time
      somatime_t stime = 0; 
      stime = evt.data[0]; 
      stime = stime << 16; 
      stime |= evt.data[1]; 
      stime = stime << 16; 
      stime |= evt.data[2]; 
      signalTimeUpdate_.emit(stime); 
    } 
  
}


void SomaNetCodec::processNewEvents(pEventList_t pEventList)
{
  EventList_t::iterator pe; 
  for (pe = pEventList->begin(); pe != pEventList->end(); pe++) {
   
      parseEvent(*pe); 
  }
        
}

