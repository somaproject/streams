/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004
#define BOOST_ITL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004


#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/itvgentor.hpp>
#include <boost/itl_xt/seqgentor.hpp>

namespace boost{namespace itl
{

template <class DomainT, class CodomainT> 
class map_segment_gentor: public RandomGentorAT<std::pair<itl::interval<DomainT>, CodomainT> >
{
public:
    typedef DomainT   domain_type;
    typedef CodomainT codomain_type;
    typedef itl::interval<DomainT> interval_type;
    typedef std::pair<itl::interval<DomainT>, CodomainT> segment_type;

public:
    map_segment_gentor(): _codomain_gentor(NULL){}
    ~map_segment_gentor(){ delete _codomain_gentor; }

    virtual void some(segment_type& x);

    void set_range(const itl::interval<domain_type>& range)
    { _interval_gentor.setRange(range); }

    void set_corange(const itl::interval<codomain_type>& range)
    { _covalue_range = range; }
    
    void setMaxIntervalLength(domain_type max_itv_length)
    { _interval_gentor.setMaxIntervalLength(max_itv_length); }

    void setCodomainGentor(RandomGentorAT<codomain_type>* gentor)
    { 
        delete _codomain_gentor;
        _codomain_gentor = gentor; 
    }

private:
    ItvGentorT<domain_type>        _interval_gentor;
    RandomGentorAT<codomain_type>* _codomain_gentor;
    itl::interval<codomain_type>   _covalue_range;
};


template <class DomainT, class CodomainT>
void map_segment_gentor<DomainT,CodomainT>::some(segment_type& value)
{
    interval_type inter_val;
    codomain_type co_val;
    _interval_gentor.some(inter_val);
    _codomain_gentor->some(co_val);
    value = segment_type(inter_val, co_val);
};

}} // BOOST_ITL_XT_MAP_SEGMENT_GENTOR_HPP_JOFA_091004

#endif


