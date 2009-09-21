#ifndef SOURCEBASE_H
#define SOURCEBASE_H

#include <boost/shared_ptr.hpp>
#include <list>

#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include "forward.h"

#include "core/element.h"
#include "isource.h"
#include "isourcevisitor.h"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem; 

class SourceBase : public core::Element, public ISource
{
public:
      
  // name
  static const std::string TYPENAME; 
  
  // this is just a prototype source

  SourceBase(std::string name); 
  
  virtual ~SourceBase(); 
  // Output hook-up
  
//   virtual newDataSignal_t newDataSignal() { return newDataSignal_;  }; 
//   virtual invalidateDataSignal_t invalidateDataSignal() { return invalidateDataSignal_; };
//   QueueView<pBuffer_t> getQueueView() { 
//     return QueueView<pBuffer_t>(data_); 
//   }
      
  
  float lastT_; 
//   destroySignal_t & destroySignal() { return destroySignal_;}
  
//   virtual void destroy() {
//     destroySignal_.emit(); 
//   }
   
//   virtual void setPipeline(StreamPipeline* pipeline) {
//     pPipeline_ = pipeline; 
//   }
  
//   virtual StreamPipeline * getPipeline() {
//     return pPipeline_; 
//   }
  
      
protected:
  //destroySignal_t destroySignal_; 
  
  // StreamPipeline * pPipeline_; 
  
};
#endif // SOURCEBASE_H

