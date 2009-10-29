/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval_map_infix unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

// -----------------------------------------------------------------------------
// test_interval_map_shared are tests that should give identical results for all
// interval_maps: interval_map and split_interval_map.
#include "../test_interval_map_shared.hpp"

#define INTERVAL_MAP interval_map
#include "../test_interval_map_infix_cases.hpp"


