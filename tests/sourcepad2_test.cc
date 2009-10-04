#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "elements/sourcepad.h"


BOOST_AUTO_TEST_SUITE(SourcePad2Test); 

using namespace elements; 

BOOST_AUTO_TEST_CASE(simpletest)
{


  SourcePad<int> * pSourcePad = SourcePad<int>::createPad(data, "testpad"); 
  
  BOOST_CHECK_EQUAL(pSourcePad->getName(), "testpad"); 
  
  // now make sure we can add/invalidate data when there's no one connected
  pSourcePad->newData(); 
  pSourcePad->invalidateData(); 


  
}

BOOST_AUTO_TEST_SUITE_END(); 
