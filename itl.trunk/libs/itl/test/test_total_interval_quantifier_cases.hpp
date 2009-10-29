/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_TOTAL_INTERVAL_QUANTIFIER_CASES_HPP_JOFA_090701
#define BOOST_ITL_TEST_TOTAL_INTERVAL_QUANTIFIER_CASES_HPP_JOFA_090701

//------------------------------------------------------------------------------
// total_absorber
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, total_absorber, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_monoid_et_4_bicremental_types<T, int, total_absorber, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, total_absorber, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, total_absorber, INTERVAL_MAP>();}

// (0-x) + x = 0 | total absorber
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_group_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_group_plus_4_bicremental_types<T, int, total_absorber, INTERVAL_MAP>();}
//------------------------------------------------------------------------------
// total_enricher
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, total_enricher, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_monoid_et_4_bicremental_types<T, int, total_enricher, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, total_enricher, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, total_enricher, INTERVAL_MAP>();}

// (0-x) + x =p= 0 | total absorber
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_group_plus_prot_inv_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_group_plus_prot_inv_4_bicremental_types<T, int, total_enricher, INTERVAL_MAP>();}


//            absorber      enricher
// partial    x - x == 0    x - x =p= 0   partiality of subtraction   
// total    (-x)+ x == 0  (-x)+ x =p= 0   totality   of subtraction

#endif // BOOST_ITL_TEST_TOTAL_INTERVAL_QUANTIFIER_CASES_HPP_JOFA_090701

