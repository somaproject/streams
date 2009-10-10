#include <iostream>
#include <list>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/assign/list_of.hpp>

#include <vis/spectvis/spectvis.h>
#include <vis/spectvis/data.h>
#include <vis/spectvis/helper.h>
#include <vis/spectvis/fftengine.h>
#include <boost/random.hpp>

BOOST_AUTO_TEST_SUITE(spectvistest); 
using namespace boost::assign;
using namespace spectvis; 


WaveBuffer_t  create_canonical_wb(elements::timeid_t time,
				  timeid_t period, 
				  timeid_t len, 
				  int index) 
{
  /* 
     Create a canonical wavebuffer for testing
  */ 

  WaveBuffer_t wb; 
  wb.time = time; 
  wb.samprate = 1./(double(period)/1e9); 

  for (int i = 0; i < len; i++) { 
    wb.data.push_back(i + index * len); 
  }
  return wb; 
}


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

  bl = get_buffers_that_depend_on_times(200000, 299999, 1000000, 1); 
  er = list_of(0); 
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
  
  //Identity operation that just copies the data 
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
  fft.set_fftN(128); 
  fft.set_windowsize(100*elements::TIMEID_US); // 
  fft.set_overlapFactor(1); 

  
  // Now, the null test
  fftlist_t fl = fft.getFFT(0, 100*elements::TIMEID_US); // 100 uS
  BOOST_CHECK_EQUAL(fl.size(), 0); 
  
  WaveBuffer_t wb; 
  wb.time = 0; 
  wb.samprate = 1000000.0; // 1 sample = 1 uS
  for (int i = 0; i < 100; i++) { 
    wb.data.push_back(i); 
  }
  fft.appendData(wb); 
  fft.process(10); 

  fl = fft.getFFT(0, 10000); // 100 uS
  BOOST_CHECK_EQUAL(fl.size(), 1); 

}

BOOST_AUTO_TEST_CASE(fft_engine_2)
{
  /* 
     Try and assemble smaller buffers
     
  */ 
  
  typedef std::list<pFFT_t> fftlist_t; 
  FFTEngine fft(identity); 
  fft.set_fftN(128); 
  fft.set_windowsize(1*elements::TIMEID_MS); 
  fft.set_overlapFactor(1); 

  /* Add 10 1000-sample long (1 ms) 
     data elements */ 

  for (int i = 0; i < 9; i++) {
    fft.appendData(create_canonical_wb( i * 100*elements::TIMEID_US, 
					elements::TIMEID_US, 
					100, i)); 
  }
  fft.process(20); 


  fftlist_t fl; 

  fl = fft.getFFT(0, 10000); // 10 uS
  BOOST_CHECK_EQUAL(fl.size(), 0); 

  fft.appendData(create_canonical_wb( 9 * 100*elements::TIMEID_US, 
				      elements::TIMEID_US, 
				      100, 9)); 

  fft.process(20); 
  fl = fft.getFFT(0, 10000); // 10 uS
  BOOST_CHECK_EQUAL(fl.size(), 1); 
  if(fl.empty() == false) {
    pFFT_t data = fl.front(); 
    // validate the data
    for (int i = 0; i < data->data.size(); i++) {
      BOOST_CHECK_EQUAL(data->data[i], i);       
    }
  }
}



BOOST_AUTO_TEST_CASE(fft_engine_3)
{
  /* 
     Try and assemble smaller buffers, but leave one out
     
  */ 
  
  typedef std::list<pFFT_t> fftlist_t; 
  FFTEngine fft(identity); 
  fft.set_fftN(128); 
  fft.set_windowsize(1*elements::TIMEID_MS); 
  fft.set_overlapFactor(1); 

  /* Add 10 1000-sample long (1 ms) 
     data elements */ 

  for (int i = 0; i < 10; i++) {
    if (i != 6) { 
      fft.appendData(create_canonical_wb( i * 100*elements::TIMEID_US, 
					  elements::TIMEID_US, 
					  100, i)); 
    }
  }
  fft.process(20); 


  fftlist_t fl; 

  fl = fft.getFFT(0, 10000); // 10 uS
  BOOST_CHECK_EQUAL(fl.size(), 0); 

  fft.appendData(create_canonical_wb( 6 * 100*elements::TIMEID_US, 
				      elements::TIMEID_US, 
				      100, 6)); 

  fft.process(20); 
  fl = fft.getFFT(0, 10000); // 10 uS
  BOOST_CHECK_EQUAL(fl.size(), 1); 
  if(fl.empty() == false) {
    pFFT_t data = fl.front(); 
    for (int i = 0; i < data->data.size(); i++) {
      BOOST_CHECK_EQUAL(data->data[i], i);       
    }

  }
}


BOOST_AUTO_TEST_CASE(fft_engine_4)
{
  /* 
     1. Compute the data for the requested buffers. 
     2. take the data and chop it up into segments. 
     3. send the segments out of order. 
     4. make sure we reassemble correctly at the end. 
     
  */ 
  typedef std::list<pFFT_t> fftlist_t; 
  
  timeid_t period_ns = 1 * elements::TIMEID_US; 
  timeid_t windowdur = 1*elements::TIMEID_MS; 
  timeid_t windowsize = windowdur / period_ns; 
  int wincnt = 10; 

  // now chop it up into lists
  //  typedef std::vector<float> samplevect_t; 
  typedef std::vector<WaveBuffer_t> samples_t; 
  
  samples_t samples; 
  
  // insert the data into the samples
  boost::mt19937 rng;
  int minsize = 4; 
  int maxsize = 1000; 
  boost::uniform_int<> dist(minsize, maxsize);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
           die(rng, dist);             // glues randomness with mapping



  int curpos = 0; 
  timeid_t currenttime = 0; 
  for(int i = 0; i < wincnt * windowsize; i++) { 
    if (curpos == 0) { 
      curpos = die(); 
      WaveBuffer_t wb; 
      wb.time = currenttime; 
      wb.samprate = 1.0 / (double(period_ns) /1e9); 
      samples.push_back(wb); 
    }
    samples.back().data.push_back(i); 
    curpos--; 
    currenttime += period_ns; 
  }

  // now shuffle the list
  random_shuffle(samples.begin(), samples.end()); 
  

  FFTEngine fft(identity); 
  fft.set_fftN(128); 
  fft.set_windowsize(windowdur); 
  fft.set_overlapFactor(1); 
  BOOST_FOREACH(WaveBuffer_t wb, samples) { 
    fft.appendData(wb); 
  }
  fft.process(samples.size() * 4); 
  
  // now make sure we can get all of them
  fftlist_t fl = fft.getFFT(0, wincnt * windowdur); 
  BOOST_CHECK_EQUAL(fl.size(), wincnt); 
}



BOOST_AUTO_TEST_SUITE_END(); 
