#ifndef FORWARD_H
#define FORWARD_H

#include <boost/shared_ptr.hpp>

namespace soma { 
  namespace streams {
    // Forward declarations 
    class StreamPipeline; 
    typedef boost::shared_ptr<StreamPipeline> pStreamPipeline_t; 
    
    template<class T> 
    class SourceBase; 
    
  }

}

#endif // FORWARD_H
