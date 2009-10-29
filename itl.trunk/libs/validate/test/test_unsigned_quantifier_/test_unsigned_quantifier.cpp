/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE validate::itl_unsigned_quantifier unit test
#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/validate/driver/unsigned_quantifier_driver.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

BOOST_AUTO_TEST_CASE(test_validate_itl_unsigned_quantifier)
{
    unsigned_quantifier_driver validater;
    GentorProfileSgl::it()->set_std_profile(16,1);
    //GentorProfileSgl::it()->report_profile();
    validater.terminate_at_law_count(84, 2);
    validater.set_information_level(inform::never);

    BOOST_CHECK_EQUAL(validater.validate(), true);
}

