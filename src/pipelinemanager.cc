#include <boost/foreach.hpp>
#include "pipelinemanager.h"

PipelineManager::PipelineManager(bf::path scratchdir) :
  scratchdir_(scratchdir)
{
  
  

}

pStreamPipeline_t PipelineManager::createPipeline(std::string name)
{
  pStreamPipeline_t pipeline(new StreamPipeline(name, scratchdir_, 
						pSourceState_, 
						pSourceVisitor_, 
						pFilterVisitor_, 
						pTriggerVisitor_, 
						pVisVisitor_)); 
  pipelines_.push_back(pipeline); 
  
  newPipeline_.emit(pipeline);
  return pipeline; 

}

std::list<pIVis_t> PipelineManager::getVisList()
{
  std::list<pIVis_t> vislist; 
  for (std::list<pStreamPipeline_t>::iterator p = pipelines_.begin(); 
       p != pipelines_.end(); ++p) {
    std::list<pIVis_t> curvis = (*p)->getVis(); 

    copy(curvis.begin(), curvis.end(), std::back_inserter(vislist)); 
    
  }

  return vislist; 

}

sigc::signal<void, pStreamPipeline_t> & PipelineManager::pipelineCreatedSignal()
{
  return newPipeline_; 
}

size_t PipelineManager::size()
{
  return pipelines_.size(); 
}

pStreamPipeline_t PipelineManager::getPipeline(std::string name) 
{
  BOOST_FOREACH(pStreamPipeline_t p, pipelines_) {
    if (p->getName() == name) { 
      return p; 
    }
  }
  throw std::runtime_error("unknown pipeline"); 

}

std::list<pStreamPipeline_t> PipelineManager::getPipelines()
{
  return pipelines_; 
  
}


