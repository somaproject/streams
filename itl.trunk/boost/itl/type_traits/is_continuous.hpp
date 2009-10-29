/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_CONTINUOUS_HPP_JOFA_080910
#define BOOST_ITL_TYPE_TRAITS_IS_CONTINUOUS_HPP_JOFA_080910

#include <string>
#include <boost/config.hpp>

namespace boost{ namespace itl
{
    template <class Type> struct is_continuous;

    template<> struct is_continuous<float>
    {
        typedef is_continuous<float> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<double>
    {
        typedef is_continuous<double> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<long double>
    {
        typedef is_continuous<long double> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<std::string>
    {
        typedef is_continuous<std::string> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

#ifdef ITL_NEEDS_RATIONAL_IS_CONTINUOUS
#define ITL_HAS_RATIONAL_IS_CONTINUOUS
    template<class Integral> 
    struct is_continuous<boost::rational<Integral> >
    {
        typedef is_continuous<boost::rational<Integral> > type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };
#endif

    template <class Type> struct is_continuous
    {
        typedef is_continuous<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = false);
    };


}} // namespace boost itl

#endif


