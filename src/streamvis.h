#ifndef STREAMVIS_H
#define STREAMVIS_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>

#include "streamsourcebase.h"
#include "streamvisbase.h"
#include "streamsource.h"
#include "wave.h"
#include "queueview.h"
#include "streams.h"



template<class buffer_t>
class StreamVis : public StreamVisBase
{
  typedef buffer_t Buffer_t; 
  typedef boost::shared_ptr<buffer_t> pBuffer_t; 

protected: 
  boost::shared_ptr<QueueView<pBuffer_t> > pInDataQueue_; 
  
public: 
  void connectSource(boost::shared_ptr<StreamSourceBase> ssb) {
    
    connectSource(boost::dynamic_pointer_cast<StreamSource<buffer_t> >(ssb)); 
    
  }

  void connectSource(boost::shared_ptr<StreamSource<buffer_t> > ss) {
    boost::shared_ptr<QueueView<pBuffer_t> > nsp(new QueueView<pBuffer_t>(ss->getQueueView())); 
    pInDataQueue_ = nsp; //.reset(new QueueView<pBuffer_t>(ss->getQueueView())); 
    
    ss->newDataSignal().connect(sigc::mem_fun(*this, &StreamVis<buffer_t>::newData)); 
    ss->invalidateDataSignal().connect(sigc::mem_fun(*this, &StreamVis<buffer_t>::invalidateData)); 
  }
  
}; 



#endif // STREAMVIS_H
