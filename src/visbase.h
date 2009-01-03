#ifndef VISBASE_H
#define VISBASE_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>

#include "core/element.h"
#include "ivis.h"
#include "ivisvisitor.h"


class VisBase : public core::Element, public IVis
{
//         typedef boost::shared_ptr<buffer_t> pBuffer_t; 

//     protected: 
//       boost::shared_ptr<QueueView<pBuffer_t> > pInDataQueue_; 
  
//     public: 
public: 
  VisBase(std::string name) : Element(name) {}

  //       // implemented methods
  invStreamRenderSignal_t & invStreamRenderSignal() 
  {
    return invStreamRenderSignal_; 
  }
  
  invTriggerRenderSignal_t & invTriggerRenderSignal()
  {
    return invTriggerRenderSignal_; 
  }

// //       void connectSource(boost::shared_ptr<StreamSourceBase> ssb) {
    
// // 	connectSource(boost::dynamic_pointer_cast<StreamSource<buffer_t> >(ssb)); 
    
// //       }

// //       void connectSource(boost::shared_ptr<StreamSource<buffer_t> > ss) {
// // 	boost::shared_ptr<QueueView<pBuffer_t> > nsp(new QueueView<pBuffer_t>(ss->getQueueView())); 
// // 	pInDataQueue_ = nsp; //.reset(new QueueView<pBuffer_t>(ss->getQueueView())); 
    
// // 	ss->newDataSignal().connect(sigc::mem_fun(*this, &StreamVis<buffer_t>::newData)); 
// // 	ss->invalidateDataSignal().connect(sigc::mem_fun(*this, &StreamVis<buffer_t>::invalidateData)); 
// //       }

//       destroySignal_t & destroySignal() { return destroySignal_;}

//       virtual void setPipeline(StreamPipeline* pipeline) {
//  	pPipeline_ = pipeline; 
//       }
      
//       virtual void destroy() {
// 	destroySignal_.emit(); 
//       }


//       virtual void connect(FilterBase<buffer_t> & source)  = 0; 
      
//       virtual StreamPipeline * getPipeline() {
// 	return pPipeline_; 
//       }

protected:
  invStreamRenderSignal_t invStreamRenderSignal_; 
  invTriggerRenderSignal_t invTriggerRenderSignal_; 
  
//       StreamPipeline * pPipeline_; 

//       destroySignal_t destroySignal_; 


//     }; 

  
}; 
#endif // VISBASE_H
