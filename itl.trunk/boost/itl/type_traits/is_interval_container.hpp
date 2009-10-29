/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_INTERVAL_CONTAINER_HPP_JOFA_081004
#define BOOST_ITL_TYPE_TRAITS_IS_INTERVAL_CONTAINER_HPP_JOFA_081004

#include <boost/mpl/and.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/itl/type_traits/is_map.hpp> 

namespace boost{ namespace itl
{
    template <class Type> struct is_interval_container
    { 
        typedef is_interval_container<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = false); 
    };

    template<class Type>
    struct is_interval_map
    {
        typedef is_interval_map<Type> type; 
        BOOST_STATIC_CONSTANT(bool, value =
            (mpl::and_<is_interval_container<Type>, is_map<Type> >::value)
            ); 
    };

    template<class Type>
    struct is_interval_set
    { 
        typedef is_interval_set<Type> type; 
        BOOST_STATIC_CONSTANT(bool, value =
            (mpl::and_<          is_interval_container<Type>,
                       mpl::not_<is_interval_map<Type> > >::value)
            ); 
    };


}} // namespace boost itl

#endif


