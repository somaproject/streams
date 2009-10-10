#include <iostream>
#include <list>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/assign/list_of.hpp>

#include <vis/spectvis/spectvis.h>
#include <vis/spectvis/data.h>
#include <vis/spectvis/helper.h>
#include <vis/spectvis/fftengine.h>

BOOST_AUTO_TEST_SUITE(spectvistest); 
using namespace boost::assign;

using namespace spectvis; 
BOOST_AUTO_TEST_CASE(helper_get_bin_bounds)
{
  typedef std::pair<timeid_t, timeid_t> range_t; 
  
  range_t rid = get_bin_bounds(0, 10000, 1); 
  BOOST_CHECK_EQUAL(rid.first, 0); 
  BOOST_CHECK_EQUAL(rid.second, 10000 -1); 

  rid = get_bin_bounds(1, 10000, 1); 
  BOOST_CHECK_EQUAL(rid.first, 10000); 
  BOOST_CHECK_EQUAL(rid.second, 20000 -1); 

  rid = get_bin_bounds(-1, 10000, 1); 
  BOOST_CHECK_EQUAL(rid.first, -10000); 
  BOOST_CHECK_EQUAL(rid.second, -1); 

  rid = get_bin_bounds(0, 10000, 5); 
  BOOST_CHECK_EQUAL(rid.first, 0); 
  BOOST_CHECK_EQUAL(rid.second, 10000-1); 

  rid = get_bin_bounds(1, 10000, 5); 
  BOOST_CHECK_EQUAL(rid.first, 2000); 
  BOOST_CHECK_EQUAL(rid.second, 12000-1); 

  
}

bool list_equal(bufferlist_t a, bufferlist_t b) {
  BOOST_CHECK_EQUAL(a.size(), b.size()); 
  if (a.size() == b.size()) { 
    bufferlist_t::iterator ai, bi; 
    ai = a.begin(); 
    bi = b.begin();
    
    while (ai != a.end() and bi != b.end()) {
      BOOST_CHECK_EQUAL(*ai, *bi); 
      ai++; 
      bi++; 

    }
  }
}

BOOST_AUTO_TEST_CASE(helper_buf_range) {
  
  // first simple cases
  bufferlist_t bl = get_buffers_that_depend_on_times(0, 1000, 
						     10000, 1); 
  bufferlist_t er = list_of(0); 

  BOOST_CHECK_EQUAL(bl.size(), 1); 
  
  list_equal(bl, er); 

  bl = get_buffers_that_depend_on_times(0, 10000, 10000, 1); 
  er = list_of(0)(1); 
  list_equal(bl, er); 

  bl = get_buffers_that_depend_on_times(-1, 10000, 10000, 1); 
  er = list_of(-1)(0)(1); 
  list_equal(bl, er); 

  bl = get_buffers_that_depend_on_times(-1, 10000, 10000, 2); 
  er = list_of(-2)(-1)(0)(1)(2); 
  list_equal(bl, er); 

  

}

BOOST_AUTO_TEST_CASE(helper_range_query)
{
  using namespace spectvis; 
  
  RangeQuery<int> rq; 
  
  rq.insert(0, 0, 10); 
  BOOST_CHECK_EQUAL(rq.query(-10, 2).size(), 1); 
  BOOST_CHECK_EQUAL(rq.query(0, 2).size(), 1); 
  BOOST_CHECK_EQUAL(rq.query(4, 8).size(), 1); 

  rq.insert(1, 11, 20); 
  BOOST_CHECK_EQUAL(rq.query(-10, 2).size(), 1); 
  BOOST_CHECK_EQUAL(rq.query(0, 2).size(), 1); 
  BOOST_CHECK_EQUAL(rq.query(4, 8).size(), 1); 
  BOOST_CHECK_EQUAL(rq.query(8, 12).size(), 2); 

  BOOST_CHECK_EQUAL(rq.query(30, 40).size(), 0); 

}

pFFT_t identity(float * data, int data_size, int N, float fs)
{
  /* 
     Identity operation that just copies the data
     
  */ 
  pFFT_t y(new FFT); 
  y->data.reserve(data_size); 
  for (int i = 0; i < data_size; i++) { 
    y->data.push_back(data[i]); 
  }
  return y; 
}


BOOST_AUTO_TEST_CASE(fft_engine_1)
{
  typedef std::list<pFFT_t> fftlist_t; 
  FFTEngine fft(identity); 
//   fft.set_fftN(128); 
//   fft.set_windowsize(10000); 
//   fft.set_overlapFactor(1); 

//   // Now, the null test
//   fftlist_t fl = fft.getFFT(0, 10000); 
//   BOOST_CHECK_EQUAL(fl.size(), 0); 

  
}


BOOST_AUTO_TEST_SUITE_END(); 
