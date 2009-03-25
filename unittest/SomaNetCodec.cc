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

BOOST_AUTO_TEST_SUITE(somanetcodec)

using namespace somanetwork;


std::list<pDataPacket_t> data; 



Glib::RefPtr<Glib::MainLoop> ml; 

pFakeNetwork_t fn; 

void getData_1(pDataPacket_t pdp) {
  std::cout << "callback" << std::endl; 
  data.push_back(pdp); 
  if (data.size() > 5) {
    ml->quit(); 
  }
}

bool putInFakeData_1()
{
  std::cout << "Put in fake data" << std::endl; 

  // send some fake data and see if we get a response
  Raw_t rt; 
  rt.src = 0; 
  rt.time = 0x12345678; 
  rt.chansrc = 0; 
  rt.filterid = 0xAABBCC; 

  pDataPacket_t dp = rawFromRaw(rt); 
  fn->appendDataOut(dp); 
  return true; 

}


BOOST_AUTO_TEST_CASE(create)
{

  data.clear(); 

  pFakeNetwork_t fn2(new FakeNetwork()); 
  fn  = fn2; 

  pSomaNetCodec_t snc(new SomaNetCodec(fn)); 
  pTimer_t timer = Timer::createNetworkTimer(snc); 
  
  snc->enableDataRX(0, somanetcodec::RAW); 
  
  snc->newDataSignal().connect(&getData_1); 
  fn->run(); 
  

  ml = Glib::MainLoop::create(); 
  // call quit from the other thread
  Glib::signal_timeout().connect(&putInFakeData_1, 
				 10); 
  ml->run();
  BOOST_CHECK_EQUAL(data.size(), 6); 

}


BOOST_AUTO_TEST_CASE(create2)
{
  data.clear(); 
  //pNetworkInterface_t pni(fn); 
  pFakeNetwork_t fn2(new FakeNetwork()); 
  fn  = fn2; 

  pSomaNetCodec_t snc(new SomaNetCodec(fn)); 
  pTimer_t timer = Timer::createNetworkTimer(snc); 
  
  snc->enableDataRX(0, somanetcodec::RAW); 
  
  snc->newDataSignal().connect(&getData_1); 
  fn->run(); 
  

  ml = Glib::MainLoop::create(); 
  // call quit from the other thread
  Glib::signal_timeout().connect(&putInFakeData_1, 
				 10); 
  ml->run();
  BOOST_CHECK_EQUAL(data.size(), 6); 

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

BOOST_AUTO_TEST_SUITE_END(); 
