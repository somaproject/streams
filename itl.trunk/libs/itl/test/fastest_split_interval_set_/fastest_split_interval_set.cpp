/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::split_interval_set unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/split_interval_set.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

// -----------------------------------------------------------------------------
// test_interval_set_shared are tests that should give identical results for all
// interval_sets: interval_set, separate_interval_set and split_interval_set.
#include "../test_interval_set_shared.hpp"

#define INTERVAL_SET split_interval_set 
#include "../fastest_interval_set_cases.hpp"


