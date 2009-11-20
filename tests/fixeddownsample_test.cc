#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include <elements/sinkpad.h>
#include <elements/sourcepad.h>
#include <elements/element.h>

#include <vis/wavevis2/fixeddownsample.h>

BOOST_AUTO_TEST_SUITE(fixeddownsample_test); 

using namespace wavevis2; 

BOOST_AUTO_TEST_CASE(test_getbinandpos)
{
  typedef std::pair<bufferid_t, unsigned int> pos_t; 
  
  timeid_t BINSIZE = 100000; 
  timeid_t BUFFERLEN = 100; 
  timeid_t BUFFERDUR = BINSIZE * BUFFERDUR; 
  
  timeid_t t = 0; 

  pos_t p = FixedDownSample::getBinAndPos(t, BINSIZE, BUFFERLEN); 
  BOOST_CHECK_EQUAL(p.first, 0); 
  BOOST_CHECK_EQUAL(p.second, 0); 
  

  p = FixedDownSample::getBinAndPos(1, BINSIZE, BUFFERLEN); 
  BOOST_CHECK_EQUAL(p.first, 0); 
  BOOST_CHECK_EQUAL(p.second, 0); 
  
  p = FixedDownSample::getBinAndPos(-1, BINSIZE, BUFFERLEN); 
				    
  BOOST_CHECK_EQUAL(p.first, -1); 
  BOOST_CHECK_EQUAL(p.second, BUFFERLEN-1); 
  

  p = FixedDownSample::getBinAndPos(-BINSIZE-1, BINSIZE, BUFFERLEN); 
				    
  BOOST_CHECK_EQUAL(p.first, -1); 
  BOOST_CHECK_EQUAL(p.second, BUFFERLEN-2); 
  
  p = FixedDownSample::getBinAndPos(BINSIZE-1, BINSIZE, BUFFERLEN); 
				    
  BOOST_CHECK_EQUAL(p.first, 0); 
  BOOST_CHECK_EQUAL(p.second, 0); 
  
  p = FixedDownSample::getBinAndPos(BINSIZE, BINSIZE, BUFFERLEN); 
				    
  BOOST_CHECK_EQUAL(p.first, 0); 
  BOOST_CHECK_EQUAL(p.second, 1); 
  
  
}

BOOST_AUTO_TEST_SUITE_END(); 
