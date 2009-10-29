/*-----------------------------------------------------------------------------+    
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <boost/validate/gentor/gentorprofile.hpp>

using namespace boost::itl;

GentorProfileSgl* GentorProfileSgl::s_instance = NULL;

GentorProfileSgl* GentorProfileSgl::it()
{
    if(s_instance==NULL) {
        s_instance = new GentorProfileSgl;
    }
    return s_instance;
}

GentorProfileSgl::GentorProfileSgl(){}


void GentorProfile::set_defaults()
{
    set_debug_slowdown(8.45);
    set_trials_count_release(100);
#ifdef _DEBUG
    set_debug_defaults();
#else
    set_release_defaults();
#endif
}


int GentorProfile::adjusted_trials_count()const 
{ 
#ifdef _DEBUG
    return static_cast<int>(floor(0.5+(_trials_count_release / _debug_slowdown))); 
#else
    return _trials_count_release;
#endif
}


void GentorProfile::set_debug_defaults()
{
    set_range_int(-5, 5);
    set_range_nat(0, 16);
    set_range_double(0.0, 1.0);
    set_range_ContainerSize(0,4);

    set_range_interval_int(-5, 5);
    set_range_interval_double(-5.0, 5.0);
    set_maxIntervalLength(5);

    set_range_codomain_ContainerSize(0,4);

    set_repeat_count(1);
    set_trials_count(adjusted_trials_count());
    set_laws_per_cycle(100);
}

void GentorProfile::set_release_defaults()
{
    set_range_int(-5, 5);
    set_range_nat(0, 16);
    set_range_double(0.0, 1.0);
    set_range_ContainerSize(0,20);

    set_range_interval_int(-20, 20);
    set_range_interval_double(-20.0, 20.0);
    set_maxIntervalLength(10);

    set_range_codomain_ContainerSize(0,20);

    set_repeat_count(1);
    set_trials_count(trials_count_release());
    set_laws_per_cycle(100);
}


void GentorProfile::set_std_profile(int unit, int factor)
{
    int value = unit*factor;
    _unit     = unit;
    _scaling  = factor;
    set_defaults();

    // Codomain values
    set_range_int(-5, 5);
    set_range_nat(0, 16);
    set_range_double(0.0, 1.0);
    set_range_codomain_ContainerSize(0,4);

    // Parameter that influence speed
    set_range_ContainerSize(0, value);
    set_range_interval_int(-value, value);
    set_range_interval_double(-value, value);
    set_maxIntervalLength(value);

    // Parameter to influence frequencies of output update.
    set_repeat_count(1);
    set_trials_count(adjusted_trials_count());
    set_laws_per_cycle(std::max(1, 100/factor));
}

void GentorProfile::set_polygon_profile(int max_polygon_set_size, int max_polygon_size, int min_coord, int max_coord)
{
    int factor = 1;
    int unit  = max_polygon_set_size+1;
    int value = unit*factor;
    _unit     = unit;
    _scaling  = factor;
    set_defaults();

    // Codomain values
    set_range_int(min_coord, max_coord);
    set_range_nat(0, 16);
    set_range_double(0.0, 1.0);
    set_range_codomain_ContainerSize(0, max_polygon_size+1);

    // Parameter that influence speed
    set_range_ContainerSize(0, value);
    set_range_interval_int(-value, value);
    set_range_interval_double(-value, value);
    set_maxIntervalLength(value);

    // Parameter to influence frequencies of output update.
    set_repeat_count(1);
    set_trials_count(adjusted_trials_count());
    set_laws_per_cycle(std::max(1, 100/factor));
}


GentorProfile::GentorProfile()
{
    set_std_profile(4, 1);
}

void GentorProfile::report_profile()
{
    printf("(cycl=%d trls=%d rep=%d unit=%d scale=%d)\n", 
        laws_per_cycle(), trials_count(), repeat_count(),
        unit(), scaling()
        );
}

// -------------------------------------
// (Algebra, Law, Type) -> (testsCount)
// (set, ipAddCommut, itl::set) -> 150

