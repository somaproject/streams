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
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


#include "../test_itl_interval.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_ctor_4_ordered_types, T, ordered_types)
{         interval_ctor_4_ordered_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_ctor_4_bicremental_types, T, bicremental_types)
{         interval_ctor_4_bicremental_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_ctor_4_integral_types, T, integral_types)
{         interval_ctor_4_integral_types<T>(); }

BOOST_AUTO_TEST_CASE
(test_itl_interval_ctor_specific)
{         interval_ctor_specific(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_equal_4_integral_types, T, integral_types)
{         interval_equal_4_integral_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_less_4_integral_types, T, integral_types)
{         interval_less_4_integral_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_equal_4_bicremental_continuous_types, T, bicremental_continuous_types)
{         interval_equal_4_bicremental_continuous_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_touches_4_bicremental_types, T, bicremental_types)
{         interval_touches_4_bicremental_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_touches_4_integral_types, T, integral_types)
{         interval_touches_4_integral_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_inplace_intersect_4_bicremental_types, T, bicremental_types)
{         interval_inplace_intersect_4_bicremental_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_infix_intersect_4_bicremental_types, T, bicremental_types)
{         interval_infix_intersect_4_bicremental_types<T>(); }

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_subtract_4_bicremental_types, T, bicremental_types)
{         interval_subtract_4_bicremental_types<T>(); }

