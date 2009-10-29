/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval_set_mixed unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

#include "../test_interval_set_mixed.hpp"

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_ctor_4_ordered_types)
{            interval_set_mixed_ctor_4_ordered_types<int>(); }


BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_equal_4_ordered_types)
{            interval_set_mixed_equal_4_ordered_types<std::string>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_assign_4_ordered_types)
{            interval_set_mixed_assign_4_ordered_types<float>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_ctor_4_bicremental_types)
{            interval_set_mixed_ctor_4_bicremental_types<bicremental_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_assign_4_bicremental_types)
{            interval_set_mixed_assign_4_bicremental_types<bicremental_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_equal_4_bicremental_types)
{            interval_set_mixed_equal_4_bicremental_types<bicremental_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_contains_4_bicremental_types)
{            interval_set_mixed_contains_4_bicremental_types<bicremental_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_add_4_bicremental_types)
{            interval_set_mixed_add_4_bicremental_types<bicremental_type_5>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_subtract_4_bicremental_types)
{            interval_set_mixed_subtract_4_bicremental_types<bicremental_type_6>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_erase_4_bicremental_types)
{            interval_set_mixed_erase_4_bicremental_types<bicremental_type_7>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_basic_intersect_4_bicremental_types)
{            interval_set_mixed_basic_intersect_4_bicremental_types<bicremental_type_8>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_disjoint_4_bicremental_types)
{            interval_set_mixed_disjoint_4_bicremental_types<bicremental_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_infix_plus_overload_4_bicremental_types)
{            interval_set_mixed_infix_plus_overload_4_bicremental_types<bicremental_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_infix_pipe_overload_4_bicremental_types)
{            interval_set_mixed_infix_pipe_overload_4_bicremental_types<bicremental_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_infix_minus_overload_4_bicremental_types)
{            interval_set_mixed_infix_minus_overload_4_bicremental_types<bicremental_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_infix_et_overload_4_bicremental_types)
{            interval_set_mixed_infix_et_overload_4_bicremental_types<bicremental_type_5>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_mixed_infix_caret_overload_4_bicremental_types)
{            interval_set_mixed_infix_caret_overload_4_bicremental_types<bicremental_type_6>(); }
