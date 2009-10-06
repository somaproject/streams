#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include <elements/sinkpad.h>
#include <elements/sourcepad.h>
#include <elements/element.h>
#include <sources/noisewave2.h>



BOOST_AUTO_TEST_SUITE(noisewave2_test); 
using namespace elements; 
BOOST_AUTO_TEST_CASE(simpletest)
{
  NoiseWave2 nw2("Test", "/tmp"); 

  elements::SinkPad<WaveBuffer_t>::pSinkPad_t sink = 
    elements::SinkPad<WaveBuffer_t>::createPad("default");
  
    
  nw2.getSourcePad("default")->connect(sink); 
  size_t stepsize = 1000000; 
  for (int i = 0; i < 100000; i++) {
    nw2.process(i * stepsize); 
  }

  BOOST_CHECK_EQUAL(sink->dataqueue_.empty(), false); 
  
  
}

BOOST_AUTO_TEST_SUITE_END(); 

