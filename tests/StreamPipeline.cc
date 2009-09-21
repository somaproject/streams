#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "sourcestate.h"
#include "streampipeline.h" 
#include "sourcevisitorbase.h"
#include "filtervisitorbase.h"
#include "triggervisitorbase.h"
#include "visvisitorbase.h"

#include "sources/noisewave.h"
#include "timer.h"


BOOST_AUTO_TEST_CASE(streampipeline)
{
  pISourceVisitor_t isvisit(new SourceVisitorBase()); 

  pIFilterVisitor_t ifvisit(new FilterVisitorBase()); 
  pITriggerVisitor_t itvisit(new TriggerVisitorBase());  
  pIVisVisitor_t ivvisit(new VisVisitorBase()); 

  pSourceState_t  pss(new SourceState); 
  pss->timer =  Timer::createDummyTimer(); 
  
  StreamPipeline sp("stream", pss, isvisit, ifvisit, itvisit, ivvisit); 
  
  pISource_t psrc = sp.createSource("NoiseWave", "sillysource"); 
  pIVis_t pvis = sp.createVis("WaveVis", "wavevis"); 
  
  psrc->getSourcePad("default")->connect(pvis->getSinkPad("default")); 
  
  
}

// BOOST_AUTO_TEST_CASE(addvis)
// {
//   SourceVisitorDummy isvisit; 
//   FilterVisitorBase ifvisit; 
//   TriggerVisitorBase itvisit; 
//   VisVisitorBase ivvisit; 
  
//   StreamPipeline sp(isvisit, ifvisit, itvisit, ivvisit); 

//   pTimer_t pt = Timer::createDummyTimer(); 


// }
