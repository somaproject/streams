/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/** Example large_bitset.cpp \file large_bitset.cpp

    \include large_bitset_/large_bitset.cpp
*/

//[large_bitset_cpp_includes
#include <limits>
#include "large_bitset.hpp"

using namespace std;
using namespace boost;
using namespace boost::itl;
using namespace mini;
//]


//[large_bitset_test_large_set_all
void test_large()
{ 
    const nat3 much = 0xffffffffffffffffull; // BTW nat3 is defined as boost::uint64_t
                                             // in large_bitset.hpp
    large_bitset<> venti; // ... the largest, I can think of ;)
    venti += interval<nat3>(0, much);

    cout << "----- Test function test_large() -----------------------------------------------\n";
    cout << "We have just turned on the awesome amount of 18,446,744,073,709,551,615 bits ;-)\n";
    venti.show_segments();
    //]

    //[large_bitset_test_large_erase_last
    cout << "---- Let's swich off the very last bit -----------------------------------------\n";
    venti -= much;
    venti.show_segments();

    cout << "---- Venti is plenty ... let's do something small: A tall ----------------------\n\n";
}
//]

//[large_bitset_test_small
void test_small()
{
    large_bitset<nat, bits8> tall; // small is tall ...
        // ... it really is, because even this 'small' large_bitset 
        // can represent up to 2^32-1 == 4,294,967,295 bits.
        // BTW nat is defined as unsigned int.

    cout << "----- Test function test_small() -----------\n";
    cout << "-- Switch on all bits in range [0,64] ------\n";
    tall += interval<nat>(0, 64);
    tall.show_segments();
    cout << "--------------------------------------------\n";

    cout << "-- Turn off bits: 25,27,28 -----------------\n";
    (((tall -= 25) -= 27) -= 28) ;
    tall.show_segments();
    cout << "--------------------------------------------\n";

    cout << "-- Flip bits in range [24,30) --------------\n";
    tall ^= interval<nat>::rightopen(24,30);
    tall.show_segments();
    cout << "--------------------------------------------\n";

    cout << "-- Remove the first 10 bits ----------------\n";
    tall -= interval<nat>::rightopen(0,10);
    tall.show_segments();

    cout << "-- remove even bits in range [0,72) --------\n";
    int bit;
    for(bit=0; bit<72; bit++) if(!(bit%2)) tall -= bit;
    tall.show_segments();

    cout << "-- set    odd  bits in range [0,72) --------\n";
    for(bit=0; bit<72; bit++) if(bit%2) tall += bit;
    tall.show_segments();

    cout << "--------------------------------------------\n\n";

}
//]

//[large_bitset_test_picturesque
void test_picturesque()
{
    typedef large_bitset<nat, bits8> Bit8Set;

    Bit8Set square, stare;
    square += interval<nat>(0,7);
    for(int i=1; i<5; i++)
    { 
        square += 8*i; 
        square += 8*i+7; 
    }

    square += interval<nat>(41,46);

    cout << "----- Test function test_picturesque() -----\n";
    cout << "-------- empty face:       " 
         << square.interval_count()           << " intervals -----\n";
    square.show_matrix(" *");

    stare += 18; stare += 21;
    stare += interval<nat>(34,37);

    cout << "-------- compressed smile: " 
         << stare.interval_count()            << " intervals -----\n";
    stare.show_matrix(" *");

    cout << "-------- staring bitset:   " 
         << (square + stare).interval_count() << " intervals -----\n";
    (square + stare).show_matrix(" *");

    cout << "--------------------------------------------\n";
}
//]

template<class NatT, class BitsT>
void test_set()
{
    const NatT much = (numeric_limits<NatT>::max)();

    large_bitset<NatT, BitsT> venti; //the largest, I can think of
    venti += interval<NatT>(0, much);

    cout << "--------------------------------------------------------------------------------\n";
    venti.show_segments();

    venti -= much;
    cout << "--------------------------------------------------------------------------------\n";
    venti.show_segments();
}



int main()
{
    cout << ">> Interval Template Library: Sample large_bitset.cpp <<\n";
    cout << "--------------------------------------------------------\n";
    test_large();
    test_small();
    test_picturesque();
    //test_set<nat3,bits64>();
    return 0;
}

