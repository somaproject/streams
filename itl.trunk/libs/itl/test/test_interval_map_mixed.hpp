/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_interval_map_mixed_hpp_JOFA_081005__
#define __test_itl_interval_map_mixed_hpp_JOFA_081005__


//------------------------------------------------------------------------------
//- part1: Basic operations and predicates
//------------------------------------------------------------------------------

template <class T, class U> 
void interval_map_mixed_ctor_4_ordered_types()
{
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    U u1 = unon<U>::value();

    SplitIntervalMapT split_map(make_pair(v0,u1));
    IntervalMapT      join_map(split_map);

    BOOST_CHECK_EQUAL( split_map.lower(), join_map.lower() );
    BOOST_CHECK_EQUAL( split_map.upper(), join_map.upper() );
}

template <class T, class U> 
void interval_map_mixed_equal_4_ordered_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    U u1 = unon<U>::value();

    SplitIntervalMapT    split_empty, split_single(make_pair(v0,u1));
    IntervalMapT         join_empty, join_single(make_pair(v0,u1));

    // mixed ==-equality is a strange thing. Most times is does not
    // make sense. It is better to allow only for same type == equality.
    BOOST_CHECK_EQUAL( split_empty == split_empty, true );
    BOOST_CHECK_EQUAL( join_empty  == join_empty,  true );

    // There were Problems with operator== and emtpy sets.
    BOOST_CHECK_EQUAL( split_empty == split_single, false );
    BOOST_CHECK_EQUAL( join_empty  == join_single,  false );

    BOOST_CHECK_EQUAL( split_single == split_empty, false );
    BOOST_CHECK_EQUAL( join_single  == join_empty,  false );

    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_empty),  true );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_empty),  true );

    //--------------------------------------------------------------------------
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(split_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_single, join_empty),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_single, join_empty),  false );
}

template <class T, class U> 
void interval_map_mixed_assign_4_ordered_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();
    U u1 = unon<U>::value();

    mapping_pair<T,U> v0_u1(v0,u1);
    mapping_pair<T,U> v1_u1(v1,u1);

    SplitIntervalMapT split_map;
    IntervalMapT      join_map;
    split_map.add(v0_u1); //NOTE: make_pair(v0,u1); fails
    join_map = split_map; //=t T& T::operator=(const P&) ...


    BOOST_CHECK_EQUAL( split_map.lower(), join_map.lower() );
    BOOST_CHECK_EQUAL( split_map.upper(), join_map.upper() );

    SplitIntervalMapT split_self = SplitIntervalMapT().add(v0_u1);
    IntervalMapT      join_self  = IntervalMapT().add(v1_u1);

    split_self = split_self;
    join_self  = join_self;

    BOOST_CHECK_EQUAL( split_self, split_self );
    BOOST_CHECK_EQUAL( join_self, join_self );
}

template <class T, class U> 
void interval_map_mixed_ctor_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);


    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map(split_map);
}


template <class T, class U> 
void interval_map_mixed_assign_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map;
    join_map = split_map;
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 3 );
}


template <class T, class U> 
void interval_map_mixed_equal_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    IntervalMapT join_map;
    join_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    IntervalMapT join_map2 = join_map;    
    BOOST_CHECK_EQUAL( join_map, join_map2 );
    BOOST_CHECK_EQUAL( is_element_equal(join_map, join_map2), true );

    SplitIntervalMapT split_map;    
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    SplitIntervalMapT split_map2 = split_map;    
    BOOST_CHECK_EQUAL( split_map, split_map2 );
    BOOST_CHECK_EQUAL( is_element_equal(split_map2, split_map), true );

    BOOST_CHECK_EQUAL( is_element_equal(split_map, join_map),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_map,  split_map), true );
}


template <class T, class U, class Trt>
void partial_interval_map_mixed_inclusion_compare_4_bicremental_types()
{
    typedef interval_map<T,U,Trt> IntervalMapT;
    //--------------------------------------------------------------------------
    // equalities
    // { 0 1  2 3  4 5     8 9 }
    // {[0,2)[2,3](3,6)   (7,9]}
    //  ->2  ->1  ->1     ->2
    split_interval_map<T,U,Trt> split_map;
    interval_map<T,U,Trt> join_map;
    split_interval_set<T> split_set;
    separate_interval_set<T> sep_set;
    interval_set<T> join_set;

    split_map.add(IDv(0,2,2)).add(IIv(2,3,1)).add(CDv(3,6,1)).add(CIv(7,9,2));
    join_map = split_map;
    split_map.domain(split_set);
    split_map.domain(sep_set);
    split_map.domain(join_set);

    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    BOOST_CHECK_EQUAL( join_map.iterative_size(),  3 );
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 4 );
    BOOST_CHECK_EQUAL( sep_set.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_set.iterative_size(),  2 );

    BOOST_CHECK_EQUAL( inclusion_compare(split_map, join_map), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_map, split_map), inclusion::equal );

    BOOST_CHECK_EQUAL( inclusion_compare(split_map, split_set), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map, sep_set  ), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map, join_set ), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_map , split_set), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_map , sep_set  ), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_map , join_set ), inclusion::equal );

    BOOST_CHECK_EQUAL( inclusion_compare(split_set, split_map), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(sep_set  , split_map), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_set , split_map), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(split_set, join_map ), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(sep_set  , join_map ), inclusion::equal );
    BOOST_CHECK_EQUAL( inclusion_compare(join_set , join_map ), inclusion::equal );

    //--------------------------------------------------------------------------
    // inclusions
    // { 0  1   2  3   4  5       8 9 }
    // {[0,  2)[2, 3](3,   6)   (7, 9]}
    //    ->2    ->1    ->1       ->2
    // {[0,  2) [3,3](3,   6)   (7, 9]}
    //    ->2    ->1    ->1       ->2
    split_interval_map<T,U,Trt> split_sub_map1 = split_map;
    split_sub_map1.erase(MK_v(2));
    BOOST_CHECK_EQUAL( split_sub_map1.contains(MK_v(2)), false );

    interval_map<T,U,Trt> join_sub_map2;
    join_sub_map2 = split_map;
    join_sub_map2.erase(MK_v(1));
    BOOST_CHECK_EQUAL( join_sub_map2.contains(MK_v(1)), false );

    split_interval_set<T>    split_sub_set1; 
    separate_interval_set<T> sep_sub_set1; 
    interval_set<T>          join_sub_set1; 

    split_sub_map1.domain(split_sub_set1);
    split_sub_map1.domain(sep_sub_set1);
    split_sub_map1.domain(join_sub_set1);

    BOOST_CHECK_EQUAL( inclusion_compare(split_sub_map1, split_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_sub_map2, split_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(split_sub_map1,  join_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_sub_map2,  join_map), inclusion::subset );

    BOOST_CHECK_EQUAL( inclusion_compare(split_sub_set1, split_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(  sep_sub_set1, split_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_sub_set1, split_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(split_sub_set1,  join_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare(  sep_sub_set1,  join_map), inclusion::subset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_sub_set1,  join_map), inclusion::subset );

    BOOST_CHECK_EQUAL( inclusion_compare(split_map, split_sub_map1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map,  join_sub_map2), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map, split_sub_map1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map,  join_sub_map2), inclusion::superset );

    BOOST_CHECK_EQUAL( inclusion_compare(split_map, split_sub_set1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map,   sep_sub_set1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map,  join_sub_set1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map, split_sub_set1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map,   sep_sub_set1), inclusion::superset );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map,  join_sub_set1), inclusion::superset );

    split_interval_map<T,U,Trt> split_unrel_map11 = split_sub_map1;
    split_unrel_map11.set(CIv(7,9,1));
    BOOST_CHECK_EQUAL( split_unrel_map11(MK_v(8)), MK_u(1) );

    interval_map<T,U,Trt> join_unrel_map21 = join_sub_map2;
    join_unrel_map21.set(K_v(0,1));
    BOOST_CHECK_EQUAL( join_unrel_map21(MK_v(0)), MK_u(1) );

    BOOST_CHECK_EQUAL( inclusion_compare(split_unrel_map11, split_map), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare( join_unrel_map21, split_map), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare(split_unrel_map11,  join_map), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare( join_unrel_map21,  join_map), inclusion::unrelated );

    BOOST_CHECK_EQUAL( inclusion_compare(split_map, split_unrel_map11), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare(split_map,  join_unrel_map21), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map, split_unrel_map11), inclusion::unrelated );
    BOOST_CHECK_EQUAL( inclusion_compare( join_map,  join_unrel_map21), inclusion::unrelated );

    split_interval_map<T,U,Trt> split_unrel_map1 = split_sub_map1;
    split_unrel_map1.add(IDv(11,12,1));
    BOOST_CHECK_EQUAL( split_unrel_map1(MK_v(11)), MK_u(1) );

    interval_map<T,U,Trt> join_unrel_map2 = join_sub_map2;
    join_unrel_map2.add(K_v(6,1));
    BOOST_CHECK_EQUAL( join_unrel_map2(MK_v(6)), MK_u(1) );
}


template <class T, class U, class Trt>
void partial_interval_map_mixed_contains_4_bicremental_types()
{
    typedef interval_map<T,U,Trt> IntervalMapT;
    //--------------------------------------------------------------------------
    // { 0 1  2 3  4 5     8 9 }
    // {[0,2)[2,3](3,6)   (7,9]}
    //  ->2  ->1  ->1     ->2
    split_interval_map<T,U,Trt> split_map;
    interval_map<T,U,Trt> join_map;
    split_interval_set<T> split_set;
    separate_interval_set<T> sep_set;
    interval_set<T> join_set;

    split_map.add(IDv(0,2,2)).add(IIv(2,3,1)).add(CDv(3,6,1)).add(CIv(7,9,2));
    join_map = split_map;
    split_map.domain(split_set);
    split_map.domain(sep_set);
    split_map.domain(join_set);

    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    BOOST_CHECK_EQUAL( join_map.iterative_size(),  3 );
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 4 );
    BOOST_CHECK_EQUAL( sep_set.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_set.iterative_size(),  2 );

    // Key types
    BOOST_CHECK_EQUAL( split_map.contains(MK_v(0)), true );
    BOOST_CHECK_EQUAL( split_map.contains(MK_v(5)), true );
    BOOST_CHECK_EQUAL( split_map.contains(MK_v(9)), true );

    BOOST_CHECK_EQUAL( split_map.contains(I_D(2,3)), true );
    BOOST_CHECK_EQUAL( split_map.contains(I_D(0,6)), true );
    BOOST_CHECK_EQUAL( split_map.contains(I_D(0,7)), false );
    BOOST_CHECK_EQUAL(  join_map.contains(I_D(2,3)), true );
    BOOST_CHECK_EQUAL(  join_map.contains(I_D(0,6)), true );
    BOOST_CHECK_EQUAL(  join_map.contains(I_D(0,7)), false );

    // Map types
    BOOST_CHECK_EQUAL( join_map.contains(K_v(1,2)), true );
    BOOST_CHECK_EQUAL( join_map.contains(K_v(5,1)), true );
    BOOST_CHECK_EQUAL( join_map.contains(K_v(9,2)), true );

    BOOST_CHECK_EQUAL( split_map.contains(IDv(2,6,1)), true );
    BOOST_CHECK_EQUAL( split_map.contains(IDv(1,6,1)), false );
    BOOST_CHECK_EQUAL( split_map.contains(IIv(8,9,2)), true );
    BOOST_CHECK_EQUAL( split_map.contains(IIv(8,9,3)), false );
    BOOST_CHECK_EQUAL(  join_map.contains(IDv(2,6,1)), true );
    BOOST_CHECK_EQUAL(  join_map.contains(IDv(1,6,1)), false );
    BOOST_CHECK_EQUAL(  join_map.contains(IIv(8,9,2)), true );
    BOOST_CHECK_EQUAL(  join_map.contains(IIv(8,9,3)), false );

    BOOST_CHECK_EQUAL( split_map.contains(join_map), true );
    BOOST_CHECK_EQUAL( join_map.contains(split_map), true );
    BOOST_CHECK_EQUAL( split_map.contained_in(join_map), true );
    BOOST_CHECK_EQUAL( join_map.contained_in(split_map), true );

    //--------------------------------------------------------------------------
    // inclusions
    // { 0  1   2  3   4  5       8 9 }
    // {[0,  2)[2, 3](3,   6)   (7, 9]}
    //    ->2    ->1    ->1       ->2
    // {[0,  2) [3,3](3,   6)   (7, 9]}
    //    ->2    ->1    ->1       ->2
    split_interval_map<T,U,Trt> split_sub_map1 = split_map;
    split_sub_map1.erase(MK_v(2));
    BOOST_CHECK_EQUAL( split_sub_map1.contains(MK_v(2)), false );

    interval_map<T,U,Trt> join_sub_map2;
    join_sub_map2 = split_map;
    join_sub_map2.erase(MK_v(1));
    BOOST_CHECK_EQUAL( join_sub_map2.contains(MK_v(1)), false );

    split_interval_set<T>    split_sub_set1; 
    separate_interval_set<T> sep_sub_set1; 
    interval_set<T>          join_sub_set1; 

    split_sub_map1.domain(split_sub_set1);
    split_sub_map1.domain(sep_sub_set1);
    split_sub_map1.domain(join_sub_set1);

    BOOST_CHECK_EQUAL( split_sub_map1.contained_in(split_map), true );
    BOOST_CHECK_EQUAL(  join_sub_map2.contained_in(split_map), true );
    BOOST_CHECK_EQUAL( split_sub_map1.contained_in(join_map ), true );
    BOOST_CHECK_EQUAL(  join_sub_map2.contained_in(join_map ), true );

    BOOST_CHECK_EQUAL( split_map.contains(split_sub_map1), true );
    BOOST_CHECK_EQUAL( split_map.contains( join_sub_map2), true );
    BOOST_CHECK_EQUAL(  join_map.contains(split_sub_map1), true );
    BOOST_CHECK_EQUAL(  join_map.contains( join_sub_map2), true );

    BOOST_CHECK_EQUAL( split_map.contains(split_sub_set1), true );
    BOOST_CHECK_EQUAL( split_map.contains(  sep_sub_set1), true );
    BOOST_CHECK_EQUAL( split_map.contains( join_sub_set1), true );
    BOOST_CHECK_EQUAL(  join_map.contains(split_sub_set1), true );
    BOOST_CHECK_EQUAL(  join_map.contains(  sep_sub_set1), true );
    BOOST_CHECK_EQUAL(  join_map.contains( join_sub_set1), true );

    split_interval_map<T,U,Trt> split_unrel_map11 = split_sub_map1;
    split_unrel_map11.set(CIv(7,9,1));
    BOOST_CHECK_EQUAL( split_unrel_map11(MK_v(8)), MK_u(1) );

    interval_map<T,U,Trt> join_unrel_map21 = join_sub_map2;
    join_unrel_map21.set(K_v(0,1));
    BOOST_CHECK_EQUAL( join_unrel_map21(MK_v(0)), MK_u(1) );

    BOOST_CHECK_EQUAL( split_unrel_map11.contains(split_map), false );
    BOOST_CHECK_EQUAL(  join_unrel_map21.contains(split_map), false );
    BOOST_CHECK_EQUAL( split_unrel_map11.contains( join_map), false );
    BOOST_CHECK_EQUAL(  join_unrel_map21.contains( join_map), false );

    BOOST_CHECK_EQUAL( split_unrel_map11.contained_in(split_map), false );
    BOOST_CHECK_EQUAL(  join_unrel_map21.contained_in(split_map), false );
    BOOST_CHECK_EQUAL( split_unrel_map11.contained_in( join_map), false );
    BOOST_CHECK_EQUAL(  join_unrel_map21.contained_in( join_map), false );

    BOOST_CHECK_EQUAL( split_map.contains(split_unrel_map11), false );
    BOOST_CHECK_EQUAL( split_map.contains( join_unrel_map21), false );
    BOOST_CHECK_EQUAL(  join_map.contains(split_unrel_map11), false );
    BOOST_CHECK_EQUAL(  join_map.contains( join_unrel_map21), false );

}

//------------------------------------------------------------------------------
//- part2: Operations
//------------------------------------------------------------------------------

template <class T, class U>
void interval_map_mixed_add_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);
    mapping_pair<T,U> v1_1(v1, u1);
    mapping_pair<T,U> v3_1(v3, u1);
    mapping_pair<T,U> v5_1(v5, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1);
    split_map += I4_5D_1;
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map;
    join_map += split_map;
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 3 );

    IntervalMapT join_map3;
    join_map3.add(v1_1).add(v3_1);
    join_map3 += v5_1;
    BOOST_CHECK_EQUAL( join_map3.iterative_size(), 3 );
    SplitIntervalMapT split_map3;
    split_map3 += join_map3;
    BOOST_CHECK_EQUAL( split_map3.iterative_size(), 3 );
}

template <class T, class U> 
void interval_map_mixed_add2_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);
    mapping_pair<T,U> v1_1(v1, u1);
    mapping_pair<T,U> v3_1(v3, u1);
    mapping_pair<T,U> v5_1(v5, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1);
    split_map |= I4_5D_1;
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map;
    join_map |= split_map;
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 3 );

    IntervalMapT join_map3;
    join_map3.add(v1_1).add(v3_1);
    join_map3 |= v5_1;
    BOOST_CHECK_EQUAL( join_map3.iterative_size(), 3 );
    SplitIntervalMapT split_map3;
    split_map3 |= join_map3;
    BOOST_CHECK_EQUAL( split_map3.iterative_size(), 3 );
}


template <class T, class U> 
void interval_map_mixed_subtract_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = interval<T>::rightopen(v0,v4);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I7_8D = interval<T>::rightopen(v7,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);
    interval<T> I8_9I =    interval<T>::closed(v8,v9);

    std::pair<interval<T>,U> I0_4D_1(I0_4D, u1);
    std::pair<interval<T>,U> I2_6D_1(I2_6D, u1);
    std::pair<interval<T>,U> I3_6D_1(I3_6D, u1);
    std::pair<interval<T>,U> I5_7D_1(I5_7D, u1);
    std::pair<interval<T>,U> I7_8D_1(I7_8D, u1);
    std::pair<interval<T>,U> I8_9D_1(I8_9D, u1);
    std::pair<interval<T>,U> I8_9I_1(I8_9I, u1);

    SplitIntervalMapT split_map;
    split_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 7 );

    IntervalMapT join_map;
    join_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 5 );

    // Make maps to be subtracted
    SplitIntervalMapT split_sub;
    split_sub.add(I3_6D_1).add(I8_9D_1);

    IntervalMapT join_sub;
    join_sub.add(I3_6D_1).add(I8_9D_1);

    //--------------------------------------------------------------------------
    // Test for split_interval_map
    SplitIntervalMapT    split_diff = split_map;
    IntervalMapT         join_diff  = join_map;

    //subtraction combinations
    split_diff -= split_sub;
    join_diff  -= split_sub;

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 7 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  5 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff, split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff), true );

    //--------------------------------------------------------------------------
    // Test for interval_map. Reinitialize
    split_diff = split_map;
    join_diff  = join_map;

    //subtraction combinations
    split_diff -= join_sub;
    join_diff  -= join_sub;

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 7 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  5 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),   true );
}


template <class T, class U> 
void interval_map_mixed_erase_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = interval<T>::rightopen(v0,v4);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I7_8D = interval<T>::rightopen(v7,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);
    interval<T> I8_9I =    interval<T>::closed(v8,v9);

    std::pair<interval<T>,U> I0_4D_1(I0_4D, u1);
    std::pair<interval<T>,U> I2_6D_1(I2_6D, u1);
    std::pair<interval<T>,U> I3_6D_1(I3_6D, u1);
    std::pair<interval<T>,U> I5_7D_1(I5_7D, u1);
    std::pair<interval<T>,U> I7_8D_1(I7_8D, u1);
    std::pair<interval<T>,U> I8_9D_1(I8_9D, u1);
    std::pair<interval<T>,U> I8_9I_1(I8_9I, u1);

    SplitIntervalMapT split_map;
    split_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 7 );

    IntervalMapT join_map;
    join_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 5 );

    // Make sets to be erased
    SplitIntervalMapT split_sub;
    split_sub.add(I3_6D_1).add(I8_9D_1);

    IntervalMapT join_sub;
    join_sub.add(I3_6D_1).add(I8_9D_1);

    //--------------------------------------------------------------------------
    // Test for split_interval_map
    SplitIntervalMapT     split_diff = split_map;
    IntervalMapT          join_diff  = join_map;

    //subtraction combinations
    erase(split_diff, split_sub);
    erase(join_diff,  split_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 6 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(), 5 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff, split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff), true );

    //--------------------------------------------------------------------------
    // Test for interval_map. Reinitialize
    split_diff = split_map;
    join_diff  = join_map;

    //subtraction combinations
    erase(split_diff, join_sub);
    erase(join_diff, join_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 6 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  5 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),   true );
}


template <class T, class U> 
void interval_map_mixed_erase2_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>            IntervalSetT;
    typedef split_interval_set<T>    SplitIntervalSetT;
    U u1 = make<U>(1);

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = interval<T>::rightopen(v0,v4);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I7_8D = interval<T>::rightopen(v7,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);
    interval<T> I8_9I =    interval<T>::closed(v8,v9);

    std::pair<interval<T>,U> I0_4D_1(I0_4D, u1);
    std::pair<interval<T>,U> I2_6D_1(I2_6D, u1);
    std::pair<interval<T>,U> I3_6D_1(I3_6D, u1);
    std::pair<interval<T>,U> I5_7D_1(I5_7D, u1);
    std::pair<interval<T>,U> I7_8D_1(I7_8D, u1);
    std::pair<interval<T>,U> I8_9D_1(I8_9D, u1);
    std::pair<interval<T>,U> I8_9I_1(I8_9I, u1);

    SplitIntervalMapT split_map;
    split_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 7 );

    IntervalMapT join_map;
    join_map.add(I0_4D_1).add(I2_6D_1).add(I5_7D_1).add(I7_8D_1).add(I8_9I_1);
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 5 );

    // Make sets to be erased
    SplitIntervalSetT split_sub;
    split_sub.add(I3_6D).add(I8_9D);

    IntervalSetT join_sub;
    join_sub.add(I3_6D).add(I8_9D);

    //--------------------------------------------------------------------------
    // Test for split_interval_map
    SplitIntervalMapT     split_diff  = split_map;
    IntervalMapT          join_diff   = join_map;
    SplitIntervalMapT     split_diff2 = split_map;
    IntervalMapT          join_diff2  = join_map;

    //erase combinations
    erase(split_diff, split_sub);
    erase(join_diff,  split_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(), 4 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff, split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff), true );

    //subtraction combinations
    split_diff2 -= split_sub;
    join_diff2  -= split_sub;

    BOOST_CHECK_EQUAL( split_diff2.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( join_diff2.iterative_size(), 4 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff2, split_diff2), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff2, join_diff2),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff2,  split_diff2), true );

    //--------------------------------------------------------------------------
    // Test for interval_map. Reinitialize
    split_diff  = split_map;
    join_diff   = join_map;
    split_diff2 = split_map;
    join_diff2  = join_map;

    //erase combinations
    erase(split_diff, join_sub);
    erase(join_diff, join_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  4 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),   true );

    //subtraction combinations
    split_diff2 -= join_sub;
    join_diff2  -= join_sub;

    BOOST_CHECK_EQUAL( split_diff2.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( join_diff2.iterative_size(),  4 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff2,  join_diff2),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff2,  split_diff2),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff2, join_diff2),   true );
}


template <class T, class U> 
void interval_map_mixed_insert_erase_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = interval<T>::rightopen(v0,v4);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I7_8D = interval<T>::rightopen(v7,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);
    interval<T> I8_9I =    interval<T>::closed(v8,v9);

    std::pair<interval<T>,U> I0_4D_1(I0_4D, u1);
    std::pair<interval<T>,U> I2_6D_1(I2_6D, u1);
    std::pair<interval<T>,U> I3_6D_1(I3_6D, u1);
    std::pair<interval<T>,U> I5_7D_1(I5_7D, u1);
    std::pair<interval<T>,U> I7_8D_1(I7_8D, u1);
    std::pair<interval<T>,U> I8_9D_1(I8_9D, u1);
    std::pair<interval<T>,U> I8_9I_1(I8_9I, u1);

    SplitIntervalMapT split_A, split_B, split_all, split_X;
    IntervalMapT      join_A,  join_B,  join_all,  join_X;

    split_all.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1).insert(I7_8D_1).insert(I8_9I_1);
    split_A.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1);
    split_B.insert(I7_8D_1).insert(I8_9I_1);

    join_all.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1).insert(I7_8D_1).insert(I8_9I_1);
    join_A.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1);
    join_B.insert(I7_8D_1).insert(I8_9I_1);

    //-------------------------------------------------------------------------
    insert(split_X, split_A);
    BOOST_CHECK_EQUAL( split_X, split_A );
    insert(split_X, split_B);
    BOOST_CHECK_EQUAL( split_X, split_all );

    erase(split_X, split_B);
    BOOST_CHECK_EQUAL( split_X, split_A );
    erase(split_X, split_A);
    BOOST_CHECK_EQUAL( split_X, SplitIntervalMapT() );

    //-------------------------------------------------------------------------
    insert(join_X, join_A);
    BOOST_CHECK_EQUAL( join_X, join_A );
    insert(join_X, join_B);
    BOOST_CHECK_EQUAL( join_X, join_all );

    erase(join_X, join_B);
    BOOST_CHECK_EQUAL( join_X, join_A );
    erase(join_X, join_A);
    BOOST_CHECK_EQUAL( join_X, IntervalMapT() );

    //-------------------------------------------------------------------------
    split_X.clear();
    insert(split_X, join_A);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_A), true );
    insert(split_X, join_B);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_all), true );

    erase(split_X, join_B);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_A), true );
    erase(split_X, join_A);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, SplitIntervalMapT()), true );

    //-------------------------------------------------------------------------
    split_X.clear();
    insert(join_X, split_A);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, split_A), true );
    insert(join_X, split_B);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, join_all), true );

    erase(join_X, split_B);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, split_A), true );
    erase(join_X, split_A);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, IntervalMapT()), true );
}

template <class T, class U> 
void interval_map_mixed_insert_erase2_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>            IntervalSetT;
    typedef split_interval_set<T>   SplitIntervalSetT;
    U u1 = make<U>(1);

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = interval<T>::rightopen(v0,v4);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I7_8D = interval<T>::rightopen(v7,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);
    interval<T> I8_9I =    interval<T>::closed(v8,v9);

    std::pair<interval<T>,U> I0_4D_1(I0_4D, u1);
    std::pair<interval<T>,U> I2_6D_1(I2_6D, u1);
    std::pair<interval<T>,U> I3_6D_1(I3_6D, u1);
    std::pair<interval<T>,U> I5_7D_1(I5_7D, u1);
    std::pair<interval<T>,U> I7_8D_1(I7_8D, u1);
    std::pair<interval<T>,U> I8_9D_1(I8_9D, u1);
    std::pair<interval<T>,U> I8_9I_1(I8_9I, u1);

    SplitIntervalMapT split_A, split_B, split_all, split_X;
    IntervalMapT      join_A,  join_B,  join_all,  join_X;
    SplitIntervalSetT split_dA, split_dB;
    IntervalSetT      join_dA,  join_dB;

    split_all.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1).insert(I7_8D_1).insert(I8_9I_1);
    split_A.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1);
    split_B.insert(I7_8D_1).insert(I8_9I_1);

    join_all.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1).insert(I7_8D_1).insert(I8_9I_1);
    join_A.insert(I0_4D_1).insert(I2_6D_1).insert(I5_7D_1);
    join_B.insert(I7_8D_1).insert(I8_9I_1);

    split_A.domain(split_dA);
    split_B.domain(split_dB);
    join_A.domain(join_dA);
    join_B.domain(join_dB);

    //-------------------------------------------------------------------------
    insert(split_X, split_A);
    BOOST_CHECK_EQUAL( split_X, split_A );
    insert(split_X, split_B);
    BOOST_CHECK_EQUAL( split_X, split_all );

    erase(split_X, split_dB);
    BOOST_CHECK_EQUAL( split_X, split_A );
    erase(split_X, split_dA);
    BOOST_CHECK_EQUAL( split_X, SplitIntervalMapT() );

    //-------------------------------------------------------------------------
    insert(join_X, join_A);
    BOOST_CHECK_EQUAL( join_X, join_A );
    insert(join_X, join_B);
    BOOST_CHECK_EQUAL( join_X, join_all );

    erase(join_X, join_dB);
    BOOST_CHECK_EQUAL( join_X, join_A );
    erase(join_X, join_dA);
    BOOST_CHECK_EQUAL( join_X, IntervalMapT() );

    //-------------------------------------------------------------------------
    split_X.clear();
    insert(split_X, join_A);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_A), true );
    insert(split_X, join_B);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_all), true );

    erase(split_X, join_dB);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, split_A), true );
    erase(split_X, join_dA);
    BOOST_CHECK_EQUAL( is_element_equal(split_X, SplitIntervalMapT()), true );

    //-------------------------------------------------------------------------
    split_X.clear();
    insert(join_X, split_A);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, split_A), true );
    insert(join_X, split_B);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, join_all), true );

    erase(join_X, split_dB);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, split_A), true );
    erase(join_X, split_dA);
    BOOST_CHECK_EQUAL( is_element_equal(join_X, IntervalMapT()), true );
}

template <class T, class U> 
void interval_map_mixed_basic_intersect_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>         IntervalSetT;
    typedef split_interval_set<T>   SplitIntervalSetT;
    U u1 = make<U>(1);
    U u2 = make<U>(2);
    U u3 = make<U>(3);

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);


    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I1_8D = interval<T>::rightopen(v1,v8);
    interval<T> I2_7D = interval<T>::rightopen(v2,v7);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I6_7D = interval<T>::rightopen(v6,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);

    std::pair<interval<T>,U> I0_3D_1(I0_3D, u1);
    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I1_3D_2(I1_3D, u2);
    std::pair<interval<T>,U> I1_8D_1(I1_8D, u1);
    std::pair<const interval<T>,U> I2_7D_1(I2_7D, u1);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I2_3D_3(I2_3D, u3);
    std::pair<interval<T>,U> I6_7D_1(I6_7D, u1);
    std::pair<interval<T>,U> I6_7D_3(I6_7D, u3);
    std::pair<interval<T>,U> I6_8D_1(I6_8D, u1);
    std::pair<interval<T>,U> I6_8D_2(I6_8D, u2);
    std::pair<interval<T>,U> I6_9D_1(I6_9D, u1);

    //--------------------------------------------------------------------------
    // split_interval_map
    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //         &=      [1                8)
    //split_AB ->      [1   3)       [6  8)
    //         &=        [2             7)     
    //         ->        [2 3)       [6 7)
    SplitIntervalMapT split_A, split_B, split_AB, split_ab, split_ab2;

    split_A.add(I0_3D_1).add(I6_9D_1);
    split_AB = split_A;
    split_AB &= I1_8D_1;
    split_ab.add(I1_3D_2).add(I6_8D_2);

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    (split_AB &= I1_8D_1) &= I2_7D_1;
    split_ab2.add(I2_3D_3).add(I6_7D_3);

    BOOST_CHECK_EQUAL( split_AB, split_ab2 );


    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //                  1                1
    //         &=       1
    //                  1
    //split_AB ->      [1]
    //                  2
    //         +=         (1             7)
    //                            2
    //         ->      [1](1             7)
    //                  2         2
    split_A.clear();
    split_A.add(I0_3D_1).add(I6_9D_1);
    split_AB = split_A;
    split_AB &= mapping_pair<T,U>(v1,u1);
    split_ab.clear();
    split_ab.add(mapping_pair<T,U>(v1,u2));

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    split_AB &= mapping_pair<T,U>(v1,u1);
    split_AB += make_pair(interval<T>::open(v1,v7), u2);
    split_ab2.clear();
    split_ab2 += make_pair(interval<T>::rightopen(v1,v7), u2);

    BOOST_CHECK_EQUAL( is_element_equal(split_AB, split_ab2), true );
}


template <class T, class U> 
void interval_map_mixed_basic_intersect2_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>         IntervalSetT;
    typedef split_interval_set<T>   SplitIntervalSetT;
    U u1 = make<U>(1);
    U u2 = make<U>(2);
    U u3 = make<U>(3);

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);


    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I1_8D = interval<T>::rightopen(v1,v8);
    interval<T> I2_7D = interval<T>::rightopen(v2,v7);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I6_7D = interval<T>::rightopen(v6,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);

    std::pair<interval<T>,U> I0_3D_1(I0_3D, u1);
    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I1_3D_2(I1_3D, u2);
    std::pair<interval<T>,U> I1_8D_1(I1_8D, u1);
    std::pair<interval<T>,U> I2_7D_1(I2_7D, u1);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I2_3D_3(I2_3D, u3);
    std::pair<interval<T>,U> I6_7D_1(I6_7D, u1);
    std::pair<interval<T>,U> I6_7D_3(I6_7D, u3);
    std::pair<interval<T>,U> I6_8D_1(I6_8D, u1);
    std::pair<interval<T>,U> I6_8D_2(I6_8D, u2);
    std::pair<interval<T>,U> I6_9D_1(I6_9D, u1);

    //--------------------------------------------------------------------------
    // split_interval_map
    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //         &=      [1                8)
    //split_AB ->      [1   3)       [6  8)
    //         &=        [2             7)     
    //         ->        [2 3)       [6 7)
    SplitIntervalMapT split_A, split_B, split_AB, split_ab, split_ab2;

    split_A.add(I0_3D_1).add(I6_9D_1);
    split_AB = split_A;
    split_AB &= I1_8D;
    split_ab.add(I1_3D_1).add(I6_8D_1);

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    (split_AB &= I1_8D) &= I2_7D;
    split_ab2.add(I2_3D_1).add(I6_7D_1);

    BOOST_CHECK_EQUAL( split_AB, split_ab2 );

    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //                  1                1
    //         &=       1
    //                  1
    //split_AB ->      [1]
    //                  2
    //         +=         (1             7)
    //                            2
    //         ->      [1](1             7)
    //                  2         2
    split_A.clear();
    split_A.add(I0_3D_1).add(I6_9D_1);
    split_AB = split_A;
    split_AB &= v1;
    split_ab.clear();
    split_ab.add(mapping_pair<T,U>(v1,u1));

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    split_AB &= interval<T>(v1);
    split_AB += make_pair(interval<T>::open(v1,v7), u1);
    split_ab2.clear();
    split_ab2 += make_pair(interval<T>::rightopen(v1,v7), u1);

    BOOST_CHECK_EQUAL( is_element_equal(split_AB, split_ab2), true );

    split_interval_map<T,U> left, right;
    left. add(IDv(0,2,2));
    right.add(IDv(0,2,2));
    BOOST_CHECK_EQUAL( is_element_equal(left, right), true );

    split_interval_set<T> left2, right2;
    left2. add(I_D(0,2));
    right2.add(I_D(0,1));
    is_element_equal(left2, right2);
    BOOST_CHECK_EQUAL( is_element_equal(left2, right2), false );
}


template <class T, class U> 
void interval_map_mixed_intersect_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>         IntervalSetT;
    typedef split_interval_set<T>    SplitIntervalSetT;
    U u1 = make<U>(1);
    U u2 = make<U>(2);


    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);

    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_2D = interval<T>::rightopen(v1,v2);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I5_8D = interval<T>::rightopen(v5,v8);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);

    std::pair<interval<T>,U> I0_3D_1(I0_3D, u1);
    std::pair<interval<T>,U> I1_2D_1(I1_2D, u1);
    std::pair<interval<T>,U> I1_2D_2(I1_2D, u2);
    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I1_3D_2(I1_3D, u2);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I2_3D_2(I2_3D, u2);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I5_8D_1(I5_8D, u1);
    std::pair<interval<T>,U> I6_8D_1(I6_8D, u1);
    std::pair<interval<T>,U> I6_8D_2(I6_8D, u2);
    std::pair<interval<T>,U> I6_9D_1(I6_9D, u1);

    //--------------------------------------------------------------------------
    // split_interval_set
    //--------------------------------------------------------------------------
    //split_A      [0          3)       [6   9)
    //                    1                1
    //split_B  &=      [1 2)[2    4) [5    8)
    //                   1     1         1
    //split_AB ->      [1 2)[2 3)       [6 8)
    //                   2    2           2
    SplitIntervalMapT    split_A, split_B, split_AB, split_ab, split_ab_jn;
    IntervalMapT         join_A,  join_B,  join_AB,  join_ab;

    split_A.add(I0_3D_1).add(I6_9D_1);
    split_B.add(I1_2D_1).add(I2_4D_1).add(I5_8D_1);
    split_ab.add(I1_2D_2).add(I2_3D_2).add(I6_8D_2);
    split_ab_jn.add(I1_3D_2).add(I6_8D_2);
    split_AB = split_A;
    split_AB &= split_B;
    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( split_AB, split_ab );
    
    //split_A      [0          3)       [6   9)
    //                    1                1
    //join_B   &=      [1         4) [5    8)
    //                        1         1
    //split_AB ->      [1      3)       [6 8)
    //                      2             2
    split_AB = split_A;
    join_B = split_B;
    split_AB &= join_B;

    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( split_AB, split_ab_jn );
}



template <class T, class U> 
void interval_map_mixed_intersect2_4_bicremental_types()
{         
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>            IntervalSetT;
    typedef split_interval_set<T>    SplitIntervalSetT;
    U u1 = make<U>(1);
    U u2 = make<U>(2);


    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);

    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_2D = interval<T>::rightopen(v1,v2);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I5_8D = interval<T>::rightopen(v5,v8);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);

    std::pair<interval<T>,U> I0_3D_1(I0_3D, u1);
    std::pair<interval<T>,U> I1_2D_1(I1_2D, u1);
    std::pair<interval<T>,U> I1_2D_2(I1_2D, u2);
    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I1_3D_2(I1_3D, u2);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I2_3D_2(I2_3D, u2);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I5_8D_1(I5_8D, u1);
    std::pair<interval<T>,U> I6_8D_1(I6_8D, u1);
    std::pair<interval<T>,U> I6_8D_2(I6_8D, u2);
    std::pair<interval<T>,U> I6_9D_1(I6_9D, u1);

    //--------------------------------------------------------------------------
    // split_interval_set
    //--------------------------------------------------------------------------
    //split_A      [0          3)       [6   9)
    //                    1                1
    //split_B  &=      [1 2)[2    4) [5    8)
    //split_AB ->      [1 2)[2 3)       [6 8)
    //                   1    1           1
    SplitIntervalMapT    split_A, split_AB, split_ab, split_ab_jn;
    SplitIntervalSetT    split_B;
    IntervalMapT         join_A, join_AB,  join_ab;
    IntervalSetT         join_B;

    split_A.add(I0_3D_1).add(I6_9D_1);
    split_B.add(I1_2D).add(I2_4D).add(I5_8D);
    split_ab.add(I1_2D_1).add(I2_3D_1).add(I6_8D_1);
    split_ab_jn.add(I1_3D_1).add(I6_8D_1);
    split_AB = split_A;
    split_AB &= split_B;
    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( split_AB, split_ab );
    
    //split_A      [0          3)       [6   9)
    //                    1                1
    //join_B   &=      [1         4) [5    8)
    //split_AB ->      [1      3)       [6 8)
    //                      1             1
    split_AB = split_A;
    join_B = split_B;
    split_AB &= join_B;

    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( split_AB, split_ab_jn );
}


template <class T, class U> 
void interval_map_mixed_disjoint_4_bicremental_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>         IntervalSetT;
    typedef split_interval_set<T>   SplitIntervalSetT;
    U u1 = make<U>(1);
    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v6 = make<T>(6);

    interval<T> I0_2D = interval<T>::rightopen(v0,v2);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I3_4D = interval<T>::rightopen(v3,v4);
    interval<T> I4_4I = interval<T>::closed(v4,v4);
    interval<T> C4_6D = interval<T>::open(v4,v6);
    interval<T> I6_6I = interval<T>::closed(v6,v6);

    std::pair<interval<T>,U> I0_2D_1(I0_2D, u1);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I3_4D_1(I3_4D, u1);
    std::pair<interval<T>,U> I4_4I_1(I4_4I, u1);
    std::pair<interval<T>,U> C4_6D_1(C4_6D, u1);
    std::pair<interval<T>,U> I6_6I_1(I6_6I, u1);

    //--------------------------------------------------------------------------
    //split_A: [0  2)          [4 4]      [6 6]
    //split_B:       [2 3)[3 4)     (4  6)
    SplitIntervalMapT split_A, split_B;

    split_A.add(I0_2D_1).add(I4_4I_1).add(I6_6I_1);
    split_B.add(I2_3D_1).add(I3_4D_1).add(C4_6D_1);

    IntervalMapT join_A(split_A), join_B(split_B);

    BOOST_CHECK_EQUAL( is_disjoint(split_A, split_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(split_A, join_B),  true );

    BOOST_CHECK_EQUAL( is_disjoint(join_A,  split_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(join_A,  join_B),  true );
}

template<class Type>
struct size_greater_1 : public itl::property<Type>
{
    bool operator()(const Type& value)const
    {
        return value.first.size() > 1 ;
    }
};


template <class T, class U> 
void interval_map_mixed_erase_if_4_integral_types()
{         
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    typedef interval_set<T>         IntervalSetT;
    typedef split_interval_set<T>   SplitIntervalSetT;
    U u1 = make<U>(1);
    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v6 = make<T>(6);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I3_4D = interval<T>::rightopen(v3,v4);
    interval<T> I4_4I = interval<T>::closed(v4,v4);
    interval<T> C4_6D = interval<T>::open(v4,v6);
    interval<T> I6_6I = interval<T>::closed(v6,v6);

    std::pair<interval<T>,U> I0_3D_1(I0_3D, u1);
    std::pair<interval<T>,U> I2_3D_1(I2_3D, u1);
    std::pair<interval<T>,U> I3_4D_1(I3_4D, u1);
    std::pair<interval<T>,U> I4_4I_1(I4_4I, u1);
    std::pair<interval<T>,U> C4_6D_1(C4_6D, u1);
    std::pair<interval<T>,U> I6_6I_1(I6_6I, u1);

    //--------------------------------------------------------------------------
    //split_A: [0  2)          [4 4]      [6 6]
    //split_B:       [2 3)[3 4)     (4  6)
    SplitIntervalMapT split_A, split_B;

    split_A.add(I0_3D_1).add(I4_4I_1).add(I6_6I_1);
    split_B.add(I4_4I_1).add(I6_6I_1);

    split_A.erase_if(size_greater_1<typename SplitIntervalMapT::value_type>());

    BOOST_CHECK_EQUAL( split_A, split_B );
}

//------------------------------------------------------------------------------
//- infix operators
//------------------------------------------------------------------------------

template <class T, class U> 
void interval_map_mixed_infix_plus_overload_4_bicremental_types()
{
    typedef interval_map<T,U>  IntervalMapT;
    interval_map<T,U>          join_a;
    split_interval_map<T,U>    split_a;

    join_a .add(CDv(1,3,1)).add(IDv(8,9,1)).add(IIv(6,11,3));
    split_a.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    BOOST_CHECK_EQUAL(split_a + join_a, join_a + split_a);
}

template <class T, class U> 
void interval_map_mixed_infix_pipe_overload_4_bicremental_types()
{
    typedef interval_map<T,U>  IntervalMapT;
    interval_map<T,U>          join_a;
    split_interval_map<T,U>    split_a;

    join_a .add(CDv(1,3,1)).add(IDv(8,9,1)).add(IIv(6,11,3));
    split_a.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    BOOST_CHECK_EQUAL(split_a | join_a, join_a | split_a);
}

template <class T, class U> 
void interval_map_mixed_infix_minus_overload_4_bicremental_types()
{
    typedef interval_map<T,U>  IntervalMapT;
    interval_map<T,U>          join_a, join_b;
    split_interval_map<T,U>    split_a, split_b;

    join_a .add(CDv(1,3,1)).add(IDv(8,9,1)).add(IIv(6,11,3));
    split_a.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    join_b .add(CDv(1,3,1)).add(IIv(6,11,3));
    split_b.add(IDv(0,9,2)).add(IIv(3,6,1));

    BOOST_CHECK_EQUAL(split_a - join_a, (split_b = split_a) -= join_a);
    BOOST_CHECK_EQUAL(split_a - join_a, split_b);

    BOOST_CHECK_EQUAL(join_a - split_a, (join_b = join_a) -= split_a);
    BOOST_CHECK_EQUAL(join_a - split_a, join_b);
}

template <class T, class U> 
void interval_map_mixed_infix_et_overload_4_bicremental_types()
{
    typedef interval_map<T,U>  IntervalMapT;
    interval_map<T,U>          join_a, join_b;
    split_interval_map<T,U>    split_a, split_b;

    join_a .add(CDv(1,3,1)).add(IDv(8,9,1)).add(IIv(6,11,3));
    split_a.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

    BOOST_CHECK_EQUAL(split_a & join_a, join_a & split_a);
    BOOST_CHECK_EQUAL(split_a & join_a, (split_b = split_a) &= join_a);
    BOOST_CHECK_EQUAL(split_a & join_a, split_b);

    BOOST_CHECK_EQUAL(join_a & split_a, (split_b = split_a) &= join_a);
    BOOST_CHECK_EQUAL(join_a & split_a, split_b);
}



#endif // __test_itl_interval_map_mixed_hpp_JOFA_081005__


