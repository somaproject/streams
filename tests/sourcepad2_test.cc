#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "elements/sourcepad.h"


BOOST_AUTO_TEST_SUITE(SourcePad2Test); 

using namespace elements; 

struct TestSource: public IElementSource<int>
{
  
  void get_src_data(std::list<int> &, padid_t, const timewindow_t &)  {
    

  }


};

BOOST_AUTO_TEST_CASE(simpletest)
{
  typedef SourcePad<int> SP; 
  TestSource tc; 

  SP::pSourcePad_t sp = SP::createPad(0, &tc, "Hello world"); 
  
  BOOST_CHECK_EQUAL(sp->getName(), "Hello world"); 
  
//   // now make sure we can add/invalidate data when there's no one connected
  //sp->newData(); 
  //sp->reset(); 

  
}

BOOST_AUTO_TEST_SUITE_END(); 
