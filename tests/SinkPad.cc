#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "core/queueview.h" 
#include "core/sinkpad.h"

using namespace core; 
BOOST_AUTO_TEST_SUITE(SinkPadTest); 

BOOST_AUTO_TEST_CASE(simpletest)
{

  std::list<int> data; 
  SinkPad<int> * pSinkPad = SinkPad<int>::createPad("testpad"); 
  BOOST_CHECK_EQUAL(pSinkPad->getName(), "testpad"); 

}


BOOST_AUTO_TEST_SUITE_END(); 
