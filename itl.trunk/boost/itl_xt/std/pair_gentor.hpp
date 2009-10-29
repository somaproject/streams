/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_STD_PAIR_GENTOR_HPP_JOFA_091009
#define BOOST_ITL_XT_STD_PAIR_GENTOR_HPP_JOFA_091009

#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl_xt/std/pair.hpp>
#include <boost/itl_xt/gentorit.hpp>

namespace boost{namespace itl
{


template <class FirstT, class SecondT> 
class std_pair_gentor: public RandomGentorAT<std::pair<FirstT,SecondT> >
{
public:

    typedef std::pair<FirstT,SecondT> pair_type;

    std_pair_gentor(): _first_gentor(NULL), _second_gentor(NULL) {}
    ~std_pair_gentor() { delete _first_gentor; delete _second_gentor; }

    void set_first_gentor(RandomGentorAT<FirstT>* gentor)
    { 
        delete _first_gentor;
        _first_gentor = gentor; 
    }

    void set_second_gentor(RandomGentorAT<SecondT>* gentor)
    { 
        delete _second_gentor;
        _second_gentor = gentor; 
    }

    void some(pair_type& value)
    {
        _first_gentor->some(value.first);
        _second_gentor->some(value.second);
    };

private:
    RandomGentorAT<FirstT>*  _first_gentor;
    RandomGentorAT<SecondT>* _second_gentor;
};


}} // namespace itl boost

#endif


