/*-----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/** Example interval.cpp \file interval.cpp

    Much of the library code deals with intervals which are implemented
    as class interval.

    This program gives a very short samlpe of different interval instances.

    \include example/interval_/interval.cpp
*/
//[example_interval
#include <iostream>
#include <string>
#include <math.h>
#include <boost/itl/interval.hpp>
#include "../toytime.hpp"

using namespace std;
using namespace boost::itl;

int main()
{
    cout << ">> Interval Template Library: Sample interval.cpp <<\n";
    cout << "----------------------------------------------------\n";

    interval<int>    int_interval  = interval<int>::closed(3,7);
    interval<double> sqrt_interval = interval<double>::rightopen(1/sqrt(2.0), sqrt(2.0));
    interval<string> city_interval = interval<string>::leftopen("Barcelona", "Boston");
    interval<Time>   time_interval = interval<Time>::open(Time(monday,8,30), Time(monday,17,20));

    cout << "Interval<int>: "    << int_interval  << endl;
    cout << "Interval<double>: " << sqrt_interval << " does " 
                                 << string(sqrt_interval.contains(sqrt(2.0))?"":"NOT") 
                                 << " contain sqrt(2)" << endl;
    cout << "Interval<string>: " << city_interval << " does "  
                                 << string(city_interval.contains("Barcelona")?"":"NOT") 
                                 << " contain 'Barcelona'" << endl;
    cout << "Interval<string>: " << city_interval << " does "  
                                 << string(city_interval.contains("Berlin")?"":"NOT") 
                                 << " contain 'Berlin'" << endl;
    cout << "Interval<Time>: "   << time_interval << endl;

    return 0;
}

// Program output:

// >> Interval Template Library: Sample interval.cpp <<
// ---------------------------------------------------
// Interval<int>: [3,7]
// Interval<double>: [0.707107,1.41421) does NOT contain sqrt(2)
// Interval<string>: (Barcelona,Boston] does NOT contain 'Barcelona'
// Interval<string>: (Barcelona,Boston] does  contain 'Berlin'
// Interval<Time>: (mon:08:30,mon:17:20)
//]

