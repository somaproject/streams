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
  

  core::QueueView<WaveBuffer_t > wave0(ndc.getNetWaveSource(0)); 
  core::QueueView<WaveBuffer_t > raw0(ndc.getNetRawSource(0)); 

  Raw_t raw; 
  raw.src = 0;
  raw.time = 0x00; 
  for(int i = 0; i < RAWBUF_LEN; i++) {
    raw.data[i] =  i * 0x1234; 
  }

  pDataPacket_t pdp = rawFromRaw(raw); 
  
  snc->newData(pdp); 
  BOOST_CHECK_EQUAL(wave0.empty(), true); 
  BOOST_CHECK_EQUAL(raw0.empty(), false); 

  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_EQUAL(raw.data[i], raw0.front().data[i]); 
  }

  core::QueueView<WaveBuffer_t > raw1(ndc.getNetRawSource(0)); 
  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_EQUAL(raw.data[i], raw1.front().data[i]); 
  }

  
}


BOOST_AUTO_TEST_SUITE_END(); 
