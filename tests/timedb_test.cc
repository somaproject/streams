#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "timedb.h"

BOOST_AUTO_TEST_SUITE(timedbtest); 

using namespace datastore; 


BOOST_AUTO_TEST_CASE(sequence)
{
  std::cout << "TEST CASE SEQUENCE================"  << std::endl;
  MonotonicConstantSequence<std::string> seq; 
  
  ListNode<std::string> * headnode = new ListNode<std::string>(10, "Hello"); 
  seq.sequence.push_back(*headnode); 

  BOOST_CHECK(seq.search_leq(10) ==  seq.sequence.begin()); 
  BOOST_CHECK(seq.search_leq(11) ==  seq.sequence.begin()); 
  BOOST_CHECK(seq.search_leq(0) ==  seq.sequence.end()); 
  delete headnode; 

}

BOOST_AUTO_TEST_CASE(simple)  {
  std::cout << "TEST CASE SIMPLE================"  << std::endl;
  TimeSeriesDataBase<std::string> db; 
  
  
  BOOST_CHECK_EQUAL(db.seqsize(), 0); 
  BOOST_CHECK_EQUAL(db.size(), 0); 
  
  db.newSequence(10, "A");
  db.append(20, "B");
  db.append(30, "C");
  
  BOOST_CHECK_EQUAL(db.seqsize(), 1); 
  BOOST_CHECK_EQUAL(db.size(), 3); 
  
  db.newSequence(40, "D");
  db.append(50, "E");
  db.append(60, "F");
  
  BOOST_CHECK_EQUAL(db.seqsize(), 2); 
  BOOST_CHECK_EQUAL(db.size(), 6); 
  
  db.newSequence(30, "C"); 
  BOOST_CHECK_EQUAL(db.seqsize(), 2); 
  BOOST_CHECK_EQUAL(db.size(), 6); 
  
  db.append(30, "C");
  db.append(32, "C+");
  db.append(34, "C++");
  db.append(36, "C+++");

  BOOST_CHECK_EQUAL(db.seqsize(), 2); 

  db.append(40, "D"); 

  BOOST_CHECK_EQUAL(db.seqsize(), 1); 

  
}

BOOST_AUTO_TEST_CASE(error)
{
  std::cout << "TEST CASE ERROR ================"  << std::endl;
  TimeSeriesDataBase<std::string> db; 

  
  db.newSequence(40, "D");
  db.append(50, "E");
  db.append(60, "F");
  
  
  db.newSequence(10, "A");
  db.append(20, "B");
  db.append(30, "C");

  BOOST_CHECK_THROW(db.append(50, "X"), std::runtime_error); 

}

BOOST_AUTO_TEST_CASE(overlap)
{
  /*
    
    create 
    1. A B C
    2.             G H I
    3.   B C D E F G H

   */ 

  TimeSeriesDataBase<std::string> db; 
  db.newSequence(10, "A"); 
  db.append(20, "B"); 
  db.append(30, "C"); 

  db.newSequence(70, "G"); 
  db.append(80, "H"); 
  db.append(90, "I"); 

  db.newSequence(20, "B"); 
  db.append(30, "C"); 
  db.append(40, "D"); 
  db.append(50, "E"); 
  db.append(60, "F"); 
  db.append(70, "G"); 
  db.append(80, "H"); 
 
  BOOST_CHECK_EQUAL(db.seqsize(), 1); 
 
}


BOOST_AUTO_TEST_CASE(sequence_head_change) {
  /* 
     Do we do what we expect to do with changing the sequence
     head? 
  */ 

  TimeSeriesDataBase<std::string> db; 
  BOOST_CHECK_EQUAL(db.newSequence(10, "A"), false); 
  BOOST_CHECK_EQUAL(db.newSequence(10, "A"), true); 
  
  BOOST_CHECK_EQUAL(db.seqsize(), 1); 
  BOOST_CHECK_EQUAL(db.size(), 1); 
}
  

BOOST_AUTO_TEST_CASE(errors) {

  std::cout << "BOOST ERROR TEST CASE ===============" << std::endl;
  TimeSeriesDataBase<std::string> db1; 

  BOOST_CHECK_THROW(db1.append(10, "E"),
		    std::runtime_error); // should throw No Active Sequence

  TimeSeriesDataBase<std::string> db2; 
  db2.newSequence(10, "E"); 
  db2.append(12, "F"); 
  BOOST_CHECK_THROW(db2.append(11, "G"), std::runtime_error);
  // should throw "violation of monotonicity

  TimeSeriesDataBase<std::string> db3; 
  db3.newSequence(5, "A"); 
  db3.append(7, "B"); 
  db3.append(8, "C"); 

  db3.newSequence(4, "H"); 
  db3.append(5, "I"); // FIXME should throw error, non-constant insert

  BOOST_CHECK_THROW(db3.append(6, "K"),
		    std::runtime_error); // thow error, violate canonical monotonicity


}



BOOST_AUTO_TEST_SUITE_END(); 
