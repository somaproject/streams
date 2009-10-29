/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <stdio.h>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>

#define ITL_LOCATION(message) location(__FILE__,__LINE__,message)

namespace boost{namespace itl
{

class nat
{
public:
    nat(): _value(0) {}
    nat(int val): _value(val<0 ? 0 : val){}
    nat(const nat& val): _value(val._value){}

    int value()const { return _value; }

    nat& operator++(){ ++_value; return *this; }
    const nat operator++(int){ nat that = *this; ++_value; return that; }

    nat& operator--(){ if(_value > 0)--_value; return *this; }
    const nat operator--(int){ nat that = *this; --_value; return that; }

    nat& operator += (const nat& right){ _value += right._value; return *this; }

    nat& operator -= (const nat& right)
    { 
        _value = right._value > _value ? 0 : (_value - right._value); 
        return *this; 
    }

    nat& operator ^= (const nat& right) //JODO should not be required
    { 
        _value ^= right._value; 
        return *this; 
    }
    nat& operator &= (const nat& right) //JODO should not be required
    { 
        _value &= right._value; 
        return *this; 
    }

    operator int()const{ return (_value); }

    std::string as_string()const { return to_string<int>::apply(_value); }

private:
    int _value;
};

//inline nat operator + (const nat& left, const nat& right){return nat(left)+=right;}
//inline nat operator - (const nat& left, const nat& right){return nat(left)-=right;}

inline bool operator == (const boost::itl::nat& left, const boost::itl::nat& right){ return left.value() == right.value(); }
inline bool operator <  (const boost::itl::nat& left, const boost::itl::nat& right){ return left.value() < right.value(); }

template<>inline std::string type_to_string<nat>::apply() { return "nat"; }

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits> &operator<<
(std::basic_ostream<CharType, CharTraits> &stream, itl::nat const& right)
{
    return stream << right.value();
}

}} // namespace itl boost

