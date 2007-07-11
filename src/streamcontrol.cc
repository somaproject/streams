#include "streamcontrol.h"

#include "wavestreamsource.h"

StreamControl::StreamControl(NetworkInterface * pni) :
  pNetworkInterface_(pni)
{
  // do not cross the streams
  // streamcontrol also handles the network interface? I'm not
  // sure I like that...
  
  
}

StreamControl::~StreamControl()
{
  
   
  
}

streamSourcePtr_t StreamControl::newSourceFactory(std::string name, 
						  datasource_t ds, 
						  datatype_t dt){


  streamSourcePtr_t x; 
  
  if (name == "wave")
    {
      x = streamSourcePtr_t(new WaveStreamSource(ds, dt)); 
    }

  if (x) {
    datapair_t dp(ds, dt); 

    if (dataDispatchMap_.find(dp) != dataDispatchMap_.end() )
      {
	throw std::runtime_error("There is already a data source attched to that data pair"); 
      }
    
    dataDispatchMap_[dp] = x; 
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
