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

  
  BOOST_CHECK_EQUAL(raw.time, raw0->front().time); 

  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_CLOSE(float(raw.data[i])/1e9, (double)(raw0->front().data[i]), 0.00001); 
  }
  
  
  for(int i = 0; i < RAWBUF_LEN; i++) {
    BOOST_CHECK_CLOSE(raw.data[i]/1e9,(double) raw0->front().data[i], 0.0001); 
  }
}

BOOST_AUTO_TEST_CASE(benchmark)
{
  /* A simple benchmark that should take between 5 and 10 seconds
     to run

  */
  
  pMockSomaNetCodec_t snc(new MockSomaNetCodec()); 
  
  pTimer_t timer = Timer::createNetworkTimer(snc, 0); 
  
  bf::path scratchdir("/tmp"); 
  NetworkDataCache ndc(snc, timer, scratchdir); 
  
  
  core::IQueueView<WaveBuffer_t >::ptr raw0(ndc.getNetRawSource(0)); 
  core::IQueueView<WaveBuffer_t >::ptr raw1(ndc.getNetRawSource(0)); 

  timeval t1, t2; 
  gettimeofday(&t1, NULL); 
    
  int ITERS = 1000000; 
  for (int iter = 0; iter < ITERS; iter++) { 
    Raw_t raw; 
    raw.src = 0;
    raw.time = iter; 
    for(int i = 0; i < RAWBUF_LEN; i++) {
      raw.data[i] =  i * 0x1234; 
    }
    
    pDataPacket_t pdp = rawFromRaw(raw); 
  
    snc->newData(pdp); 
  }


  
  int time = 0; 
  while(!raw0->empty()) { 
    BOOST_CHECK_CLOSE(double(raw0->front().time), double(time)/5e4, 1e-3); 
    raw0->pop(); 
    time++; 
  }; 

  time = 0; 
  while(!raw0->empty()) { 
    BOOST_CHECK_CLOSE(double(raw1->front().time), double(time)/5e4, 1e-3); 
    raw1->pop(); 
    time++; 
  }; 
  
  gettimeofday(&t2, NULL); 
  double elapsed = (t2.tv_sec - t1.tv_sec)*1000000 + (t2.tv_usec - t1.tv_usec); 
  
  std::cout << "rec access took " << elapsed/1e6 << " seconds" << std::endl; 
    

}


BOOST_AUTO_TEST_SUITE_END(); 
