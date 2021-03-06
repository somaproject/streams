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
/** Example std_transform.cpp \file std_transform.cpp

    Example std_transform shows how algorithm std::transform can be used to
    fill interval containers from std::containers of objects of a user
    defined class.

    \include std_transform_/std_transform.cpp
*/
//[example_std_transform
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/itl/split_interval_map.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/iterator.hpp> // needed for itl::inserter and
                                  // itl::adder.

using namespace std;
using namespace boost;
using namespace boost::itl;

// Suppose we are working with a class called MyObject, containing some
// information about interval bounds e.g. _from, _to and some data members
// that carry associated information like e.g. _value.
class MyObject
{
public:
    MyObject(){}
    MyObject(int from, int to, int value): _from(from), _to(to), _value(value){}
    int from()const {return _from;}
    int to()const   {return _to;}
    int value()const{return _value;}
private:
    int _from;
    int _to;
    int _value;
};

// ... in order to use the std::transform algorithm to fill
// interval maps with MyObject data we need a function
// 'to_segment' that maps an object of type MyObject into
// the value type to the interval map we want to tranform to ...
pair<interval<int>, int> to_segment(const MyObject& myObj)
{
    return std::pair< interval<int>, int >
        (interval<int>::closed(myObj.from(), myObj.to()), myObj.value());
}

// ... there may be another function that returns the interval
// of an object only
interval<int> to_interval(const MyObject& myObj)
{
    return interval<int>::closed(myObj.from(), myObj.to());
}


// ... make_object computes a sequence of objects to test.
vector<MyObject> make_objects()
{
    vector<MyObject> object_vec;
    object_vec.push_back(MyObject(2,3,1));
    object_vec.push_back(MyObject(4,4,1));
    object_vec.push_back(MyObject(1,2,1));
    return object_vec;
}

// ... show_objects displays the sequence of input objects.
void show_objects(const vector<MyObject>& objects)
{
    vector<MyObject>::const_iterator iter = objects.begin();
    while(iter != objects.end())
    {
        cout << "([" << iter->from() << "," << iter->to() << "]," 
             << iter->value() << ")";
        ++iter;
    }
}


void std_transform()
{
    // This time we want to transform objects into a splitting interval map:
    split_interval_map<int,int> segmap;
    vector<MyObject> myObjects = make_objects();

    // Display the input
    cout << "input sequence: "; show_objects(myObjects); cout << "\n\n";

    // Use an itl::inserter to fill the interval map via inserts
    std::transform(myObjects.begin(), myObjects.end(),
                   itl::inserter(segmap, segmap.end()),
                   to_segment);
    cout << "itl::inserting: " << segmap << endl;
    segmap.clear();

    // In order to compute aggregation results on associated values, we
    // usually want to use an itl::adder instead of an std or itl::inserter
    std::transform(myObjects.begin(), myObjects.end(),
                   itl::adder(segmap, segmap.end()),
                   to_segment);
    cout << "itl::adding   : " << segmap << "\n\n";

    separate_interval_set<int> segset;
    std::transform(myObjects.begin(), myObjects.end(),
                   itl::adder   (segset, segset.end()),
    // could be a  itl::inserter(segset, segset.end()), here: same effect 
                   to_interval);

    cout << "Using std::transform to fill a separate_interval_set:\n\n";
    cout << "itl::adding   : " << segset << "\n\n";
}


int main()
{
    cout << ">>  Interval Template Library: Example std_transform.cpp  <<\n";
    cout << "------------------------------------------------------------\n";
    cout << "Using std::transform to fill a split_interval_map:\n\n";

    std_transform();
    return 0;
}

// Program output:
/*----------------------------------------------------------
>>  Interval Template Library: Example std_transform.cpp  <<
------------------------------------------------------------
Using std::transform to fill a split_interval_map:

input sequence: ([2,3],1)([4,4],1)([1,2],1)

itl::inserting: {([1,2)->1)([2,3]->1)([4,4]->1)}
itl::adding   : {([1,2)->1)([2,2]->2)((2,3]->1)([4,4]->1)}

Using std::transform to fill a separate_interval_set:

itl::adding   : {[1,3][4,4]}
----------------------------------------------------------*/
//]

