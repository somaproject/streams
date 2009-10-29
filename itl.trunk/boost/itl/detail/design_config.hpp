/*-----------------------------------------------------------------------------+
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------+
Template parameters of major itl class templates can be designed as
template template parameters or
template type parameter
by setting defines in this file.
+-----------------------------------------------------------------------------*/
#ifndef  BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214
#define  BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214


//------------------------------------------------------------------------------
// Auxiliary macros for denoting template signatures.
// Purpose:
// (1) Shorten the lenthy and redundant template signatures.
// (2) Name anonymous template types according to theirs meaning ...
// (3) Making easier to refactor by redefinitin of the macros
// (4) Being able to check template template parameter variants against
//     template type parameter variants.

#define ITL_USE_COMPARE_TEMPLATE_TEMPLATE
#define ITL_USE_COMBINE_TEMPLATE_TEMPLATE
#define ITL_USE_SECTION_TEMPLATE_TEMPLATE

//#define ITL_USE_COMPARE_TEMPLATE_TYPE
//#define ITL_USE_COMBINE_TEMPLATE_TYPE
//#define ITL_USE_SECTION_TEMPLATE_TYPE

//------------------------------------------------------------------------------
// template parameter Compare can not be a template type parameter as long as
// Compare<Interval<DomainT,Compare> >() is called in std::lexicographical_compare
// implementing operator< for interval_base_{set,map}. see NOTE DESIGN TTP
#ifdef ITL_USE_COMPARE_TEMPLATE_TEMPLATE
#   define ITL_COMPARE template<class>class
#   define ITL_COMPARE_DOMAIN(itl_compare, domain_type) itl_compare<domain_type> 
#   define ITL_COMPARE_INSTANCE(compare_instance, domain_type) compare_instance
#   define ITL_EXCLUSIVE_LESS(interval_type) exclusive_less
#else//ITL_USE_COMPARE_TEMPLATE_TYPE
#   define ITL_COMPARE class
#   define ITL_COMPARE_DOMAIN(itl_compare, domain_type) itl_compare 
#   define ITL_COMPARE_INSTANCE(compare_instance, domain_type) compare_instance<domain_type> 
#   define ITL_EXCLUSIVE_LESS(interval_type) exclusive_less<interval_type>
#endif

//------------------------------------------------------------------------------
// template parameter Combine could be a template type parameter.
#ifdef ITL_USE_COMBINE_TEMPLATE_TEMPLATE
#   define ITL_COMBINE template<class>class
#   define ITL_COMBINE_CODOMAIN(itl_combine, codomain_type) itl_combine<codomain_type> 
#   define ITL_COMBINE_INSTANCE(combine_instance,codomain_type) combine_instance
#else//ITL_USE_COMBINE_TEMPLATE_TYPE
#   define ITL_COMBINE class
#   define ITL_COMBINE_CODOMAIN(itl_combine, codomain_type) itl_combine 
#   define ITL_COMBINE_INSTANCE(combine_instance,codomain_type) combine_instance<codomain_type>
#endif

//------------------------------------------------------------------------------
// template parameter Section could be a template type parameter.
#ifdef ITL_USE_SECTION_TEMPLATE_TEMPLATE
#   define ITL_SECTION template<class>class
#   define ITL_SECTION_CODOMAIN(itl_intersect, codomain_type) itl_intersect<codomain_type> 
#   define ITL_SECTION_INSTANCE(section_instance,codomain_type) section_instance
#else//ITL_USE_SECTION_TEMPLATE_TYPE
#   define ITL_SECTION class
#   define ITL_SECTION_CODOMAIN(itl_intersect, codomain_type) itl_intersect 
#   define ITL_SECTION_INSTANCE(section_instance,codomain_type) section_instance<codomain_type>
#endif

//------------------------------------------------------------------------------
#define ITL_ALLOC    template<class>class

//------------------------------------------------------------------------------

#endif // BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214


