#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include <elements/sinkpad.h>
#include <elements/sourcepad.h>
#include <elements/element.h>


BOOST_AUTO_TEST_SUITE(element_test); 
using namespace elements; 

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
    pSourceInt1(createSourcePad<int>("sourceInt1")), 
    pSourceInt2(createSourcePad<int>("sourceInt2")), 
    pSourceFloat1(createSourcePad<float>("sourceFloat1")), 
    pSinkFloat1(createSinkPad<float>("sinkFloat1")),  
    pSinkInt1(createSinkPad<int>("sinkInt1"))
  {
    
  }

  void process(elements::timeid_t time) {
    // 
  }
  
  SourcePad<int>::pSourcePad_t pSourceInt1; 

  SourcePad<int>::pSourcePad_t pSourceInt2; 

  SourcePad<float>::pSourcePad_t pSourceFloat1; 

  SinkPad<float>::pSinkPad_t pSinkFloat1; 
  SinkPad<int>::pSinkPad_t pSinkInt1; 

}; 

BOOST_AUTO_TEST_CASE(simpletest)
{
  TestElement testElement; 
//   BOOST_CHECK_EQUAL(testElement.getAvailableSources().size(), 3); 
//   BOOST_CHECK_EQUAL(testElement.getAvailableSinks().size(), 2); 

//   SourcePad<int> * ps = testElement.getSource<int>("sourceInt1"); 
//   BOOST_CHECK_EQUAL(ps->getName(), "sourceInt1"); 

  
}

BOOST_AUTO_TEST_SUITE_END(); 
