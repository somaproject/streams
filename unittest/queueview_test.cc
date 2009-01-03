#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "core/queueview.h" 

using namespace core; 


class NoCopyData : boost::noncopyable
{
public: 
  NoCopyData(int x)  : value_(x) {}
  int getValue() { return value_;} 

private:
  int value_; 
}; 

BOOST_AUTO_TEST_CASE(simpletest)
{

  boost::ptr_list<int> myint; 
  QueueView<int> myView(myint); 
  
  // first, do things work as expected

  BOOST_CHECK(myView.empty() == true); 
  myint.push_back(new int(0)); 

  BOOST_CHECK(myView.empty() == false); 
  
  BOOST_CHECK_EQUAL(myView.front(), 0); 
  
  myint.push_back(new int(100)); 
  BOOST_CHECK_EQUAL(myView.front(), 0); 

  BOOST_CHECK(myView.empty() == false); 

  myView.pop(); 

  BOOST_CHECK_EQUAL(myView.front(), 100); 

  myView.pop(); 

  BOOST_CHECK(myView.empty() == true); 
  

}

BOOST_AUTO_TEST_CASE(excepttest)
{

  boost::ptr_list<int> myint; 
  QueueView<int> myView(myint); 
  
  // first, do things work as expected

  BOOST_CHECK(myView.empty() == true); 
  myint.push_back(new int(0)); 
  myint.push_back(new int(10)); 

  myView.pop(); 
  myView.pop(); 
  // now check for extra 
  
  bool caught = false; 
  BOOST_CHECK(myView.empty() == true); 
  try {
    myView.pop(); 
  } catch (std::out_of_range &) 
    {
      // this is what we want 
      caught = true; 
    }
  BOOST_CHECK(caught); 

}


BOOST_AUTO_TEST_CASE(mutipleviews)
{

  boost::ptr_list<int> myint; 
  QueueView<int> view1(myint); 
  QueueView<int> view2(myint); 
  QueueView<int> view3(myint); 
  
  BOOST_CHECK(view1.empty()); 
  BOOST_CHECK(view2.empty()); 
  BOOST_CHECK(view3.empty()); 

  for (int i = 0; i < 10; i++) {
    myint.push_back(new int(i)); 
  }
  
  BOOST_CHECK(! view1.empty()); 
  BOOST_CHECK(! view2.empty()); 
  BOOST_CHECK(! view3.empty()); 

  for (int i = 0; i < 10; i++) {
    view1.front() == i; 
    view1.pop(); 
  }
  
  for (int i = 0; i < 10; i++) {
    view2.front() == i; 
    view2.pop(); 
  }
  
  for (int i = 0; i < 10; i++) {
    view3.front() == i; 
    view3.pop(); 
  }
  
  

}

BOOST_AUTO_TEST_CASE(lateviews)
{

  boost::ptr_list<int> myint; 
  QueueView<int> view1(myint); 
  
  BOOST_CHECK(view1.empty()); 

  for (int i = 0; i < 10; i++) {
    myint.push_back(new int(i)); 
  }
  
  BOOST_CHECK(! view1.empty()); 

  for (int i = 0; i < 10; i++) {
    view1.front() == i; 
    view1.pop(); 
  }

  QueueView<int> view2(myint); 
  
  for (int i = 0; i < 10; i++) {
    view2.front() == i; 
    view2.pop(); 
  }
  
  

}

BOOST_AUTO_TEST_CASE(queueresets)
{

  boost::ptr_list<int> myint; 
  QueueView<int> view1(myint); 
  
  BOOST_CHECK(view1.empty()); 

  for (int i = 0; i < 10; i++) {
    myint.push_back(new int(i)); 
  }
  
  BOOST_CHECK(! view1.empty()); 

  for (int i = 0; i < 10; i++) {
    view1.front() == i; 
    view1.pop(); 
  }

  view1.reset(); 
  BOOST_CHECK(! view1.empty()); 
  
  BOOST_CHECK_EQUAL(view1.front(), 0); 
  view1.pop(); 
  BOOST_CHECK_EQUAL(view1.front(), 1); 
  view1.pop(); 

}

BOOST_AUTO_TEST_CASE(emptyresets)
{

  boost::ptr_list<int> myint; 
  QueueView<int> view1(myint); 
  
  BOOST_CHECK(view1.empty()); 

  view1.reset(); 
  BOOST_CHECK(view1.empty()); 

}

BOOST_AUTO_TEST_CASE(addmore)
{

  boost::ptr_list<int> myint; 
  QueueView<int> myView(myint); 
  
  // first, do things work as expected

  BOOST_CHECK(myView.empty() == true); 

  myint.push_back(new int(0)); 
  myint.push_back(new int(10)); 

  myView.pop(); 
  myView.pop(); 
  // now check for extra 
  myint.push_back(new int(100)); 
  

  
  BOOST_CHECK(myView.empty() == false); 
  BOOST_CHECK_EQUAL(myView.front(),  100); 
}


BOOST_AUTO_TEST_CASE(copytesttest)
{

  boost::ptr_list<NoCopyData> myNCD; 
  QueueView<NoCopyData> myView(myNCD); 
  
  // first, do things work as expected

  BOOST_CHECK(myView.empty() == true); 
  myNCD.push_back(new NoCopyData(7)); 

  BOOST_CHECK(myView.empty() == false); 
  
  BOOST_CHECK_EQUAL(myView.front().getValue(), 7); 
  
  myNCD.push_back(new NoCopyData(10)); 
  
  BOOST_CHECK_EQUAL(myView.front().getValue(), 7); 

  BOOST_CHECK(myView.empty() == false); 

  myView.pop(); 

  BOOST_CHECK_EQUAL(myView.front().getValue(), 10); 

  myView.pop(); 

  BOOST_CHECK(myView.empty() == true); 
  

}
