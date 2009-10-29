/*-----------------------------------------------------------------------------+
Copyright 2008 Intel Corporation
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma warning( disable : 4800 ) //'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( disable : 4244 ) //'argument' : conversion from 'T1' to 'T2', possible loss of data

#include <boost/polygon/polygon.hpp>
#include <list>
#include <time.h>
#include <cassert>
#include <deque>
#include <iostream>


#include <boost/itl/ptime.hpp>
#include "polygon_laws.hpp"

#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>
#include <boost/validate/gentor/rangegentor.hpp>

#include "point_gentor.hpp"
#include "polygon_gentor.hpp"
#include "custom_polygon.hpp"


namespace gtl = boost::polygon;
using namespace std;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;

using namespace boost::polygon::operators;

//------------------------------------------------------------------------------
//--- Parts from examplefiles to see if everything is basically right        ---
//------------------------------------------------------------------------------

//once again we make our usage of the library generic
//and parameterize it on the polygon set type
template <typename PolygonSet>
void test_polygon_set() {
  using namespace gtl;
  PolygonSet ps;
  ps += rectangle_data<int>(0, 0, 10, 10);
  PolygonSet ps2;
  ps2 += rectangle_data<int>(5, 5, 15, 15);
  PolygonSet ps3;
  assign(ps3, ps * ps2);
  PolygonSet ps4, pps;
  pps += ps;

  self_assignment_boolean_op<PolygonSet, PolygonSet, 0>(pps, ps);

  ps4 += ps + ps2;
  assert(area(ps4) == area(ps) + area(ps2) - area(ps3));
  assert(equivalence((ps + ps2) - (ps * ps2), ps ^ ps2));
  rectangle_data<int> rect;
  assert(extents(rect, ps ^ ps2));
  assert(area(rect) == 225);
  assert(area(rect ^ (ps ^ ps2)) == area(rect) - area(ps ^ ps2));
}



int polygon_test() {
  long long c1 = clock();
  for(int i = 0; i < 100; ++i)
  {
    if(i%10==0)
        std::cout << ".";
    test_polygon_set<CPolygonSet>();
  }
  long long c2 = clock();
  for(int i = 0; i < 100; ++i)
  {
    if(i%10==0)
        std::cout << ".";
    test_polygon_set<gtl::polygon_set_data<int> >();
  }
  long long c3 = clock();
  long long diff1 = c2 - c1;
  long long diff2 = c3 - c2;
  if(diff1 > 0 && diff2)
    std::cout << "library polygon_set_data is " << float(diff1)/float(diff2) << "X faster than custom polygon set deque of CPolygon" << std::endl;
  else
    std::cout << "operation was too fast" << std::endl;
  return 0;
}


//------------------------------------------------------------------------------
// This function tests the generator for polygon sets.
void test_polyset_gen()
{
    typedef itl::list<point<int> > PolygonT;
    typedef itl::list<PolygonT>    PolygonSetT;

    point_gentor<int> pointgen;
    pointgen.setRange(interval<int>::rightopen(-99, 100));

    PolygonT some_poly;
    polygon_gentor<PolygonT> polygen;
    polygen.setDomainGentor(&pointgen);
    polygen.setRangeOfSampleSize(interval<int>::rightopen(1, 5));

    PolygonSetT some_polyset;
    polygon_set_gentor<PolygonSetT> polysetgen;
    polysetgen.setDomainGentor(&polygen);
    polysetgen.setRangeOfSampleSize(interval<int>::rightopen(1, 3));

    for(int idx=0; idx<10; idx++)
    {
        polysetgen.some(some_polyset);
        cout << "[";
        for(PolygonSetT::iterator its_ = some_polyset.begin();
            its_ != some_polyset.end(); ++its_)
        {
            PolygonT& some_poly_ = *its_;
            cout << "{";
            for(PolygonT::iterator it_ = some_poly_.begin();
                it_ != some_poly_.end(); ++it_)
                cout << "(" << it_->x << "," << it_->y << ")";
            cout << "}\n";
        }
        cout << "]\n";
    }
}


//------------------------------------------------------------------------------
// Test single laws using this
void test_LawValidater()
{

    //typedef PolygonCommutativity
    //    < CPolygonSet >  TestLawT;
    //LawValidater<TestLawT, RandomGentor> test_law;

    typedef PolygonSymmetricDifference
        < CPolygonSet >  TestLawT;
    LawValidater<TestLawT, RandomGentor> test_law;

    //-----------------------------------------------------------------------------
    // Set the test automatons parameters:
    // Size of polygon sets is in [0 .. |set| ]
    // Size of polygon sets is in [0 .. |poly|]
    // Coordinates in [min .. max)
    //                                         |set|,|poly|, min, max 
    GentorProfileSgl::it()->set_polygon_profile(1,    3,     0,   10);
    int test_count = 10000;

    ptime start, stop;
    test_law.set_trials_count(test_count);

    std::cout << "Start\n";
    start = ptime(microsec_clock::local_time());
    test_law.run();
    stop = ptime(microsec_clock::local_time());
    std::cout << "Stop. Time elapsed: " << stop - start << endl;
}



int main()
{
    //test_polyset_gen();
    //polygon_test();
    //test_polygon_set<CPolygonSet>();
    test_LawValidater();
}


