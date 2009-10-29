/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_BITS_GENTOR_HPP_JOFA_091009
#define BOOST_ITL_XT_BITS_GENTOR_HPP_JOFA_091009

#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/validate/type/bits.hpp>

namespace boost{namespace itl
{


template <class NaturalT> 
class bits_gentor: public RandomGentorAT<itl::bits<NaturalT> >
{
public:

    typedef itl::bits<NaturalT> bits_type;

    void some(bits_type& value)
    {
        value = bits_type(_natural_gentor(_value_range));
    };

    void set_range(const itl::interval<NaturalT>& range)
    { _value_range = range; }

private:
    NumberGentorT<NaturalT> _natural_gentor;
    itl::interval<NaturalT> _value_range;
};


}} // namespace itl boost

#endif


