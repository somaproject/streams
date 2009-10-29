#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "elements/sinkpad.h"
#include "elements/sourcepad.h"

BOOST_AUTO_TEST_SUITE(sourcesink_test); 

using namespace elements; 

int globalcount = 0;  

void inccount()
{
  globalcount++; 
}

datawindow_t<int> get_data(const timewindow_t & tw) {

}

size_t get_seq()
{

}


BOOST_AUTO_TEST_CASE(simpletest)
{
  typedef SourcePad<int> source_t; 
  typedef SinkPad<int> sink_t; 
  sink_t::pSinkPad_t pSinkPad = sink_t::createPad("testpad"); 
  BOOST_CHECK_EQUAL(pSinkPad->getName(), "testpad"); 

  source_t::pSourcePad_t sp = source_t::createPad("testpad", get_data, get_seq); 

  sp->connect(pSinkPad); 
  
//   pSinkPad->newDataSignal().connect(&inccount); 
//   globalcount = 0; 


//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->empty(), true); 

//   data.push_back(new int(7)); 
//   data.push_back(new int(8)); 
//   data.push_back(new int(9)); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 1); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 2); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 3); 
  
//   // now try and access the queue view
//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->empty(), false); 
//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->front(), 7); 
//   pSinkPad->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->front(), 8); 
//   pSinkPad->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->front(), 9); 
//   pSinkPad->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad->getpQueueView()->empty(), true); 

}

// BOOST_AUTO_TEST_CASE(twosinktest)
// {


//   SinkPad<int> * pSinkPad1 = SinkPad<int>::createPad("testpad1"); 
//   SinkPad<int> * pSinkPad2 = SinkPad<int>::createPad("testpad1"); 

//   std::list<int> data; 
//   SourcePad<int> * pSourcePad = SourcePad<int>::createPad(data, "testpad"); 
  
//   pSourcePad->connect(pSinkPad1); 
//   pSourcePad->connect(pSinkPad2); 
  
//   pSinkPad1->newDataSignal().connect(&inccount); 
//   pSinkPad2->newDataSignal().connect(&inccount); 
//   globalcount = 0; 

//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->empty(), true); 
//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->empty(), true); 

//   data.push_back(new int(7)); 
//   data.push_back(new int(8)); 
//   data.push_back(new int(9)); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 2); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 4); 
//   pSourcePad->newData(); 
//   BOOST_CHECK_EQUAL(globalcount, 6); 
  

//   // now try and access the queue view
//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->empty(), false); 
//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->front(), 7); 
//   pSinkPad1->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->empty(), false); 
//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->front(), 7); 
//   pSinkPad2->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->front(), 8); 
//   pSinkPad1->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->front(), 9); 
//   pSinkPad1->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad1->getpQueueView()->empty(), true); 

//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->front(), 8); 
//   pSinkPad2->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->front(), 9); 
//   pSinkPad2->getpQueueView()->pop(); 
  
//   BOOST_CHECK_EQUAL(pSinkPad2->getpQueueView()->empty(), true); 

// }


BOOST_AUTO_TEST_SUITE_END(); 
