/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_ABSORBS_NEUTRONS_HPP_JOFA_081004
#define BOOST_ITL_TYPE_TRAITS_ABSORBS_NEUTRONS_HPP_JOFA_081004

namespace boost{ namespace itl
{
    template <class Type> struct absorbs_neutrons
    {
        typedef absorbs_neutrons<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = false); 
    };

}} // namespace boost itl

#endif


