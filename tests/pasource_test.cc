#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <somanetwork/network.h>
#include <somanetwork/fakenetwork.h>
#include <core/queueview.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <data/wave.h>

#include <sources/pasource.h>
#include <gtkmm.h>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;


BOOST_AUTO_TEST_SUITE(pasource);

bool quitcallback(  Glib::RefPtr< Glib::MainLoop > ml)
{
  
  std::cout << "Quitting " << std::endl; 
  ml->quit(); 
  return false; 

}

BOOST_AUTO_TEST_CASE(create)
{

  Glib::RefPtr< Glib::MainLoop > mainloop = Glib::MainLoop::create(); 
  Glib::signal_timeout().connect(sigc::bind(sigc::ptr_fun(quitcallback), 
					    mainloop),
			       1000); 


  PASource src; 
  PASource src2; 
  mainloop->run(); 
  std::cout << "And nwo the end" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END(); 
