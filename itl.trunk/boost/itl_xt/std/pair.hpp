/*-----------------------------------------------------------------------------+
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
Additions to std::pair
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_STD_PAIR_HPP_JOFA_091006
#define BOOST_ITL_XT_STD_PAIR_HPP_JOFA_091006

#include <boost/itl/type_traits/type_to_string.hpp>

namespace std {

template<class CharType, class CharTraits, class FirstT, class SecondT>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, const std::pair<FirstT,SecondT>& object)
{
    return stream << "(" << object.first << "," << object.second << ")";
}

} //namespace std

namespace boost{namespace itl
{

template<> 
inline std::string binary_template_to_string<std::pair>::apply() { return "p"; }

}} // namespace itl boost


#endif // BOOST_ITL_XT_STD_PAIR_HPP_JOFA_091006

