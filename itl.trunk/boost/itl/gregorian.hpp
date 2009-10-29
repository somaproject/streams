/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_GREGORIAN_DATE_HPP_JOFA_080416
#define BOOST_ITL_GREGORIAN_DATE_HPP_JOFA_080416

#include <stdio.h>
#include <string>
#include <sstream>

#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef ITL_NEUTRONS_PROVIDED
#pragma message("error: No neutral element provided for type boost::gregorian::date")
#pragma message(".. please #include <boost/itl/gregorian.hpp> PRIOR TO other itl/* classes")
#endif

#ifdef ITL_DIFFERENCE_TYPES_PROVIDED
#pragma message("error: No difference type provided for type boost::gregorian::date")
#pragma message(".. please #include <boost/itl/gregorian.hpp> PRIOR TO other itl/* classes")
#endif

#ifdef ITL_SIZE_TYPES_PROVIDED
#pragma message("error: No size type provided for type boost::gregorian::date")
#pragma message(".. please #include <boost/itl/gregorian.hpp> PRIOR TO other itl/* classes")
#endif

#define ITL_NEEDS_GREGORIAN_DATE_NEUTRON_VALUE
#define ITL_NEEDS_GREGORIAN_DATE_DIFFERENCE_TYPE
#define ITL_NEEDS_GREGORIAN_DATE_SIZE_TYPE

namespace boost{namespace itl
{

    boost::gregorian::date operator ++(boost::gregorian::date& x)
    {
        return x += boost::gregorian::date::duration_type::unit();
    }

    boost::gregorian::date operator --(boost::gregorian::date& x)
    {
        return x -= boost::gregorian::date::duration_type::unit();
    }

    // ------------------------------------------------------------------------
    boost::gregorian::date_duration operator ++(boost::gregorian::date_duration& x)
    {
        return x += boost::gregorian::date::duration_type::unit();
    }

    boost::gregorian::date_duration operator --(boost::gregorian::date_duration& x)
    {
        return x -= boost::gregorian::date::duration_type::unit();
    }

    // ------------------------------------------------------------------------


}} // namespace itl boost

#endif


