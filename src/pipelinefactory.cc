#include "pipelinefactory.h"

using namespace soma::streams; 

PipelineFactory::PipelineFactory(ISourceVisitor & isv, IFilterVisitor &ifv, 
				 ITriggerVisitor & itv,  IVisVisitor & ivv) :
  sourceVisitor_(isv),
  filterVisitor_(ifv), 
  triggerVisitor_(itv), 
  visVisitor_(ivv)
{


}

pStreamPipeline_t PipelineFactory::create(std::string name)
{
  


}
