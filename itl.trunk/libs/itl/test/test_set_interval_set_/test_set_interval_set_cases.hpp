/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090701
#define BOOST_ITL_TEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090701

//------------------------------------------------------------------------------
// interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{         interval_set_check_monoid_plus_4_bicremental_types<T, interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{         interval_set_check_abelian_monoid_plus_4_bicremental_types<T, interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{         interval_set_check_abelian_monoid_et_4_bicremental_types<T, interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types, T, bicremental_types)
{         interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<T, interval_set>();}


//------------------------------------------------------------------------------
// separate_interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_separate_interval_set_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{                  interval_set_check_monoid_plus_4_bicremental_types<T, separate_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_separate_interval_set_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{                  interval_set_check_abelian_monoid_plus_4_bicremental_types<T, separate_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_separate_interval_set_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{                  interval_set_check_abelian_monoid_et_4_bicremental_types<T, separate_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_separate_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types, T, bicremental_types)
{                  interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<T, separate_interval_set>();}


//------------------------------------------------------------------------------
// split_interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_split_interval_set_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{               interval_set_check_monoid_plus_4_bicremental_types<T, split_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_split_interval_set_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{               interval_set_check_abelian_monoid_plus_4_bicremental_types<T, split_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_split_interval_set_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{               interval_set_check_abelian_monoid_et_4_bicremental_types<T, split_interval_set>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_split_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types, T, bicremental_types)
{               interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<T, split_interval_set>();}

#endif // BOOST_ITL_TEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090701

