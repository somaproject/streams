#ifndef STREAMPIPELINE_H
#define STREAMPIPELINE_H

#include <list>
#include <boost/filesystem.hpp>

#include "forward.h"

#include <sources/isource.h>
#include <filters/ifilter.h>
#include <triggers/itrigger.h>
#include <vis/ivis.h>

#include "sourcestate.h" 
#include <sources/isourcevisitor.h>
#include <filters/ifiltervisitor.h>
#include <triggers/itriggervisitor.h>
#include <vis/ivisvisitor.h>

#include "jsonutil.h"


namespace bf = boost::filesystem; 

class StreamPipeline {
public:
  StreamPipeline(std::string name, boost::filesystem::path scratchdir,
		 pSourceState_t sourceState, 
		 pISourceVisitor_t,  pIFilterVisitor_t, 
		 pITriggerVisitor_t, pIVisVisitor_t ); 
  
  ~StreamPipeline(); 
  
  void resetVisitors(pISourceVisitor_t, pIFilterVisitor_t, 
		     pITriggerVisitor_t, pIVisVisitor_t); 
  
  pISource_t createSource(std::string type, std::string name, 
			  const json_util::mObject &); 
  pIFilter_t createFilter(std::string type, std::string name); 
  pITrigger_t createTrigger(std::string type, std::string name); 
  pIVis_t createVis(std::string type, std::string vis); 
  
  std::list<pISource_t> getSources(); 
  std::list<pIFilter_t> getFilters(); 
  std::list<pITrigger_t> getTriggers(); 
  std::list<pIVis_t> getVis(); 
  std::string getName() {return name_;}

private:
  
  pSourceState_t pSourceState_;       

  std::list<pISource_t> sources_; 
  std::list<pIFilter_t> filters_;  
  std::list<pITrigger_t> triggers_; 
  std::list<pIVis_t> vis_; 
  
  pISourceVisitor_t sourceVisitor_; 
  pIFilterVisitor_t filterVisitor_; 
  pITriggerVisitor_t triggerVisitor_; 
  pIVisVisitor_t  visVisitor_; 
  
  void setSource(pISource_t src); 
  void addFilter(pIFilter_t filter); 
  void addTrigger(pITrigger_t trigger); 
  void addVis(pIVis_t vis); 
  
  // callbacks for destruction ! 
  void destroySource(); 

  std::string name_; 
  bf::path scratchdir_; 

}; 

typedef boost::shared_ptr<StreamPipeline> pStreamPipeline_t; 


#endif // STREAMPIPELINE_H

