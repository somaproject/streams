#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "streampipeline.h" 

#include "pipelinemanager.h"
#include "pipelineconfig.h"
#include "util.h"

BOOST_AUTO_TEST_SUITE(configuration); 

BOOST_AUTO_TEST_CASE(pipelinemanager)
{

  pSourceState_t ss(new SourceState); 
  ss->timer = Timer::createDummyTimer(0); 

  pPipelineManager_t pm(new PipelineManager(create_tempdir())); 
  pm->pSourceState_ = ss; 

  std::string cfgstr = "[ "
    "{ \"name\" : \"pipeline01\","
    "\"elements\" : { "
          "\"first\" : {\"type\" : \"source\","
           "            \"class\" : \"NoiseWave\"}, "
          "\"second\" : {\"type\" : \"vis\","
          "            \"class\" : \"WaveVis\"}}, "
    "\"connections\" : [[\"first\", \"default\", \"second\", \"default\"]]" // testing"
    " }]"; 

  std::cout << cfgstr << std::endl;
  std::istringstream is(cfgstr );

  load_pipeline_config(pm, is); 
  BOOST_CHECK_EQUAL(pm->size(), 1); 
  pStreamPipeline_t pl = pm->getPipeline("pipeline01"); 
  
  std::list<pISource_t> sources = pl->getSources(); 
  BOOST_CHECK_EQUAL(sources.size(), 1); 
  
}



BOOST_AUTO_TEST_SUITE_END(); 
