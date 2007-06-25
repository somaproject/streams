
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "streamcontrol.h" 

int deletednum = 0; 

void deleted()
{
  // test deleted callback
  deletednum++; 

}

// teststruct:
struct testDelStruct
{
  streamSourcePtr_t s; 
  streamVisPtr_t v; 
}; 

BOOST_AUTO_TEST_CASE(simpletest)
{
  /* This test simply checks if we can create a source and a vis
     and if removing the source removes the vis
  */


  StreamControl x; 
  
  streamSourcePtr_t s = x.newSourceFactory("wave"); 

  streamVisPtr_t v = x.newVisFactory(s, "wave"); 
  
  x.remove(s);
  
  BOOST_CHECK(s.unique()); 
  BOOST_CHECK(v.unique()); 

  
}

BOOST_AUTO_TEST_CASE(removevistest)
{
  /*
    Can we successfully remove the vis ? 


  */

  StreamControl x; 
  
  streamSourcePtr_t s = x.newSourceFactory("wave"); 

  streamVisPtr_t v = x.newVisFactory(s, "wave"); 
  
  x.remove(v);
  
  BOOST_CHECK(!s.unique()); 
  BOOST_CHECK(v.unique()); 



}

BOOST_AUTO_TEST_CASE(propagate)
{
  StreamControl x; 
  
  testDelStruct * t = new testDelStruct; 
  t->s = x.newSourceFactory("wave"); 
  t->v = x.newVisFactory(t->s, "wave"); 
  
  t->s->disconnectSignal().connect(sigc::ptr_fun(&deleted)); 
  t->v->disconnectSignal().connect(sigc::ptr_fun(&deleted)); 

  x.remove(t->s); 

  delete t; 
  BOOST_CHECK_EQUAL(deletednum, 2); 

}
