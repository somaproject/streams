/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_set_itl_set_h_JOFA_090119__
#define __test_itl_set_itl_set_h_JOFA_090119__


//------------------------------------------------------------------------------
// Monoid EAN
//------------------------------------------------------------------------------
template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void itl_set_check_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef itl::set<T> SetT;

    IntervalSetT itv_set_a, itv_set_b, itv_set_c;
    itv_set_a.add(I_D(3,6)).add(I_I(5,7));
    itv_set_b.add(C_D(1,3)).add(I_D(8,9));
    itv_set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    SetT set_a, set_b, set_c;
    Interval::atomize(set_a, itv_set_a);
    Interval::atomize(set_b, itv_set_b);
    Interval::atomize(set_c, itv_set_c);

    T val1 = MK_v(7);
    T val2 = MK_v(5);

    CHECK_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, val1, val2);
    CHECK_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, val1, val2);
}


template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void itl_set_check_monoid_et_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef itl::set<T> SetT;

    IntervalSetT itv_set_a, itv_set_b, itv_set_c;
    itv_set_a.add(I_D(3,6)).add(I_I(5,7));
    itv_set_b.add(C_D(1,3)).add(I_D(8,9));
    itv_set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    SetT set_a, set_b, set_c;
    Interval::atomize(set_a, itv_set_a);
    Interval::atomize(set_b, itv_set_b);
    Interval::atomize(set_c, itv_set_c);

    T val1 = MK_v(7);
    T val2 = MK_v(5);

    CHECK_MONOID_INSTANCE_WRT(et)   (set_a, set_b, set_c, val1, val2);
    CHECK_MONOID_INSTANCE_WRT(caret)(set_a, set_b, set_c, val1, val2);
}

//------------------------------------------------------------------------------
// Abelian monoid EANC
//------------------------------------------------------------------------------

template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void itl_set_check_abelian_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef itl::set<T> SetT;

    IntervalSetT itv_set_a, itv_set_b, itv_set_c;
    itv_set_a.add(I_D(3,6)).add(I_I(5,7));
    itv_set_b.add(C_D(1,3)).add(I_D(8,9));
    itv_set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    SetT set_a, set_b, set_c;
    Interval::atomize(set_a, itv_set_a);
    Interval::atomize(set_b, itv_set_b);
    Interval::atomize(set_c, itv_set_c);

    T val1 = MK_v(7);
    T val2 = MK_v(5);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, val1, val2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, val1, val2);
}

template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void itl_set_check_abelian_monoid_et_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef itl::set<T> SetT;

    IntervalSetT itv_set_a, itv_set_b, itv_set_c;
    itv_set_a.add(I_D(3,6)).add(I_I(5,7));
    itv_set_b.add(C_D(1,3)).add(I_D(8,9));
    itv_set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    SetT set_a, set_b, set_c;
    Interval::atomize(set_a, itv_set_a);
    Interval::atomize(set_b, itv_set_b);
    Interval::atomize(set_c, itv_set_c);

    T val1 = MK_v(7);
    T val2 = MK_v(5);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(et)   (set_a, set_b, set_c, val1, val2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(caret)(set_a, set_b, set_c, val1, val2);
}


//------------------------------------------------------------------------------
// Abelian partial invertive monoid 
//------------------------------------------------------------------------------
template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void itl_set_check_partial_invertive_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef itl::set<T> SetT;

    IntervalSetT itv_set_a, itv_set_b, itv_set_c;
    itv_set_a.add(I_D(3,6)).add(I_I(5,7));
    itv_set_b.add(C_D(1,3)).add(I_D(8,9));
    itv_set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    SetT set_a, set_b, set_c;
    Interval::atomize(set_a, itv_set_a);
    Interval::atomize(set_b, itv_set_b);
    Interval::atomize(set_c, itv_set_c);

    T val1 = MK_v(7);
    T val2 = MK_v(5);

    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, val1, val2);
    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, val1, val2);
}


#endif // __test_itl_set_itl_set_h_JOFA_090119__

