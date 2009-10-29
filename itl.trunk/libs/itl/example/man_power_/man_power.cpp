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
/** Example man_power.cpp \file man_power.cpp

    Interval sets and maps can be filled and manipulated using
    set style operation like union (+=), difference (-=) and intersection
    (&=).

    In this example 'man_power' a number of those operations are
    demonstrated in the process of calculation the available working 
    times (man-power) of a company's employees accounting for weekends,
    holidays, sickness times and vacations.

    \include man_power_/man_power.cpp
*/
//[example_man_power
#include <iostream>

// The next line includes <boost/gregorian/date.hpp>
// and a few lines of adapter code.
#include <boost/itl/gregorian.hpp> 

#include <boost/itl/interval_map.hpp>

using namespace std;
using namespace boost::gregorian;
using namespace boost::itl;


// Function weekends returns the interval_set of weekends that are contained in
// the date interval 'scope'
interval_set<date> weekends(const interval<date>& scope)
{
    interval_set<date> weekends;

    date cur_weekend_sat 
        = scope.first() 
          + days(days_until_weekday(scope.first(), greg_weekday(Saturday))) 
          - weeks(1);
    week_iterator week_iter(cur_weekend_sat);

    for(; week_iter <= scope.last(); ++week_iter)
        weekends += interval<date>::rightopen(*week_iter, *week_iter + days(2));

    weekends &= scope; // cut off the surplus

    return weekends;
}

// The available working time for the employees of a company is calculated
// for a period of 3 months accounting for weekends and holidays.
//    The available daily working time for the employees is calculated
// using interval_sets and interval_maps demonstrating a number of
// addition, subtraction and intersection operations.
void man_power()
{
    date someday = from_string("2008-08-01");
    date thenday = someday + months(3);

    interval<date> scope = interval<date>::rightopen(someday, thenday);

    // ------------------------------------------------------------------------
    // (1) In a first step, the regular working times are computed for the
    // company within the given scope. From all available days, the weekends
    // and holidays have to be subtracted: 
    interval_set<date> worktime(scope);
    // Subtract the weekends
    worktime -= weekends(scope);
    // Subtract holidays
    worktime -= from_string("2008-10-03"); //German reunification ;)

    // company holidays (fictitious ;)
    worktime -= interval<date>::closed(from_string("2008-08-18"), 
                                       from_string("2008-08-22"));

    //-------------------------------------------------------------------------
    // (2) Now we calculate the individual work times for some employees
    //-------------------------------------------------------------------------
    // In the company works Claudia. 
    // This is the map of her regular working times:
    interval_map<date,int> claudias_working_hours;

    // Claudia is working 8 hours a day. So the next statement says
    // that every day in the whole scope is mapped to 8 hours worktime.
    claudias_working_hours += make_pair(scope, 8);

    // But Claudia only works 8 hours on regular working days so we do
    // an intersection of the interval_map with the interval_set worktime:
    claudias_working_hours &= worktime;

    // Yet, in addition Claudia has her own absence times like
    interval<date> claudias_seminar (from_string("2008-09-16"), 
                                     from_string("2008-09-24"));
    interval<date> claudias_vacation(from_string("2008-08-01"), 
                                     from_string("2008-08-14"));

    interval_set<date> claudias_absence_times(claudias_seminar);
    claudias_absence_times += claudias_vacation;

    // All the absence times have to subtracted from the map of her working times
    claudias_working_hours -= claudias_absence_times;

    //-------------------------------------------------------------------------
    // Claudia's boss is Bodo. He only works part time. 
    // This is the map of his regular working times:
    interval_map<date,int> bodos_working_hours;

    // Bodo is working 4 hours a day.
    bodos_working_hours += make_pair(scope, 4);

    // Bodo works only on regular working days
    bodos_working_hours &= worktime;

    // Bodos additional absence times
    interval<date> bodos_flu(from_string("2008-09-19"), from_string("2008-09-29"));
    interval<date> bodos_vacation(from_string("2008-08-15"), from_string("2008-09-03"));

    interval_set<date> bodos_absence_times(bodos_flu);
    bodos_absence_times += bodos_vacation;

    // All the absence times have to be subtracted from the map of his working times
    bodos_working_hours -= bodos_absence_times;

    //-------------------------------------------------------------------------
    // (3) Finally we want to calculate the available manpower of the company
    // for the selected time scope: This is done by adding up the employees
    // working time maps:
    interval_map<date,int> manpower;
    manpower += claudias_working_hours;
    manpower += bodos_working_hours;


    cout << scope.first() << " - " << scope.last() 
         << "    available man-power:" << endl;
    cout << "---------------------------------------------------------------\n";

    for(interval_map<date,int>::iterator it = manpower.begin(); 
        it != manpower.end(); it++)
    {
        cout << it->first.first() << " - " << it->first.last() 
             << " -> " << it->second << endl;
    }
}

int main()
{
    cout << ">> Interval Template Library: Sample man_power.cpp <<\n";
    cout << "---------------------------------------------------------------\n";
    man_power();
    return 0;
}

// Program output:
/*
>> Interval Template Library: Sample man_power.cpp <<
---------------------------------------------------------------
2008-Aug-01 - 2008-Oct-31    available man-power:
---------------------------------------------------------------
2008-Aug-01 - 2008-Aug-01 -> 4
2008-Aug-04 - 2008-Aug-08 -> 4
2008-Aug-11 - 2008-Aug-14 -> 4
2008-Aug-15 - 2008-Aug-15 -> 8
2008-Aug-25 - 2008-Aug-29 -> 8
2008-Sep-01 - 2008-Sep-03 -> 8
2008-Sep-04 - 2008-Sep-05 -> 12
2008-Sep-08 - 2008-Sep-12 -> 12
2008-Sep-15 - 2008-Sep-15 -> 12
2008-Sep-16 - 2008-Sep-18 -> 4
2008-Sep-25 - 2008-Sep-26 -> 8
2008-Sep-29 - 2008-Sep-29 -> 8
2008-Sep-30 - 2008-Oct-02 -> 12
2008-Oct-06 - 2008-Oct-10 -> 12
2008-Oct-13 - 2008-Oct-17 -> 12
2008-Oct-20 - 2008-Oct-24 -> 12
2008-Oct-27 - 2008-Oct-31 -> 12
*/
//]

