/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_TYPE_LISTS_HPP_JOFA_080916
#define BOOST_ITL_TEST_TYPE_LISTS_HPP_JOFA_080916

#include <boost/mpl/list.hpp>

// interval instance types
#include <boost/itl/gregorian.hpp> 
#include <boost/itl/ptime.hpp> 
#include <boost/itl/rational.hpp> 

#include <boost/itl/interval.hpp>

#if(_MSC_VER < 1500 && defined(_DEBUG) ) // 1500 = MSVC-9.0
typedef int boost_posix_time_ptime;
#else
typedef boost::posix_time::ptime boost_posix_time_ptime;
#endif

typedef ::boost::mpl::list<
    unsigned short, unsigned int, unsigned long  
    ,short, int, long, long long
    ,float, double, long double
    ,boost::rational<int>
    ,boost_posix_time_ptime
//    ,boost::gregorian::date
> bicremental_types;

typedef unsigned int             bicremental_type_1;
typedef          int             bicremental_type_2;
typedef          double          bicremental_type_3;
typedef boost::rational<int>     bicremental_type_4;
typedef boost_posix_time_ptime   bicremental_type_5;
typedef unsigned short           bicremental_type_6;
typedef          short           bicremental_type_7;
typedef          float           bicremental_type_8;

//DBG short list for debugging
typedef ::boost::mpl::list<
    int
> debug_types;

typedef ::boost::mpl::list<
    float, double, long double
    ,boost::rational<int>
> bicremental_continuous_types;

typedef float                bicremental_continuous_type_1;
typedef double               bicremental_continuous_type_2;
typedef boost::rational<int> bicremental_continuous_type_3;


typedef ::boost::mpl::list<
    unsigned short, unsigned int
    ,unsigned long, unsigned long long  
    ,short, int, long, long long
> integral_types;

typedef int           integral_type_1;
typedef unsigned int  integral_type_2;
typedef short         integral_type_3;
typedef unsigned int  integral_type_4;

typedef ::boost::mpl::list<
    unsigned short, unsigned int
    ,unsigned long, unsigned long long  
    ,short, int, long
    ,boost_posix_time_ptime
//    ,boost::gregorian::date
> discrete_types;

typedef int                      discrete_type_1;
typedef boost_posix_time_ptime   discrete_type_2;
typedef unsigned int             discrete_type_3;
typedef short                    discrete_type_4;
typedef unsigned int             discrete_type_5;

typedef ::boost::mpl::list<
    float, double, long double
    ,boost::rational<int>
    ,std::string
> continuous_types;

typedef double               continuous_type_1;
typedef float                continuous_type_2;
typedef boost::rational<int> continuous_type_3;
typedef std::string          continuous_type_4;

typedef ::boost::mpl::list<
    unsigned short, unsigned int
    ,unsigned long, unsigned long long  
    ,short, int, long, long long
    ,float, double, long double
    ,boost::rational<int>
    ,std::string
    ,boost_posix_time_ptime
//    ,boost::gregorian::date
> ordered_types;

typedef int                      ordered_type_1;
typedef std::string              ordered_type_2;
typedef boost_posix_time_ptime   ordered_type_3;
typedef boost::rational<int>     ordered_type_4;
typedef double                   ordered_type_5;

#endif 

