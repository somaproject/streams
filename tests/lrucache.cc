#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "lrucache.h"
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;



BOOST_AUTO_TEST_SUITE(lrutest_test); 

BOOST_AUTO_TEST_CASE(simpletest)
{
  LRUCache<int, float> x; 

  BOOST_CHECK_EQUAL(x.size(), 0);   
  x.insert(1, 10.0); 

  BOOST_CHECK_EQUAL(x.size(), 1); 
 
  BOOST_CHECK(x.exists(1)); 
  
  BOOST_CHECK_EQUAL(x.pop_old(), 10.0); 
  
  BOOST_CHECK_EQUAL(x.size(), 0); 
  
  BOOST_CHECK(!x.exists(1)); 
  
}


BOOST_AUTO_TEST_CASE(simpletest_touch)
{
  LRUCache<int, float> x; 

  BOOST_CHECK_EQUAL(x.size(), 0);   
  x.insert(1, 10.0); 
  x.insert(2, 20.0); 
  x.insert(3, 30.0); 
  x.insert(4, 40.0); 


  BOOST_CHECK_EQUAL(x.size(), 4); 
 
  x.touch(4); 
  x.touch(2); 
  x.touch(3); 
  
    
  BOOST_CHECK_EQUAL(x.pop_old(), 10.0); 
  BOOST_CHECK_EQUAL(x.pop_old(), 40.0); 
  BOOST_CHECK_EQUAL(x.pop_old(), 20.0); 
  BOOST_CHECK_EQUAL(x.pop_old(), 30.0); 
  
  BOOST_CHECK_EQUAL(x.size(), 0); 
  
  
}

BOOST_AUTO_TEST_SUITE_END(); 
