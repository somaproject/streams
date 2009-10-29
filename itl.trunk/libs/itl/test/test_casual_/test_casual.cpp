/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::casual unit test
#include <string>
#include <vector>
#include <boost/mpl/list.hpp>
#include <boost/itl/iterator.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/type_traits/is_same.hpp>

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/validate/type/nat.hpp>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


BOOST_AUTO_TEST_CASE(casual_test)
{
    typedef int T;
    typedef int U;
    typedef interval_map<int,int>       IntervalMapT;
    typedef split_interval_map<int,int> SplitIntervalMapT;
    typedef interval_map<int,int>       IntervalMapT;
    typedef vector<pair<interval<int>,int> > IntervalVecT;

    IntervalVecT ivec;
    ivec.push_back(make_pair(interval<int>::rightopen(1,3),1));
    ivec.push_back(make_pair(interval<int>::rightopen(2,4),1));

    SplitIntervalMapT sim1, sim2;
    sim1.insert(make_pair(interval<int>::rightopen(1,3),1));
    sim1.insert(make_pair(interval<int>::rightopen(2,4),1));

    IntervalMapT jim1;
    std::copy(ivec.begin(), ivec.end(), itl::adder(jim1, jim1.end()));
    //std::copy(ivec.begin(), ivec.end(), std::inserter(jim2, jim2.end()));
    cout << jim1 << endl;
}

