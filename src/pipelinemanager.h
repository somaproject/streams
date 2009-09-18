#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <boost/utility.hpp>
#include <viscontrol.h>
#include <streampipeline.h>

class PipelineManager : boost::noncopyable, 
			public VisControl
{
public: 
  PipelineManager(bf::path scratchdir); 
  
  pSourceState_t pSourceState_; 
  // dependency injection! 

  pISourceVisitor_t pSourceVisitor_; 
  pIFilterVisitor_t  pFilterVisitor_; 
  pITriggerVisitor_t pTriggerVisitor_; 
  pIVisVisitor_t  pVisVisitor_; 
  
  pStreamPipeline_t createPipeline(std::string name); 
  
  std::list<pIVis_t> getVisList();
  sigc::signal<void, pStreamPipeline_t> & pipelineCreatedSignal(); 

private:
  sigc::signal<void, pStreamPipeline_t> newPipeline_; 
  
  std::list<pStreamPipeline_t>  pipelines_; 
  bf::path scratchdir_; 

}; 

typedef boost::shared_ptr<PipelineManager> pPipelineManager_t; 

#endif // PIPELINE_MANAGER_H
