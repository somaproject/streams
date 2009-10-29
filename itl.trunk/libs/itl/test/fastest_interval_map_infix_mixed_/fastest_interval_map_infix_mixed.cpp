/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval_map_infix_mixed unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

#include "../test_interval_map_mixed.hpp"

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_mixed_infix_plus_overload_4_bicremental_types)
{            interval_map_mixed_infix_plus_overload_4_bicremental_types<bicremental_type_1, int>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_mixed_infix_pipe_overload_4_bicremental_types)
{            interval_map_mixed_infix_pipe_overload_4_bicremental_types<bicremental_type_2, double>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_mixed_infix_minus_overload_4_bicremental_types)
{            interval_map_mixed_infix_minus_overload_4_bicremental_types<bicremental_type_3, boost::rational<int> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_mixed_infix_et_overload_4_bicremental_types)
{            interval_map_mixed_infix_et_overload_4_bicremental_types<bicremental_type_4, int>(); }
