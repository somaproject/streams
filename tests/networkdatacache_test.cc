#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <somanetwork/network.h>
#include <somanetwork/fakenetwork.h>
#include <core/queueview.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <data/wave.h>

#include "mocksomanetcodec.h"
#include "networkdatacache.h"
#include "timer.h"

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;


BOOST_AUTO_TEST_SUITE(networkdatacache);

BOOST_AUTO_TEST_CASE(create)
{
  pMockSomaNetCodec_t snc(new MockSomaNetCodec()); 
  
  pTimer_t timer = Timer::createNetworkTimer(snc, 0); 
  
  bf::path scratchdir("/tmp"); 
  NetworkDataCache ndc(snc, timer, scratchdir); 
  
  
  core::IQueueView<WaveBuffer_t >::ptr raw0(ndc.getNetRawSource(0)); 
  core::IQueueView<WaveBuffer_t >::ptr raw1(ndc.getNetRawSource(0)); 

  Raw_t raw; 
  raw.src = 0;
  raw.time = 0x00; 
  for(int i = 0; i < RAWBUF_LEN; i++) {
    raw.data[i] =  i * 0x1234; 
  }

  pDataPacket_t pdp = rawFromRaw(raw); 
  
  snc->newData(pdp); 
  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_CLOSE(float(raw.data[i])/1e9, (double)(raw0->front().data[i]), 0.00001); 
  }
  
  
  WaveBuffer_t & wb = raw1->front(); 
  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_CLOSE(raw.data[i]/1e9,(double) wb.data[i], 0.0001); 
  }
}


BOOST_AUTO_TEST_SUITE_END(); 
