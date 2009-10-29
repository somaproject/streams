/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_itl_map_h_JOFA_090119__
#define __test_itl_itl_map_h_JOFA_090119__


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
void itl_map_find_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a;
    itv_map_a.add(IDv(2,4,1)).add(IIv(6,7,3));

    MapT map_a;
    Interval::atomize(map_a, itv_map_a);

    typename MapT::const_iterator found = map_a.find(MK_v(6));

    BOOST_CHECK_EQUAL( found->second, MK_u(3) );
    BOOST_CHECK_EQUAL( map_a(MK_v(6)), MK_u(3) );

    found = map_a.find(MK_v(5));

    BOOST_CHECK_EQUAL( found == map_a.end(), true );
    BOOST_CHECK_EQUAL( map_a(MK_v(5)), MK_u(0) );
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
void itl_map_inclusion_compare_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;
    typedef itl::set<T> SetT;

    IntervalMapT itv_map_sub_a, itv_map_a, itv_map_super_a, 
                 itv_map_b, itv_map_c;
    itv_map_sub_a.add(IDv(2,4,1)).add(IIv(6,7,3));
    itv_map_a = itv_map_sub_a; 
    itv_map_a.add(IIv(9,9,1));
    itv_map_c = itv_map_sub_a;
    itv_map_c.erase(MK_v(7)).add(IIv(11,11,2));
    itv_map_b = itv_map_a;
    itv_map_b.set(IIv(6,7,2));

    MapT map_sub_a, map_a, map_a2, map_b, map_c;
    Interval::atomize(map_a, itv_map_a);
    Interval::atomize(map_b, itv_map_b);
    Interval::atomize(map_c, itv_map_c);
    Interval::atomize(map_sub_a, itv_map_sub_a);

    map_a2 = map_a;
    BOOST_CHECK_EQUAL( inclusion_compare(MapT(), MapT()), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(map_a, map_a), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(map_a, map_a2), inclusion::equal );

    BOOST_CHECK_EQUAL( inclusion_compare(map_a, MapT()), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(map_a, map_sub_a), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(MapT(), map_a), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(map_sub_a, map_a), inclusion::subset );

    BOOST_CHECK_EQUAL( inclusion_compare(map_a, map_b), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare(map_a, map_c), inclusion::unrelated );

    SetT set_sub_a, set_a, set_a2, set_b, set_c;
    map_a.domain(set_a);
    map_a2.domain(set_a2);
    map_sub_a.domain(set_sub_a);

    BOOST_CHECK_EQUAL( inclusion_compare(MapT(), SetT()), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(SetT(), MapT()), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(SetT(), SetT()), inclusion::equal );

    BOOST_CHECK_EQUAL( inclusion_compare(map_a, set_a), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(set_a, map_a), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(set_a, set_a2), inclusion::equal );

    BOOST_CHECK_EQUAL( inclusion_compare(map_a, SetT()), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(map_a, set_sub_a), inclusion::superset );

    BOOST_CHECK_EQUAL( inclusion_compare(SetT(), map_a), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(set_sub_a, map_a), inclusion::subset );

    BOOST_CHECK_EQUAL( inclusion_compare(set_a, SetT()), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(set_a, set_sub_a), inclusion::superset );

    BOOST_CHECK_EQUAL( inclusion_compare(SetT(), set_a), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(set_sub_a, set_a), inclusion::subset );

    BOOST_CHECK_EQUAL( inclusion_compare(set_a, map_c), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare(map_c, set_a), inclusion::unrelated );

}


#endif // __test_itl_itl_map_h_JOFA_090119__

