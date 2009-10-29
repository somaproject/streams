/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FASTEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090703
#define BOOST_ITL_FASTEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090703

//------------------------------------------------------------------------------
// interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_check_monoid_plus_4_bicremental_types)
{            interval_set_check_monoid_plus_4_bicremental_types<bicremental_type_1, interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_check_abelian_monoid_plus_4_bicremental_types)
{            interval_set_check_abelian_monoid_plus_4_bicremental_types<bicremental_type_2, interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_check_abelian_monoid_et_4_bicremental_types)
{            interval_set_check_abelian_monoid_et_4_bicremental_types<bicremental_type_3, interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types)
{            interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<bicremental_type_4, interval_set>();}


//------------------------------------------------------------------------------
// separate_interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_separate_interval_set_check_monoid_plus_4_bicremental_types)
{                     interval_set_check_monoid_plus_4_bicremental_types<bicremental_type_5, separate_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_separate_interval_set_check_abelian_monoid_plus_4_bicremental_types)
{                     interval_set_check_abelian_monoid_plus_4_bicremental_types<bicremental_type_6, separate_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_separate_interval_set_check_abelian_monoid_et_4_bicremental_types)
{                     interval_set_check_abelian_monoid_et_4_bicremental_types<bicremental_type_7, separate_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_separate_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types)
{                     interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<bicremental_type_8, separate_interval_set>();}


//------------------------------------------------------------------------------
// split_interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_split_interval_set_check_monoid_plus_4_bicremental_types)
{                  interval_set_check_monoid_plus_4_bicremental_types<bicremental_type_1, split_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_split_interval_set_check_abelian_monoid_plus_4_bicremental_types)
{                  interval_set_check_abelian_monoid_plus_4_bicremental_types<bicremental_type_2, split_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_split_interval_set_check_abelian_monoid_et_4_bicremental_types)
{                  interval_set_check_abelian_monoid_et_4_bicremental_types<bicremental_type_3, split_interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_split_interval_set_check_partial_invertive_monoid_plus_4_bicremental_types)
{                  interval_set_check_partial_invertive_monoid_plus_4_bicremental_types<bicremental_type_4, split_interval_set>();}

#endif // BOOST_ITL_FASTEST_SET_INTERVAL_SET_CASES_HPP_JOFA_090703

