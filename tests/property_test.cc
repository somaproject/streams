#include <iostream>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <elements/property.h> 

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;


BOOST_AUTO_TEST_SUITE(property_test);

using namespace elements; 

class Notify : public IPropertyNotify
{
public:
  int notifycnt; 

  Notify() :
    notifycnt(0)
  {


  }
  void notify() {
    notifycnt++; 
  }

}; 

typedef boost::shared_ptr<Notify> pNotify_t; 


BOOST_AUTO_TEST_CASE(simpletest) {
  
  Property<int> prop(0); 

  BOOST_CHECK_EQUAL((int)prop, 0); 
  
  BOOST_CHECK_EQUAL(prop.pendingRequest(), false); 

  prop = 10; 

  BOOST_CHECK_EQUAL(prop.pendingRequest(), true); 
  
  prop.set_value(prop.get_req_value()); 
  BOOST_CHECK_EQUAL((int)prop, 10); 
  BOOST_CHECK_EQUAL(prop.pendingRequest(), false); 

  
}

BOOST_AUTO_TEST_CASE(notify) {
  
  Property<int> prop(0); 

  BOOST_CHECK_EQUAL((int)prop, 0); 
  
  BOOST_CHECK_EQUAL(prop.pendingRequest(), false); 

  pNotify_t n1(new Notify); 
  pNotify_t n2(new Notify); 
  
  size_t n1i = prop.add_watcher(n1);
  size_t n2i = prop.add_watcher(n2); 

  prop = 10; 

  BOOST_CHECK_EQUAL(prop.pendingRequest(), true); 
  
  prop.set_value(prop.get_req_value()); 
  BOOST_CHECK_EQUAL((int)prop, 10); 
  BOOST_CHECK_EQUAL(prop.pendingRequest(), false); 

  BOOST_CHECK_EQUAL(n1->notifycnt, 1); 
  BOOST_CHECK_EQUAL(n2->notifycnt, 1); 

  prop.remove_watcher(n2i); 

  prop.set_value(100); 

  BOOST_CHECK_EQUAL(n1->notifycnt, 2); 
  BOOST_CHECK_EQUAL(n2->notifycnt, 1); 

}


BOOST_AUTO_TEST_SUITE_END(); 
