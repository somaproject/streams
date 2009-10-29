/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>

#include <boost/validate/loki_xt/Tuple.h>
#include <boost/itl/set.hpp>
#include <boost/itl/map.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>
#include <boost/validate/gentor/rangegentor.hpp>

#include <boost/itl/interval_set.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/setgentor.hpp>
#include <boost/itl/functors.hpp>
#include <boost/validate/driver/map_order_driver.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::itl;

void test_itl_order_driver()
{
    map_order_driver validater;
    cout << 
    ">> ------------------------------------------------------ <<\n"
    ">> -------- Law based test automaton 'LaBatea' ---------- <<\n"
    ">> Output will be generated in a few seconds\n"
    ">> terminate by typing <CTRL>C\n"
    ">> ------------------------------------------------------ <<\n";
    GentorProfileSgl::it()->set_std_profile(16,1);
    GentorProfileSgl::it()->report_profile();
    validater.validate();
};


int main()
{
    test_itl_order_driver();
    return 0;
}
