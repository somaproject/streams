/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_SIZE_HPP_JOFA_080911
#define BOOST_ITL_TYPE_TRAITS_SIZE_HPP_JOFA_080911

// I DO NOT #include boost/itl/itl_<date_time_adapter>.hpp here, because it
// HAS TO be included by client code prior to this location.

namespace boost{ namespace itl
{
    template <class Type> struct size;

#ifdef ITL_NEEDS_GREGORIAN_DATE_SIZE_TYPE
#define ITL_HAS_GREGORIAN_DATE_SIZE_TYPE
    template<> 
    struct size<boost::gregorian::date> 
    { typedef boost::gregorian::date_duration type; };  
#endif 

#ifdef ITL_NEEDS_POSIX_TIME_PTIME_SIZE_TYPE
#define ITL_HAS_POSIX_TIME_PTIME_SIZE_TYPE
    template<> 
    struct size<boost::posix_time::ptime> 
    { typedef boost::posix_time::time_duration type; };  
#endif

    template <class Type> struct size{ typedef std::size_t type; };
}} // namespace boost itl

#define ITL_SIZE_TYPES_PROVIDED

#endif


