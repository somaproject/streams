#ifndef PIPELINEFACTORY_H
#define PIPELINEFACTORY_H

#include "streampipeline.h"
#include "isourcevisitor.h"
#include "ifiltervisitor.h"
#include "itriggervisitor.h"
#include "ivisvisitor.h"

namespace soma {
  namespace streams { 

    class PipelineFactory 
    {
    public:
      PipelineFactory(ISourceVisitor &, IFilterVisitor &, 
		      ITriggerVisitor &, IVisVisitor & ); 

      pStreamPipeline_t create(std::string name); 

    private:
      ISourceVisitor &  sourceVisitor_; 
      IFilterVisitor &  filterVisitor_; 
      ITriggerVisitor & triggerVisitor_; 
      IVisVisitor &  visVisitor_; 
    }; 
    
    
  }

}

#endif // PIPELINEFACTORY_H
