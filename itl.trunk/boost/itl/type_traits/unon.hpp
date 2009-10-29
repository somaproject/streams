/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_UNON_HPP_JOFA_080912
#define BOOST_ITL_TYPE_TRAITS_UNON_HPP_JOFA_080912

#include <string>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/succ_pred.hpp>

namespace boost{ namespace itl
{
    template <class Type> struct unon{ static Type value(); };

    template<> inline float  unon<float>::value()  { return 1.0; }
    template<> inline double unon<double>::value() { return 1.0; }
    template<> inline long double unon<long double>::value() 
                                                   { return 1.0; }
    
    // Smallest 'visible' string that is greater than the empty string.
    template <>    
    inline std::string unon<std::string>::value(){ return std::string(" "); }

    template <class Type> 
    inline Type unon<Type>::value(){ return succ(neutron<Type>::value()); }

}} // namespace boost itl

#endif


