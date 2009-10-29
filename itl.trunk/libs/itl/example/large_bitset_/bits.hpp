/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_LIBS_ITL_EXAMPLE_LARGE_BITSET_BITS_HPP_JOFA_091019
#define BOOST_LIBS_ITL_EXAMPLE_LARGE_BITSET_BITS_HPP_JOFA_091019
//[mini_bits_includes
                                               // These includes are needed ...
#include <string>                              // for conversion to output
#include <boost/itl/type_traits/is_set.hpp>    // to define that bits is a set
//]

namespace mini
{
//[mini_bits_class_bits
template<class NaturalT> class bits
{
public:
    typedef NaturalT chunk_type;
    BOOST_STATIC_CONSTANT( int,      bit_count = CHAR_BIT*sizeof(NaturalT) );
    BOOST_STATIC_CONSTANT( NaturalT, n1 = static_cast<NaturalT>(1)         );

    bits():_bits(){}
    explicit bits(NaturalT value):_bits(value){}

    NaturalT number()const{ return _bits; }
    bits& operator |= (const bits& value){_bits |= value._bits; return *this;}
    bits& operator &= (const bits& value){_bits &= value._bits; return *this;}
    bits& operator ^= (const bits& value){_bits ^= value._bits; return *this;}
    bits  operator ~  ()const { return bits(~_bits); }
    bool operator  <  (const bits& value)const{return _bits < value._bits;}
    bool operator  == (const bits& value)const{return _bits == value._bits;}

    bool contains(NaturalT element)const{ return ((n1 << element) & _bits) != 0; } 
    std::string as_string(const char off_on[2] = " 1")const;

private:
    NaturalT _bits;
};
//]

template<class NaturalT>
std::string bits<NaturalT>::as_string(const char off_on[2])const
{
    std::string sequence;
    for(int bit=0; bit < bit_count; bit++)
        sequence += contains(bit) ? off_on[1] : off_on[0];
    return sequence;
}

} // mini

//[mini_bits_is_set
namespace boost { namespace itl 
{
    template<class NaturalT>
    struct is_set<mini::bits<NaturalT> >
    { 
        typedef is_set<mini::bits<NaturalT> > type;
        BOOST_STATIC_CONSTANT(bool, value = true); 
    };
}}
//]

#endif
