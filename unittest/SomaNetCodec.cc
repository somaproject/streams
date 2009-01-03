#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include <somanetwork/network.h>
#include <somanetwork/fakenetwork.h>
#include <core/queueview.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <data/wave.h>
#include "timer.h"
#include "somanetcodec.h"
#include "networkdatacache.h"

BOOST_AUTO_TEST_CASE(somanetcodec_create)
{
//   pNetworkInterface_t pni(new FakeNetwork()); 

//   pSomaNetCodec_t snc(new SomaNetCodec(pni)); 
//   pTimer_t timer = Timer::createNetworkTimer(snc); 
//   pNetworkDataCache_t ndc (new NetworkDataCache(snc, timer)); 

//   core::QueueView<WaveBuffer_t> qv1 = ndc->getNetWaveSource(1); 
  
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
