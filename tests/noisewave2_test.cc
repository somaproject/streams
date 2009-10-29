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

  elements::SinkPad<pWaveBuffer_t>::pSinkPad_t sink = 
    elements::SinkPad<pWaveBuffer_t>::createPad("default");
  
    
  nw2.getSourcePad("default")->connect(sink); 
  int ITERS = 10000; 
  std::vector<datawindow_t<pWaveBuffer_t> > wbs; 
  wbs.reserve(ITERS + 2); 

  wbs.push_back(sink->get_src_data(timewindow_t(-1000, 100000000000))); // all data? 
  std::cout << "now going to add data" << std::endl; 
  size_t stepsize = 1000000; 

  for (int i = 0; i < ITERS; i++) {
    nw2.process(i * stepsize); 
    wbs.push_back(sink->get_src_data(timewindow_t(-1000, 100000000000))); // all data? 
    std::cout << i << std::endl;
  }
  
  for (int i = 0; i < wbs.size(); i++) {
    // FIXME : check that each result is the perfect subset of the previous results
    
  }
  
}

BOOST_AUTO_TEST_SUITE_END(); 

