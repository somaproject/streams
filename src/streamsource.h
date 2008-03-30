#ifndef STREAMSOURCE_H
#define STREAMSOURCE_H

#include <somanetwork/network.h>

#include <boost/shared_ptr.hpp>
#include <list>

#include "streams.h"

#include <vector>
#include <sigc++/sigc++.h>
#include <somanetwork/wave.h>
#include <boost/shared_ptr.hpp>

#include "queueview.h"

#include "streamsourcebase.h"

template <class buffer_t>
class StreamSource : public StreamSourceBase
{
public:
  
  typedef buffer_t Buffer_t; 
  typedef boost::shared_ptr<buffer_t> pBuffer_t; 
  
  // this is just a prototype source
  StreamSource()   : lastT_(0.0){
  } 
  
  virtual ~StreamSource() {}; 
  
  
  // public data access
  
  QueueView<pBuffer_t> getQueueView() { 
    return QueueView<pBuffer_t>(data_); 
  }
  
  float lastT_; 
  
protected:
  std::list<pBuffer_t> data_; 
  
};


#endif // STREAMSOURCE_H

