#include "streamcontrol.h"

#include "wavestreamsource.h"

StreamControl::StreamControl(NetworkInterface * pni) :
  pNetworkInterface_(pni)
{
  // do not cross the streams
  // streamcontrol also handles the network interface? I'm not
  // sure I like that...
  
  // wire up the network callbacks

  Glib::signal_io().connect(sigc::mem_fun(*this, 
					  &StreamControl::dataRXCallback), 
			    pni->getDataFifoPipe(), Glib::IO_IN); 

  Glib::signal_io().connect(sigc::mem_fun(*this, 
					  &StreamControl::eventRXCallback), 
			    pni->getEventFifoPipe(), Glib::IO_IN); 
  
  
}

StreamControl::~StreamControl()
{
  
   
  
}

streamSourcePtr_t StreamControl::newSourceFactory(std::string name, 
						  datasource_t ds){


  streamSourcePtr_t x; 
  datatype_t dt = WAVE; 
  if (name == "wave")
    {
      dt = WAVE; 
      x = streamSourcePtr_t(new WaveStreamSource(ds, WAVE)); 
    }

  if (x) {
    datapair_t dp(ds, dt); 

    if (dataDispatchMap_.find(dp) != dataDispatchMap_.end() )
      {
	throw std::runtime_error("There is already a data source attched to that data pair"); 
      }
    
    dataDispatchMap_[dp] = x; 
    pNetworkInterface_->enableDataRX(ds, dt); 
    sourceList_.push_back(x); 

    return x; 
  } else {
    throw std::range_error("Unknown source type " + name); 
  }
  
  
}

streamVisPtr_t StreamControl::newVisFactory(streamSourcePtr_t src, std::string name)
{

  streamVisPtr_t x; 
  x = src->newVisFactory(name); 

  if (x) {
    visMap_.insert(std::pair<streamSourcePtr_t, streamVisPtr_t>(src, x)); 
    visPtrList.push_back(x); 

    return x; 

  } else {

    throw std::range_error("Unknown vis type " + name); 
  }
  
  
}

void StreamControl::remove(streamSourcePtr_t source) {

  sourcePtrList_t::iterator i; 
  i = find(sourceList_.begin(), sourceList_.end(), source); 

  if (i == sourceList_.end() ) {
    throw std::runtime_error("Attempted to remove a source which was not in our list"); 
  }
  
  // delete the associated vis components
  visPtrMap_t::iterator pVis = visMap_.find(source); 
  while (pVis != visMap_.end()) {
    remove(pVis->second); 

    pVis = visMap_.find(source); 

  }

  // remove data dispatch
  for (dataDispatchMap_t::iterator ddi = dataDispatchMap_.begin(); 
       ddi !=  dataDispatchMap_.end(); ddi++) 
    {
      if (ddi->second == *i) {

	
	// delete RX from network
	pNetworkInterface_->disableDataRX(ddi->first.first, ddi->first.second); 
	
	// remvoe from map
	dataDispatchMap_.erase(ddi); 
      }
      
    }



  // delete self
  (*i)->disconnect(); 
  sourceList_.erase(i); 

  // done

}

void StreamControl::remove(streamVisPtr_t vis)
{
  visPtrMap_t::iterator v = findVis(vis); 
  vis->disconnect(); 
  visMap_.erase(v); 
  
  std::list<streamVisPtr_t>::iterator i; 
  i = find(visPtrList.begin(), visPtrList.end(), vis); 
  visPtrList.erase(i); 

}

void StreamControl::dispatch(DataPacket_t * pdp )
{
  // We dispatch inbound packets to the appropraite stream 
  // source by performing a  lookup in our map

  datapair_t dp(pdp->src, pdp->typ); 
  dataDispatchMap_t::iterator ddi = dataDispatchMap_.find(dp); 
  if (ddi != dataDispatchMap_.end() )
    {
      ddi->second->newDataPacket(pdp); 
    } else {
      std::cerr << "Received a packet with an incorrect source/id" << std::endl; 
    }

}
  
visPtrMap_t::iterator  StreamControl::findVis(streamVisPtr_t v)
{
  // this is a stupid linear-time search
  visPtrMap_t::iterator i = visMap_.begin(); 
  for (i = visMap_.begin(); i != visMap_.end(); ++i)
    {
      if (i->second == v) {
	return i; 
      }

    }
  return i; 

}


bool StreamControl::dataRXCallback(Glib::IOCondition io_condition)
{
  
  if ((io_condition & Glib::IO_IN) == 0) {
    std::cerr << "Invalid fifo response" << std::endl;
    return false; 
  }
  else 
    {
      char x; 
      read(pNetworkInterface_->getDataFifoPipe(), &x, 1); 
      DataPacket_t * rdp = pNetworkInterface_->getNewData(); 
      // is this a wave 
      if (rdp->typ == WAVE)
	{
	  dispatch(rdp); 

	}
      else 
	{
	  std::cerr << "Not a wave packet?"  << std::endl; 
	}
      delete rdp; 

    }
  return true; 
}

bool StreamControl::eventRXCallback(Glib::IOCondition io_condition)
{
  
  if ((io_condition & Glib::IO_IN) == 0) {
    std::cerr << "Invalid fifo response" << std::endl;
    return false; 
  }
  else 
    {
      char x; 
      read(pNetworkInterface_->getEventFifoPipe(), &x, 1); 
      EventList_t * pel = pNetworkInterface_->getNewEvents(); 
      
      EventList_t::iterator i; 
      for (i = pel->begin(); i != pel->end(); i++)
	{
	  // we really need to figure out how to FACTOR THIS OUT
	  
	  if (i->src == 0x00 && i->cmd == 0x10 )
	    {
	      // this is the time
	      uint64_t time = 0; 
	      time = i->data[0]; 
	      time = time << 16; 
	      time |= i->data[1]; 
	      time = time << 16; 
	      time |= i->data[2]; 
	      float ftime = float(time) / 50e3; 

	      setTime(ftime); 
	    }

	}
    }
  return true; 
}

void StreamControl::setTime(float time)
{

  timeSignal_.emit(time); 
}
