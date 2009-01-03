#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "core/queueview.h" 
#include "core/sinkpad.h"
#include "core/sourcepad.h"

using namespace core; 
BOOST_AUTO_TEST_SUITE(sourcesink_test); 

BOOST_AUTO_TEST_CASE(simpletest)
{

  std::list<int> data; 
  SinkPad<int> * pSinkPad = SinkPad<int>::createPad("testpad"); 
  BOOST_CHECK_EQUAL(pSinkPad->getName(), "testpad"); 


  std::list<int> data; 

  SourcePad<int> * pSourcePad = SourcePad<int>::createPad(data, "testpad"); 
  
  BOOST_CHECK_EQUAL(pSourcePad->getName(), "testpad"); 
  


}


BOOST_AUTO_TEST_SUITE_END(); 
