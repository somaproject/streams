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
    dspStateProxies_.push_back(new somadspio::StateProxy(datasource_t(i), 
							sigc::mem_fun(*this, 
								      &SomaNetCodec::sendEvents), 5)); 
  }
  std::cout << "constructor this = " << this << std::endl; 

}

somadspio::StateProxy & SomaNetCodec::getDSPStateProxy(datasource_t src)
{
  assert(src < 64); 
  return dspStateProxies_[src]; 

}

void SomaNetCodec::sendEvents(const somanetwork::EventTXList_t & evt)
{
  pNetwork_->sendEvents(evt); 

}

bool SomaNetCodec::dataRXCallback(Glib::IOCondition io_condition)
{
  if ((io_condition & Glib::IO_IN) == 0) {
    std::cerr << "Invalid fifo response" << std::endl;
    return false; 
  }
  else 
    {

      char x; 
      read(pNetwork_->getDataFifoPipe(), &x, 1); 
      pDataPacket_t rdp = pNetwork_->getNewData(); 
      newDataSignal_.emit(rdp); 
      
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
  std::cout << "enable data source " << (int)src
	    << " type = " << (int) typ << std::endl;
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

