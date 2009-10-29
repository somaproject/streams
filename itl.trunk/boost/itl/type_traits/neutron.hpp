/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_NEUTRON_HPP_JOFA_080912
#define BOOST_ITL_TYPE_TRAITS_NEUTRON_HPP_JOFA_080912

#include <boost/itl/type_traits/type_to_string.hpp>

// I DO NOT #include boost/itl/itl_<date_time_adapter>.hpp here, because it
// HAS TO be included by client code prior to this location.

namespace boost{ namespace itl
{
    template <class Type> struct neutron
    { 
        static Type value(); 
        Type operator()()const { return value(); }
    };

#ifdef ITL_NEEDS_GREGORIAN_DATE_NEUTRON_VALUE
#define ITL_HAS_GREGORIAN_DATE_NEUTRON_VALUE
    template<> 
    inline boost::gregorian::date neutron<boost::gregorian::date>::value()
    { 
        return boost::gregorian::date(boost::gregorian::min_date_time); 
    }

    template<> 
    struct neutron<boost::gregorian::date_duration>
    {
        static boost::gregorian::date_duration value()
        { 
            return boost::gregorian::date(boost::gregorian::min_date_time) 
                 - boost::gregorian::date(boost::gregorian::min_date_time); 
        }
    };
#endif

#ifdef ITL_NEEDS_POSIX_TIME_PTIME_NEUTRON_VALUE
#define ITL_HAS_POSIX_TIME_PTIME_NEUTRON_VALUE
    template<> 
    inline boost::posix_time::ptime neutron<boost::posix_time::ptime>::value()
    { 
        return boost::posix_time::ptime(boost::posix_time::min_date_time); 
    }
#endif

    template <class Type>
    inline Type neutron<Type>::value()
    { 
        return Type(); 
    }

    template<>
    inline std::string unary_template_to_string<neutron>::apply() { return "0"; }

}} // namespace boost itl

#define ITL_NEUTRONS_PROVIDED

#endif


