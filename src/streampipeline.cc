#include <boost/format.hpp>
#include "streampipeline.h"
#include "sources/sourcefactory.h"
#include "vis/visfactory.h"


StreamPipeline::StreamPipeline(std::string name, bf::path scratchdir, 
			       pSourceState_t ss, 
			       pISourceVisitor_t isv, pIFilterVisitor_t ifv, 
			       pITriggerVisitor_t itv,  pIVisVisitor_t  ivv) :
  name_(name),
  pSourceState_(ss), 
  sourceVisitor_(isv),
  filterVisitor_(ifv), 
  triggerVisitor_(itv), 
  visVisitor_(ivv)
{
  // set scratch dir
  scratchdir_ = scratchdir / name; 
  if (bf::exists(scratchdir_)) { 
    bf::remove_all(scratchdir_); 
  }
  
  if (!bf::create_directory(scratchdir_)) { 
    throw std::runtime_error(
      boost::str(boost::format("Unable to create scratch directory %s" ) 
		 % scratchdir_.string())); 
    
  }
  

}

StreamPipeline::~StreamPipeline()
{
  
  
}

pISource_t StreamPipeline::createSource(std::string type, std::string name,
					const json_util::mObject & config)
{
  // check if name is already in use
  for (std::list<pISource_t>::iterator pps = sources_.begin(); 
       pps != sources_.end(); ++pps) {
    if ((*pps)->getName() == name) {
      throw std::runtime_error("An element named " + name
			       + " already exists in this pipeline"); 
      
    }
  }
  pISource_t src = SourceFactory::create(type, pSourceState_, scratchdir_ / "source", 
					 config); 
  src->accept(sourceVisitor_); 
  
  src->setName(name); 
  sources_.push_back(src); 
  return src; 
  
}

pIVis_t StreamPipeline::createVis(std::string type, std::string name)
{
  for (std::list<pIVis_t>::iterator pps = vis_.begin(); 
       pps != vis_.end(); ++pps) {
    if ((*pps)->getName() == name) {
      throw std::runtime_error("An element named " + name
			       + " already exists in this pipeline"); 
      
    }
  }
  pIVis_t vis = VisFactory::create(type, scratchdir_ / "vis"); 
  vis->setName(name); 

  vis->accept(visVisitor_); 

  vis_.push_back(vis); 
  return vis; 


}



std::list<pISource_t> StreamPipeline::getSources()
{
  return sources_; 
}

std::list<pIFilter_t> StreamPipeline::getFilters()
{
  
  return filters_; 
}

std::list<pITrigger_t> StreamPipeline::getTriggers()
{
  return triggers_; 
}

std::list<pIVis_t> StreamPipeline::getVis()
{
  return vis_; 
}
      
/*
  Reset the visitors to something else. This enables a gui element,
  upon being notified of a pipeline's creation, to then inject
  its own (perhaps instance-specialized) visitors into the stream
  pipeline

*/

void StreamPipeline::resetVisitors(pISourceVisitor_t sv, pIFilterVisitor_t fv, 
				   pITriggerVisitor_t tv, pIVisVisitor_t vv)
{
  sourceVisitor_ = sv; 
  filterVisitor_ = fv; 
  triggerVisitor_ = tv; 
  visVisitor_ = vv; 

}
  
