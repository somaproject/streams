#ifndef IFILTER_H
#define IFILTER_H
#include <boost/shared_ptr.hpp>
#include <map>

#include "core/ielement.h" 
#include "ifiltervisitor.h"


// this is where we typedef the relevent typedefs
class IFilter : public virtual core::IElement
{
  
//   //public:
//   typedef sigc::signal<void> newDataSignal_t; 
//   typedef sigc::signal<void> invalidateDataSignal_t; 
  
//   virtual newDataSignal_t newDataSignal() = 0; 
//   virtual invalidateDataSignal_t invalidateDataSignal() = 0; 
  
//   //virtual void setPipeline(pStreamPipeline_t pipeline) = 0; 
//   //virtual void accept(IFilterVisitor & sv) = 0; 
//   virtual void accept(IFilterVisitor & sv) = 0; 
  
  
}; 

typedef boost::shared_ptr<IFilter> pIFilter_t; 


#endif // IFILTER_H
