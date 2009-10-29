/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
Auxiliary functions to reduce redundancies in test case code.
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_FUNCTIONS_H_JOFA_091003
#define BOOST_ITL_TEST_FUNCTIONS_H_JOFA_091003

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/functors.hpp>

namespace boost{namespace itl
{

template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap,
          class SequenceT
>
void itl_map_copy(const SequenceT& segments, 
                  IntervalMap<T,U,Trt>& destination)
{
    ITL_const_FORALL(typename SequenceT, segment_, segments)
        destination.insert(*segment_);
}


template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap,
          class SequenceT
>
void test_interval_map_copy_via_inserter(const SequenceT& segments, IntervalMap<T,U,Trt>& std_copied_map)
{
    // The second parameter (std_copied_map) could be omitted and only held as a 
    // local variable. I is there to help gcc-3.4.4 resolving the function template type.
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    IntervalMapT looped_copied_map;
    std_copied_map.clear();
    itl_map_copy(segments, looped_copied_map);
    std::copy(segments.begin(), segments.end(), std::inserter(std_copied_map, std_copied_map.end()));
    BOOST_CHECK_EQUAL( looped_copied_map, std_copied_map );
}

}} // namespace itl boost

#endif // BOOST_ITL_TEST_FUNCTIONS_H_JOFA_091003

