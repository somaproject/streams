#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>


#include "sourcestate.h"
#include "streampipeline.h" 
#include "sourcevisitorbase.h"
#include "filtervisitorbase.h"
#include "triggervisitorbase.h"
#include "visvisitorbase.h"

#include "sources/noisewave.h"
#include "timer.h"

#include "pipelinemanager.h"


using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;



BOOST_AUTO_TEST_CASE(pipelinemanager)
{
//   pISourceVisitor_t isvisit(new SourceVisitorBase); 
//   pIVisVisitor_t ivvisit(new VisVisitorBase); 
//   pIFilterVisitor_t ifvisit(new FilterVisitorBase); 
//   pITriggerVisitor_t itvisit(new TriggerVisitorBase); 
  
//   PipelineManager pm; 
//   pSourceState_t  pss(new SourceState); 
//   pss->timer =  Timer::createDummyTimer(); 


//   pm.pSourceState_ = pss; 
//   pm.pSourceVisitor_ = isvisit; 
//   pm.pFilterVisitor_ = ifvisit; 
//   pm.pTriggerVisitor_ = itvisit; 
//   pm.pVisVisitor_ = ivvisit; 
  
//   pStreamPipeline_t sp  = pm.createPipeline("testPipeline"); 

//   pISource_t psrc = sp->createSource("NoiseWave", "sillysource"); 
//   pIVis_t pvis = sp->createVis("WaveVis", "wavevis"); 
  
//   psrc->getSourcePad("default")->connect(pvis->getSinkPad("default")); 
  
  
}
