/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_laws_h_JOFA_090119__
#define __test_itl_laws_h_JOFA_090119__

#include <boost/itl/type_traits/neutron.hpp>

namespace boost{namespace itl
{

template<class Type>
struct equality : std::binary_function<Type,Type,bool> 
{
    typedef bool (type)(const Type&, const Type&);
};

//------------------------------------------------------------------------------
// Associativity
//------------------------------------------------------------------------------

#define DEFINE_ASSOCIATIVITY_CHECK_WRT(op_tag, op_sign) \
template<class Type, class TypeB, class TypeC> \
void check_associativity_wrt_##op_tag(const Type& a, const TypeB& b, const TypeC& c) \
{ \
    Type left  = (a op_sign b) op_sign c; \
    Type right = a op_sign (b op_sign c); \
    BOOST_CHECK_EQUAL(left,right); \
}

#define DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type, class TypeB, class TypeC> \
void check_associativity_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    Type left  = (a op_sign b) op_sign c; \
    Type right = a op_sign (b op_sign c); \
    BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define CHECK_ASSOCIATIVITY_WRT(op_tag)       check_associativity_wrt_##op_tag
#define CHECK_ASSOCIATIVITY_WRT_EQUAL(op_tag) check_associativity_wrt_equal_##op_tag

DEFINE_ASSOCIATIVITY_CHECK_WRT      (plus,  + );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(plus,  + );
DEFINE_ASSOCIATIVITY_CHECK_WRT      (pipe,  | );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(pipe,  | );
DEFINE_ASSOCIATIVITY_CHECK_WRT      (et,    & );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(et,    & );
DEFINE_ASSOCIATIVITY_CHECK_WRT      (caret, ^ );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(caret, ^ );


//------------------------------------------------------------------------------
// Neutrality
//------------------------------------------------------------------------------

#define DEFINE_RIGHT_NEUTRALITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type> \
void check_right_neutrality_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
    Type left  = a op_sign neutron; \
    Type right = neutron; \
    BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_RIGHT_NEUTRALITY_CHECK_WRT(op_tag, op_sign) \
template<class Type> \
void check_right_neutrality_wrt_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
    Type left  = a op_sign neutron; \
    Type right = neutron; \
    BOOST_CHECK_EQUAL(left,right); \
}

#define DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type> \
void check_neutrality_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
    Type left  = a op_sign neutron; \
    Type right = neutron op_sign a; \
    BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_NEUTRALITY_CHECK_WRT(op_tag, op_sign) \
template<class Type> \
void check_neutrality_wrt_##op_tag \
(const Type& a, const Type& neutron) \
{ \
    Type left  = a op_sign neutron; \
    Type right = neutron op_sign a; \
    BOOST_CHECK_EQUAL(left,right); \
}

#define CHECK_NEUTRALITY_WRT(op_tag) check_neutrality_wrt_##op_tag
#define CHECK_NEUTRALITY_WRT_EQUAL(op_tag) check_neutrality_wrt_equal_##op_tag

DEFINE_NEUTRALITY_CHECK_WRT      (plus,  + );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(plus,  + );
DEFINE_NEUTRALITY_CHECK_WRT      (pipe,  | );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(pipe,  | );
DEFINE_NEUTRALITY_CHECK_WRT      (minus, - );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(minus, - );
DEFINE_NEUTRALITY_CHECK_WRT      (et,    & );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(et,    & );
DEFINE_NEUTRALITY_CHECK_WRT      (caret, ^ );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(caret, ^ );

//------------------------------------------------------------------------------
// Commutativity
//------------------------------------------------------------------------------

#define DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type, class TypeB> \
void check_commutativity_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const TypeB& b) \
{ \
    Type left  = a op_sign b; \
    Type right = b op_sign a; \
    BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_COMMUTATIVITY_CHECK_WRT(op_tag, op_sign) \
template<class Type, class TypeB> \
    void check_commutativity_wrt_##op_tag(const Type& a, const TypeB& b) \
{ \
    Type left  = a op_sign b; \
    Type right = b op_sign a; \
    BOOST_CHECK_EQUAL(left,right); \
}

#define CHECK_COMMUTATIVITY_WRT_EQUAL(op_tag) check_commutativity_wrt_equal_##op_tag
#define CHECK_COMMUTATIVITY_WRT(op_tag) check_commutativity_wrt_##op_tag

DEFINE_COMMUTATIVITY_CHECK_WRT      (plus,  + );
DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(plus,  + );
DEFINE_COMMUTATIVITY_CHECK_WRT      (pipe,  | );
DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(pipe,  | );
DEFINE_COMMUTATIVITY_CHECK_WRT      (et,    & );
DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(et,    & );
DEFINE_COMMUTATIVITY_CHECK_WRT      (caret, ^ );
DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(caret, ^ );

//------------------------------------------------------------------------------
// Natural inversion
//------------------------------------------------------------------------------

#define DEFINE_PARTIAL_INVERSION_CHECK_WRT_EQUAL(plus_tag, plus_sign) \
template<class Type> \
    void check_partial_inversion_wrt_equal_##plus_tag \
(typename equality<Type>::type* equal, const Type& neutron, const Type& var_a) \
{ \
    Type positive_difference  = var_a - var_a; \
    BOOST_CHECK_EQUAL((*equal)(positive_difference, neutron), true); \
}

#define DEFINE_PARTIAL_INVERSION_CHECK_WRT(plus_tag, plus_sign) \
template<class Type> \
    void check_partial_inversion_wrt_##plus_tag \
(const Type& neutron, const Type& var_a) \
{ \
    Type positive_difference  = var_a - var_a; \
    BOOST_CHECK_EQUAL(positive_difference, neutron); \
}

#define CHECK_PARTIAL_INVERSION_WRT_EQUAL(plus_tag) check_partial_inversion_wrt_equal_##plus_tag
#define CHECK_PARTIAL_INVERSION_WRT(plus_tag) check_partial_inversion_wrt_##plus_tag

DEFINE_PARTIAL_INVERSION_CHECK_WRT      (plus,  + );
DEFINE_PARTIAL_INVERSION_CHECK_WRT_EQUAL(plus,  + );
DEFINE_PARTIAL_INVERSION_CHECK_WRT      (pipe,  | );
DEFINE_PARTIAL_INVERSION_CHECK_WRT_EQUAL(pipe,  | );

//------------------------------------------------------------------------------
// Inverse
//------------------------------------------------------------------------------

#define DEFINE_INVERSE_CHECK_WRT_EQUAL(plus_tag, plus_sign) \
template<class Type> \
    void check_inverse_wrt_equal_##plus_tag \
(typename equality<Type>::type* equal, const Type& neutron, const Type& var_a) \
{ \
    Type positive_difference  = var_a - var_a; \
    BOOST_CHECK_EQUAL((*equal)(positive_difference, neutron), true); \
    Type negative_difference = (neutron - var_a) plus_sign var_a; \
    BOOST_CHECK_EQUAL((*equal)(negative_difference, neutron), true); \
}

#define DEFINE_INVERSE_CHECK_WRT(plus_tag, plus_sign) \
template<class Type> \
    void check_inverse_wrt_##plus_tag \
(const Type& neutron, const Type& var_a) \
{ \
    Type positive_difference  = var_a - var_a; \
    BOOST_CHECK_EQUAL(positive_difference, neutron); \
    Type negative_difference = (neutron - var_a) plus_sign var_a; \
    BOOST_CHECK_EQUAL(negative_difference, neutron); \
}

#define CHECK_INVERSE_WRT_EQUAL(plus_tag) check_inverse_wrt_equal_##plus_tag
#define CHECK_INVERSE_WRT(plus_tag) check_inverse_wrt_##plus_tag

DEFINE_INVERSE_CHECK_WRT      (plus,  + );
DEFINE_INVERSE_CHECK_WRT_EQUAL(plus,  + );
DEFINE_INVERSE_CHECK_WRT      (pipe,  | );
DEFINE_INVERSE_CHECK_WRT_EQUAL(pipe,  | );

//------------------------------------------------------------------------------
// Monodid EAN
//------------------------------------------------------------------------------

#define DEFINE_MONOID_CHECK_WRT_EQUAL(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_monoid_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ASSOCIATIVITY_WRT_EQUAL(op_tag)(equal,a,b,c); \
    CHECK_NEUTRALITY_WRT_EQUAL(op_tag)(equal,a,neutron); \
}

#define DEFINE_MONOID_CHECK_WRT(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_monoid_wrt_##op_tag \
(const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ASSOCIATIVITY_WRT(op_tag)(a,b,c); \
    CHECK_NEUTRALITY_WRT(op_tag)(a,neutron); \
}

DEFINE_MONOID_CHECK_WRT(plus);
DEFINE_MONOID_CHECK_WRT_EQUAL(plus);
DEFINE_MONOID_CHECK_WRT(pipe);
DEFINE_MONOID_CHECK_WRT_EQUAL(pipe);
DEFINE_MONOID_CHECK_WRT(et);
DEFINE_MONOID_CHECK_WRT_EQUAL(et);
DEFINE_MONOID_CHECK_WRT(caret);
DEFINE_MONOID_CHECK_WRT_EQUAL(caret);

#define CHECK_MONOID_WRT(op_tag) check_monoid_wrt_##op_tag
#define CHECK_MONOID_WRT_EQUAL(op_tag) check_monoid_wrt_equal_##op_tag

//------------------------------------------------------------------------------
// Commutative or Abelian monodid EANC
//------------------------------------------------------------------------------
#define DEFINE_ABELIAN_MONOID_CHECK_WRT_EQUAL(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_abelian_monoid_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ASSOCIATIVITY_WRT_EQUAL(op_tag)(equal,a,b,c); \
    CHECK_NEUTRALITY_WRT_EQUAL(op_tag)(equal,a,neutron); \
    CHECK_COMMUTATIVITY_WRT_EQUAL(op_tag)(equal,a,b); \
    CHECK_COMMUTATIVITY_WRT_EQUAL(op_tag)(equal,a,c); \
}

#define DEFINE_ABELIAN_MONOID_CHECK_WRT(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_abelian_monoid_wrt_##op_tag \
(const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ASSOCIATIVITY_WRT(op_tag)(a,b,c); \
    CHECK_NEUTRALITY_WRT(op_tag)(a,neutron); \
    CHECK_COMMUTATIVITY_WRT(op_tag)(a,b); \
    CHECK_COMMUTATIVITY_WRT(op_tag)(a,c); \
}

DEFINE_ABELIAN_MONOID_CHECK_WRT(plus);
DEFINE_ABELIAN_MONOID_CHECK_WRT_EQUAL(plus);
DEFINE_ABELIAN_MONOID_CHECK_WRT(pipe);
DEFINE_ABELIAN_MONOID_CHECK_WRT_EQUAL(pipe);
DEFINE_ABELIAN_MONOID_CHECK_WRT(et);
DEFINE_ABELIAN_MONOID_CHECK_WRT_EQUAL(et);
DEFINE_ABELIAN_MONOID_CHECK_WRT(caret);
DEFINE_ABELIAN_MONOID_CHECK_WRT_EQUAL(caret);

#define CHECK_ABELIAN_MONOID_WRT(op_tag) check_abelian_monoid_wrt_##op_tag
#define CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag) check_abelian_monoid_wrt_equal_##op_tag

//------------------------------------------------------------------------------
// Abelian monodid EANC with partial inversion
//------------------------------------------------------------------------------
#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT_EQUAL(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_partial_invertive_monoid_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal,neutron,a,b,c); \
    CHECK_PARTIAL_INVERSION_WRT_EQUAL(op_tag)(equal,neutron,a); \
}

#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_partial_invertive_monoid_wrt_##op_tag \
(const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ABELIAN_MONOID_WRT(op_tag)(neutron,a,b,c); \
    CHECK_PARTIAL_INVERSION_WRT(op_tag)(neutron,a); \
}

DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT_EQUAL(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT_EQUAL(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT_EQUAL(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT(caret);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_WRT_EQUAL(caret);

#define CHECK_PARTIAL_INVERTIVE_MONOID_WRT(op_tag) check_partial_invertive_monoid_wrt_##op_tag
#define CHECK_PARTIAL_INVERTIVE_MONOID_WRT_EQUAL(op_tag) check_partial_invertive_monoid_wrt_equal_##op_tag

//------------------------------------------------------------------------------
// Abelian group EANIC
//------------------------------------------------------------------------------

#define DEFINE_ABELIAN_GROUP_CHECK_WRT_EQUAL(op_tag) \
template<class Type, class TypeB, class TypeC> \
    void check_abelian_group_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal,neutron,a,b,c); \
    CHECK_INVERSE_WRT_EQUAL(op_tag)(equal,neutron,a); \
}

#define DEFINE_ABELIAN_GROUP_CHECK_WRT(op_tag) \
template<class Type, class TypeB, class TypeC> \
    void check_abelian_group_wrt_##op_tag \
(const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
    CHECK_ABELIAN_MONOID_WRT(op_tag)(neutron,a,b,c); \
    CHECK_INVERSE_WRT(op_tag)(neutron,a); \
}


DEFINE_ABELIAN_GROUP_CHECK_WRT(plus);
DEFINE_ABELIAN_GROUP_CHECK_WRT_EQUAL(plus);
DEFINE_ABELIAN_GROUP_CHECK_WRT(pipe);
DEFINE_ABELIAN_GROUP_CHECK_WRT_EQUAL(pipe);

#define CHECK_ABELIAN_GROUP_WRT(op_tag) check_abelian_group_wrt_##op_tag
#define CHECK_ABELIAN_GROUP_WRT_EQUAL(op_tag) check_abelian_group_wrt_equal_##op_tag

//------------------------------------------------------------------------------
// Modoid permuted
//------------------------------------------------------------------------------

#define DEFINE_MONOID_CHECK_PERMUTED_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_monoid_permuted_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}

#define DEFINE_MONOID_CHECK_PERMUTED_WRT(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_monoid_permuted_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}


DEFINE_MONOID_CHECK_PERMUTED_WRT(plus);
DEFINE_MONOID_CHECK_PERMUTED_WRT_EQUAL(plus);
DEFINE_MONOID_CHECK_PERMUTED_WRT(pipe);
DEFINE_MONOID_CHECK_PERMUTED_WRT_EQUAL(pipe);
DEFINE_MONOID_CHECK_PERMUTED_WRT(et);
DEFINE_MONOID_CHECK_PERMUTED_WRT_EQUAL(et);
DEFINE_MONOID_CHECK_PERMUTED_WRT(caret);
DEFINE_MONOID_CHECK_PERMUTED_WRT_EQUAL(caret);

#define CHECK_MONOID_PERMUTED_WRT(op_tag)       check_monoid_permuted_wrt_##op_tag
#define CHECK_MONOID_PERMUTED_WRT_EQUAL(op_tag) check_monoid_permuted_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Abelian modoid permuted
//------------------------------------------------------------------------------

#define DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_abelian_monoid_permuted_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_ABELIAN_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}

#define DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_abelian_monoid_permuted_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_ABELIAN_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_ABELIAN_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_ABELIAN_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_ABELIAN_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}


DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT(plus);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT_EQUAL(plus);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT(pipe);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT_EQUAL(pipe);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT(et);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT_EQUAL(et);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT(caret);
DEFINE_ABELIAN_MONOID_CHECK_PERMUTED_WRT_EQUAL(caret);

#define CHECK_ABELIAN_MONOID_PERMUTED_WRT(op_tag)       check_abelian_monoid_permuted_wrt_##op_tag
#define CHECK_ABELIAN_MONOID_PERMUTED_WRT_EQUAL(op_tag) check_abelian_monoid_permuted_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Abelian modoid with partial inversion permuted
//------------------------------------------------------------------------------

#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_partial_invertive_monoid_permuted_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}

#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
void check_partial_invertive_monoid_permuted_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_PARTIAL_INVERTIVE_MONOID_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}


DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT_EQUAL(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT_EQUAL(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT_EQUAL(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT(caret);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_PERMUTED_WRT_EQUAL(caret);

#define CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT(op_tag)       check_partial_invertive_monoid_permuted_wrt_##op_tag
#define CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT_EQUAL(op_tag) check_partial_invertive_monoid_permuted_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Abelian group permuted
//------------------------------------------------------------------------------

#define DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
    void check_abelian_group_permuted_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_ABELIAN_GROUP_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_ABELIAN_GROUP_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_ABELIAN_GROUP_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_ABELIAN_GROUP_WRT_EQUAL(op_tag)(equal, itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}

#define DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT(op_tag) \
template<class TypeA, class TypeB, class Assoc> \
    void check_abelian_group_permuted_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const Assoc& assoc) \
{ \
    CHECK_ABELIAN_GROUP_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, var_b, assoc);\
    CHECK_ABELIAN_GROUP_WRT(op_tag)(itl::neutron<TypeA>::value(), var_a, assoc, var_b);\
    CHECK_ABELIAN_GROUP_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, var_a, assoc);\
    CHECK_ABELIAN_GROUP_WRT(op_tag)(itl::neutron<TypeB>::value(), var_b, assoc, var_a);\
}


DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT(plus);
DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT_EQUAL(plus);
DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT(pipe);
DEFINE_ABELIAN_GROUP_CHECK_PERMUTED_WRT_EQUAL(pipe);

#define CHECK_ABELIAN_GROUP_PERMUTED_WRT(op_tag)       check_abelian_group_permuted_wrt_##op_tag
#define CHECK_ABELIAN_GROUP_PERMUTED_WRT_EQUAL(op_tag) check_abelian_group_permuted_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Modoid instance
//------------------------------------------------------------------------------

#define DEFINE_MONOID_CHECK_INSTANCE_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_monoid_instance_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, var_c);\
    CHECK_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_a);\
    CHECK_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_b);\
}

#define DEFINE_MONOID_CHECK_INSTANCE_WRT(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_monoid_instance_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, var_c);\
    CHECK_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_a);\
    CHECK_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_b);\
}

DEFINE_MONOID_CHECK_INSTANCE_WRT(plus);
DEFINE_MONOID_CHECK_INSTANCE_WRT_EQUAL(plus);
DEFINE_MONOID_CHECK_INSTANCE_WRT(pipe);
DEFINE_MONOID_CHECK_INSTANCE_WRT_EQUAL(pipe);
DEFINE_MONOID_CHECK_INSTANCE_WRT(et);
DEFINE_MONOID_CHECK_INSTANCE_WRT_EQUAL(et);
DEFINE_MONOID_CHECK_INSTANCE_WRT(caret);
DEFINE_MONOID_CHECK_INSTANCE_WRT_EQUAL(caret);

#define CHECK_MONOID_INSTANCE_WRT(op_tag)       check_monoid_instance_wrt_##op_tag
#define CHECK_MONOID_INSTANCE_WRT_EQUAL(op_tag) check_monoid_instance_wrt_equal_##op_tag



//------------------------------------------------------------------------------
// Abelian modoid instance
//------------------------------------------------------------------------------

#define DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_abelian_monoid_instance_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_ABELIAN_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, var_c);\
    CHECK_ABELIAN_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_a);\
    CHECK_ABELIAN_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_b);\
}

#define DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_abelian_monoid_instance_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_ABELIAN_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, var_c);\
    CHECK_ABELIAN_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_a);\
    CHECK_ABELIAN_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_b);\
}

DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT(plus);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT_EQUAL(plus);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT(pipe);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT_EQUAL(pipe);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT(et);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT_EQUAL(et);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT(caret);
DEFINE_ABELIAN_MONOID_CHECK_INSTANCE_WRT_EQUAL(caret);

#define CHECK_ABELIAN_MONOID_INSTANCE_WRT(op_tag)       check_abelian_monoid_instance_wrt_##op_tag
#define CHECK_ABELIAN_MONOID_INSTANCE_WRT_EQUAL(op_tag) check_abelian_monoid_instance_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Abelian partial invertive modoid instance
//------------------------------------------------------------------------------

#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_partial_invertive_monoid_instance_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, var_c);\
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_a);\
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_b);\
}

#define DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
void check_partial_invertive_monoid_instance_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, var_c);\
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_a);\
    CHECK_PARTIAL_INVERTIVE_MONOID_PERMUTED_WRT(op_tag)(var_a, var_b, ass_b);\
}

DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT_EQUAL(plus);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT_EQUAL(pipe);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT_EQUAL(et);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT(caret);
DEFINE_PARTIAL_INVERTIVE_MONOID_CHECK_INSTANCE_WRT_EQUAL(caret);

#define CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(op_tag)       check_partial_invertive_monoid_instance_wrt_##op_tag
#define CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT_EQUAL(op_tag) check_partial_invertive_monoid_instance_wrt_equal_##op_tag


//------------------------------------------------------------------------------
// Abelian group instance
//------------------------------------------------------------------------------

#define DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT_EQUAL(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
    void check_abelian_group_instance_wrt_equal_##op_tag \
(typename equality<TypeA>::type* equal, const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_ABELIAN_GROUP_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, var_c);\
    CHECK_ABELIAN_GROUP_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_a);\
    CHECK_ABELIAN_GROUP_PERMUTED_WRT_EQUAL(op_tag)(equal, var_a, var_b, ass_b);\
}

#define DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT(op_tag) \
template<class TypeA, class TypeB, class TypeC, class AssocA, class AssocB> \
    void check_abelian_group_instance_wrt_##op_tag \
(const TypeA& var_a, const TypeB& var_b, const TypeC& var_c, \
 const AssocA& ass_a, const AssocB& ass_b) \
{ \
    CHECK_ABELIAN_GROUP_PERMUTED_WRT(op_tag)(var_a, var_b, var_c);\
    CHECK_ABELIAN_GROUP_PERMUTED_WRT(op_tag)(var_a, var_b, ass_a);\
    CHECK_ABELIAN_GROUP_PERMUTED_WRT(op_tag)(var_a, var_b, ass_b);\
}

DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT(plus);
DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT_EQUAL(plus);
DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT(pipe);
DEFINE_ABELIAN_GROUP_CHECK_INSTANCE_WRT_EQUAL(pipe);

#define CHECK_ABELIAN_GROUP_INSTANCE_WRT(op_tag)       check_abelian_group_instance_wrt_##op_tag
#define CHECK_ABELIAN_GROUP_INSTANCE_WRT_EQUAL(op_tag) check_abelian_group_instance_wrt_equal_##op_tag

}} // namespace itl boost

#endif // __test_itl_laws_h_JOFA_080920__

