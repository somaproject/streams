/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

#include "../test_itl_interval.hpp"

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_4_ordered_types)
{            interval_ctor_4_ordered_types<ordered_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_4_bicremental_types)
{            interval_ctor_4_bicremental_types<bicremental_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_4_integral_types)
{            interval_ctor_4_integral_types<integral_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_specific)
{            interval_ctor_specific(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_equal_4_integral_types)
{            interval_equal_4_integral_types<integral_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_less_4_integral_types)
{            interval_less_4_integral_types<integral_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_equal_4_bicremental_continuous_types)
{            interval_equal_4_bicremental_continuous_types<bicremental_continuous_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_touches_4_bicremental_types)
{            interval_touches_4_bicremental_types<bicremental_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_touches_4_integral_types)
{            interval_touches_4_integral_types<integral_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_inplace_intersect_4_bicremental_types)
{            interval_inplace_intersect_4_bicremental_types<bicremental_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_infix_intersect_4_bicremental_types)
{            interval_infix_intersect_4_bicremental_types<bicremental_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_subtract_4_bicremental_types)
{            interval_subtract_4_bicremental_types<bicremental_type_5>(); }
