#include <iostream>
#include <boost/assign/std/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <elements/timecache.h>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;



BOOST_AUTO_TEST_SUITE(timecache_test); 

using namespace boost::assign;
using namespace elements; 

/* 
   Some simple example hashes to make it easy to verify that we're
   doing the right thing. 

 */ 

class TimeCacheHashPolicy_Test
{
public:
  // example time cache hash
  typedef uint64_t datahash_t; 
  typedef uint64_t levelhash_t; 
  

  class hashcombine_data {
  public:
    hashcombine_data() :
      dh_(NONE)
    {
      
    }
    
    void combine(timeid_t tid, datahash_t d) {
      dh_ += (tid << 32) | (d && 0xFFFFFFFF); 
    }

    levelhash_t value() {
      return dh_; 
    }

    static const datahash_t NONE = 0; 
  private:
    levelhash_t dh_; 
  }; 
  
  class hashcombine_level {
  public:
    hashcombine_level() :
      lh_(NONE)
    {


    }

    void combine(bucketid_t lid, datahash_t d) {
      lh_ += d;
    }

    levelhash_t value() {
      return lh_; 
    }

    static const levelhash_t NONE = 0; 
  private:
    levelhash_t lh_;
  }; 
  

}; 

typedef TimeCache<  TimeCacheHashPolicy_Test> tc_t; 


BOOST_AUTO_TEST_CASE(simpletest)
{
  int g = 1000; 
  tc_t tc(g, 4, 8); 

  timewindow_t w = tc.get_bucket_window(0, 0); 
  BOOST_CHECK_EQUAL(w.start, 0); 
  BOOST_CHECK_EQUAL(w.end, g-1); 

  
  w = tc.get_bucket_window(0, 1);
  BOOST_CHECK_EQUAL(w.start, g);
  BOOST_CHECK_EQUAL(w.end, 2*g-1);
  
  w = tc.get_bucket_window(0, -1);
  BOOST_CHECK_EQUAL(w.start, -g);
  BOOST_CHECK_EQUAL(w.end, -1);
  
  w = tc.get_bucket_window(1, 0);
  BOOST_CHECK_EQUAL(w.start, 0);
  BOOST_CHECK_EQUAL(w.end, 4*g-1);
  
  w = tc.get_bucket_window(1, 1);
  BOOST_CHECK_EQUAL(w.start, 4*g);
  BOOST_CHECK_EQUAL(w.end, 8*g-1);
  
  w = tc.get_bucket_window(1, -1);
  BOOST_CHECK_EQUAL(w.start, -4*g);
  BOOST_CHECK_EQUAL(w.end, -1);
    
  BOOST_CHECK_EQUAL(tc.get_bucket(0, -1), -1); 

}

BOOST_AUTO_TEST_CASE(test_sub_bins)
{
  
  
  int g = 1000; 
  int l = 4; 
  int m = 8; 
  tc_t tc(g, l, m); 
  
  std::list<bucketid_t> bid; 
  bid += 0, 1, 2, 3; 
  
  BOOST_CHECK(tc.get_sub_buckets(1, 0) == bid); 
  bid.clear(); 
  bid += 4, 5, 6, 7; 
  BOOST_CHECK(tc.get_sub_buckets(1, 1) == bid); 
  BOOST_CHECK(tc.get_sub_buckets(2, 1) == bid); 
  
  bid.clear(); 
  bid += -4, -3, -2, -1; 
  BOOST_CHECK(tc.get_sub_buckets(1, -1) == bid); 
  

}

BOOST_AUTO_TEST_CASE(test_data_in_range)
{
  int g = 1000; 
  int l = 4; 
  int m = 8; 
  tc_t tc(g, l, m); 
  
  tc.insert(10, 1); 
  
  std::list<timeid_t> ans, ans_empty; 
  ans += 10; 
  
  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(0, 9))
	      == ans_empty);
  
  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(8, 9))
	      == ans_empty);

  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(8, 10))
	      == ans);
  
  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(9, 10))
	      == ans);

  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(9, 11))
	      == ans);
  
  BOOST_CHECK(tc.get_data_times_in_window(timewindow_t(10, 11))
	      == ans);
  
  
    
  

}

BOOST_AUTO_TEST_CASE(test_hash_simple)
{

  int g = 1000; 
  int l = 4; 
  int m = 8; 
  tc_t tc(g, l, m); 
  
  
  BOOST_CHECK_EQUAL(tc.get_hash(0, 0), 0);
  //TimeCacheHashPolicy_Test::hashcombine_level::NONE); 
  BOOST_CHECK_EQUAL(tc.get_hash(3, 0), 0); 
		    //TimeCacheHashPolicy_Test::hashcombine_level::NONE); 

  tc.insert(10, 1); 

  BOOST_CHECK_EQUAL(tc.get_hash(0, 0), (10L << 32) | 1); 
  BOOST_CHECK_EQUAL(tc.get_hash(1, 0), (10L << 32) | 1); 


}


BOOST_AUTO_TEST_SUITE_END(); 
