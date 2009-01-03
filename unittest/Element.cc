#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "core/queueview.h" 
#include "core/sinkpad.h"
#include "core/sourcepad.h"
#include "core/element.h"


using namespace core; 
BOOST_AUTO_TEST_SUITE(element_test); 


int globalcount; 
void inccount()
{
  globalcount++; 
}

class TestElement : public Element
{
public:
  TestElement() : 
    Element("Hello"), 
    pSourceInt1(createSourcePad<int>(intBuffer1, "sourceInt1")), 
    pSourceInt2(createSourcePad<int>(intBuffer2, "sourceInt2")), 
    pSourceFloat1(createSourcePad<float>(floatBuffer1, "sourceFloat1")), 
    pSinkFloat1(createSinkPad<float>("sinkFloat1")),  
    pSinkInt1(createSinkPad<int>("sinkInt1"))
  {
    
  }
  
  SourcePad<int> * pSourceInt1; 
  boost::ptr_list<int> intBuffer1; 

  SourcePad<int> * pSourceInt2; 
  boost::ptr_list<int> intBuffer2; 

  SourcePad<float> * pSourceFloat1; 
  boost::ptr_list<float> floatBuffer1; 

  SinkPad<float> * pSinkFloat1; 
  SinkPad<int> * pSinkInt1; 

}; 

BOOST_AUTO_TEST_CASE(simpletest)
{
  TestElement testElement; 
  BOOST_CHECK_EQUAL(testElement.getAvailableSources().size(), 3); 
  BOOST_CHECK_EQUAL(testElement.getAvailableSinks().size(), 2); 

  SourcePad<int> * ps = testElement.getSource<int>("sourceInt1"); 
  BOOST_CHECK_EQUAL(ps->getName(), "sourceInt1"); 

  
}

BOOST_AUTO_TEST_SUITE_END(); 
