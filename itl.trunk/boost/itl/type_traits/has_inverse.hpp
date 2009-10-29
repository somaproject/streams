/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_HAS_INVERSE_HPP_JOFA_090205
#define BOOST_ITL_TYPE_TRAITS_HAS_INVERSE_HPP_JOFA_090205

#include <boost/type_traits/is_signed.hpp>

namespace boost{ namespace itl
{

    template <class Type> struct has_inverse
    { 
        typedef has_inverse<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = (is_signed<Type>::value)); 
    };

}} // namespace boost itl

#endif // BOOST_ITL_TYPE_TRAITS_HAS_INVERSE_HPP_JOFA_090205


