/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
itl_ptime provides adapter code for boost::posix_time::ptime.
It implements incrementation (++) decrementation (--) and a neutral element
w.r.t. addition (neutron()).
------------------------------------------------------------------------------*/

#ifndef BOOST_ITL_PTIME_HPP_JOFA_080416
#define BOOST_ITL_PTIME_HPP_JOFA_080416

#include <stdio.h>
#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef ITL_NEUTRONS_PROVIDED
#pragma message("error: No neutral element provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
#endif

#ifdef ITL_DIFFERENCE_TYPES_PROVIDED
#pragma message("error: No difference type provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
#endif

#ifdef ITL_SIZE_TYPES_PROVIDED
#pragma message("error: No size type provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
#endif

#define ITL_NEEDS_POSIX_TIME_PTIME_NEUTRON_VALUE
#define ITL_NEEDS_POSIX_TIME_PTIME_DIFFERENCE_TYPE
#define ITL_NEEDS_POSIX_TIME_PTIME_SIZE_TYPE

namespace boost{namespace itl
{

    // ------------------------------------------------------------------------
    boost::posix_time::ptime operator ++(boost::posix_time::ptime& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::ptime operator --(boost::posix_time::ptime& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }

    // ------------------------------------------------------------------------
    boost::posix_time::time_duration operator ++(boost::posix_time::time_duration& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::time_duration operator --(boost::posix_time::time_duration& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }
}} // namespace itl boost

#endif


