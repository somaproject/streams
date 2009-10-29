/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_quantifier_itl_map_h_JOFA_090119__
#define __test_itl_quantifier_itl_map_h_JOFA_090119__


//------------------------------------------------------------------------------
// Monoid EAN
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_monoid_plus_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_MONOID_INSTANCE_WRT(plus) (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_MONOID_INSTANCE_WRT(pipe) (map_a, map_b, map_c, map_pair1, map_pair2);
}


template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_monoid_et_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_MONOID_INSTANCE_WRT(et)   (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_MONOID_INSTANCE_WRT(caret)(map_a, map_b, map_c, map_pair1, map_pair2);
}

//------------------------------------------------------------------------------
// Abelian monoid EANC
//------------------------------------------------------------------------------

template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_abelian_monoid_plus_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(plus) (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(pipe) (map_a, map_b, map_c, map_pair1, map_pair2);
}


template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_abelian_monoid_et_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(et)   (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(caret)(map_a, map_b, map_c, map_pair1, map_pair2);
}


//------------------------------------------------------------------------------
// Abelian partial invertive monoid 
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_partial_invertive_monoid_plus_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(plus) (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(pipe) (map_a, map_b, map_c, map_pair1, map_pair2);
}

//------------------------------------------------------------------------------
// Abelian partial invertive monoid with protonic equality for inversion
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_partial_invertive_monoid_plus_prot_inv_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(plus)(map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(pipe)(map_a, map_b, map_c, map_pair1, map_pair2);

    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT_EQUAL(plus)(is_protonic_equal, map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT_EQUAL(pipe)(is_protonic_equal, map_a, map_b, map_c, map_pair1, map_pair2);
}


//------------------------------------------------------------------------------
// Abelian group EANIC
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_abelian_group_plus_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_ABELIAN_GROUP_INSTANCE_WRT(plus) (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_GROUP_INSTANCE_WRT(pipe) (map_a, map_b, map_c, map_pair1, map_pair2);
}

//------------------------------------------------------------------------------
// (0 - x) + x =p= 0  |  
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void quantifier_map_check_abelian_group_plus_prot_inv_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a, itv_map_b, itv_map_c;
    itv_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
    itv_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
    itv_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    MapT map_a, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);

    typename MapT::value_type map_pair1 = sK_v(5,1);
    typename MapT::value_type map_pair2 = sK_v(9,3);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(plus) (map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(pipe) (map_a, map_b, map_c, map_pair1, map_pair2);

    CHECK_ABELIAN_GROUP_INSTANCE_WRT_EQUAL(plus) (is_protonic_equal, map_a, map_b, map_c, map_pair1, map_pair2);
    CHECK_ABELIAN_GROUP_INSTANCE_WRT_EQUAL(pipe) (is_protonic_equal, map_a, map_b, map_c, map_pair1, map_pair2);
}

#endif // __test_itl_quantifier_itl_map_h_JOFA_090119__

