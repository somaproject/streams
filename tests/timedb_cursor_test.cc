#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "timedb.h"

BOOST_AUTO_TEST_SUITE(timedbtest); 

using namespace datastore; 


BOOST_AUTO_TEST_CASE(cursor_simple)
{
  TimeSeriesDataBase<std::string> db; 
  
  db.newSequence(10, "E"); 
  db.append(11, "F"); 
  db.append(12, "G"); 
  
  TimeCursor<std::string> tc = db.createCursor(8); 
  
  BOOST_CHECK(tc.valid()); 
  BOOST_CHECK_EQUAL(tc.requestTime(), 8); 
  BOOST_CHECK_EQUAL(tc.startTime(), 10); 
  std::string x; 

  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 10); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 11); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 12); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 

  db.append(13, "G"); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 13); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 
  


}
  
    

BOOST_AUTO_TEST_CASE(cursor_join_sequence_simple)
{
   TimeSeriesDataBase<std::string> db; 
  
  db.newSequence(10, "E"); 
  db.append(11, "F"); 
  db.append(12, "G"); 
  
  std::string x; 

  TimeCursor<std::string> tc = db.createCursor(8); 

  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 10); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 11); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 12); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 

  db.newSequence(15, "X"); 
  db.append(16, "X"); 
  db.append(17, "X"); 

  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 
  db.newSequence(12, "G"); 
  db.append(13, "X"); 

  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  db.append(14, "X"); 

  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  db.append(15, "X"); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 

}
  

BOOST_AUTO_TEST_CASE(shared_ptr_test)
{
  typedef boost::shared_ptr<std::string> pshared_str_t; 
  TimeSeriesDataBase<pshared_str_t> db; 
  
  
  db.newSequence(10, pshared_str_t(new std::string("E"))); 
  db.append(11, pshared_str_t(new std::string("F"))); 
  db.append(12, pshared_str_t(new std::string("G"))); 
  
  TimeCursor<pshared_str_t> tc = db.createCursor(8); 
  
  BOOST_CHECK(tc.valid()); 
  BOOST_CHECK_EQUAL(tc.requestTime(), 8); 
  BOOST_CHECK_EQUAL(tc.startTime(), 10); 
  pshared_str_t x; 

  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 10); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 11); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 12); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 

  db.append(13, pshared_str_t(new std::string("H"))); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_GOOD); 
  BOOST_CHECK_EQUAL(tc.getCurrentValue(&x), 13); 
  BOOST_CHECK_EQUAL(tc.next(), CURSOR_RET_NO_NEXT); 
  


}
 


BOOST_AUTO_TEST_SUITE_END(); 
