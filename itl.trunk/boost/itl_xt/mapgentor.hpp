/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_MAPGENTOR_HPP_JOFA_000724
#define BOOST_ITL_XT_MAPGENTOR_HPP_JOFA_000724

#include <boost/itl_xt/list.hpp>
#include <boost/itl_xt/gentorit.hpp>

namespace boost{namespace itl
{

template <class MapTV> 
class MapGentorT: public RandomGentorAT<MapTV>
{
public:
    typedef typename MapTV::value_type ValueTypeTD;
    typedef typename MapTV::key_type   DomainTD;
    typedef typename MapTV::data_type  CodomainTD;
    typedef list<ValueTypeTD>          SampleTypeTD;

    MapGentorT(): p_domainGentor(NULL), p_codomainGentor(NULL) {}
    ~MapGentorT() { delete p_domainGentor; delete p_codomainGentor; }

    virtual void some(MapTV& x);
    void last(MapTV& x)const;
    void last_permuted(MapTV& x)const;

    void setDomainGentor(RandomGentorAT<DomainTD>* gentor)
    { 
        delete p_domainGentor;
        p_domainGentor = gentor; 
    }
    void setCodomainGentor(RandomGentorAT<CodomainTD>* gentor)
    { 
        delete p_codomainGentor;
        p_codomainGentor = gentor; 
    }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_sampleSizeRange = interval<int>::rightopen(lwb,upb); }
    void setRangeOfSampleSize(const interval<int>& szRange)
    { BOOST_ASSERT(szRange.is(right_open)); m_sampleSizeRange = szRange; }

private:
    RandomGentorAT<DomainTD>*    p_domainGentor;
    RandomGentorAT<CodomainTD>*  p_codomainGentor;
    interval<int>                m_sampleSizeRange;
    SampleTypeTD                 m_sample;
    int                          m_sampleSize;
};


template <class MapTV> 
void MapGentorT<MapTV>::some(MapTV& x)
{
    NumberGentorT<int> intGentor;
    x.clear();
    m_sample.clear();
    m_sampleSize = intGentor(m_sampleSizeRange);

    for(int i=0; i<m_sampleSize; i++)
    {
        DomainTD key;
        p_domainGentor->some(key);
        CodomainTD val;
        p_codomainGentor->some(val);
        x += ValueTypeTD(key,val);
        m_sample.push_back(ValueTypeTD(key,val));
    }
}


template <class MapTV> 
void MapGentorT<MapTV>::last(MapTV& x)const
{
    x.clear();
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample) 
        x += *it;
}

template <class MapTV>
void MapGentorT<MapTV>::last_permuted(MapTV& x)const
{
    x.clear();

    SampleTypeTD perm;

    NumberGentorT<int> intGentor;
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor(2) ) perm.push_back(*it);
        else perm.push_front(*it);
    }

    ITL_const_FORALL(typename SampleTypeTD, pit, perm) 
        x += *pit;
}


/*
template <class MapTV> 
void MapGentorT<MapTV>::lastSample(SampleTypeTD& sam)const
{ sam = m_sample; }


template <class MapTV> 
void MapGentorT<MapTV>::lastSample_permuted(SampleTypeTD& sam)
{
    NumberGentorT<unsigned> intGentor;
    x.clear();
    int coin = intGentor.some(2); // gives 0 or 1
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor.some(2) ) sam.push_back(*it);
        else sam.push_front(*it);
    }
}
*/

}} // namespace itl boost

#endif // BOOST_ITL_XT_MAPGENTOR_HPP_JOFA_000724


