/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __polygon_gentor_H_JOFA_000724__
#define __polygon_gentor_H_JOFA_000724__

#include <boost/itl/interval.hpp>
#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/list.hpp>

namespace boost{namespace itl
{

template <class PolygonT> 
class polygon_gentor: public RandomGentorAT<PolygonT>
{
public:
    typedef typename PolygonT::value_type ValueTypeTD;
    typedef typename PolygonT::value_type DomainTD;
    typedef list<ValueTypeTD>             SampleTypeTD;

    polygon_gentor(): p_domainGentor(NULL) {}
    ~polygon_gentor() { delete p_domainGentor; }

    virtual void some(PolygonT& x);
    void last(PolygonT& x)const;
    void last_permuted(PolygonT& x)const;

    void setDomainGentor(RandomGentorAT<DomainTD>* gentor)
    {
        delete p_domainGentor;
        p_domainGentor = gentor; 
    }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_sampleSizeRange = interval<int>::rightopen(lwb,upb); }
    void setRangeOfSampleSize(const interval<int>& szRange)
    { BOOST_ASSERT(szRange.is(right_open)); m_sampleSizeRange = szRange; }

    void setUnique(bool truth) { m_unique = truth; }

private:
    RandomGentorAT<DomainTD>*   p_domainGentor;
    interval<int>               m_sampleSizeRange;
    SampleTypeTD                m_sample;
    int                         m_sampleSize;
    bool                        m_unique;
};


template <class PolygonT> 
void polygon_gentor<PolygonT>::some(PolygonT& x)
{
    NumberGentorT<int> intGentor;
    x.clear();
    m_sample.clear();
    m_sampleSize = intGentor(m_sampleSizeRange);

    DomainTD key, first; 

    if(m_sampleSize == 0)
        return;

    p_domainGentor->some(first);
    x.push_back(first);

    for(int i=1; i<m_sampleSize; i++)
    {
        p_domainGentor->some(key);

        if(m_unique)
        {
            typename PolygonT::iterator pos_ = std::find(x.begin(), x.end(), key);
            if(pos_ == x.end())
                x.push_back(key);
        }
        else x.push_back(key);

        m_sample.push_back(key);
    }

    if(m_sampleSize > 1 && !(*x.rbegin() == first))
        x.push_back(first);
}


template <class PolygonT> 
void polygon_gentor<PolygonT>::last(PolygonT& x)const
{
    x.clear();
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample) x.insert(*it);
}

template <class PolygonT>
void polygon_gentor<PolygonT>::last_permuted(PolygonT& x)const
{
    x.clear();

    SampleTypeTD perm;

    NumberGentorT<int> intGentor;
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor(2) ) perm.push_back(*it);
        else perm.push_front(*it);
    }

    ITL_const_FORALL(typename SampleTypeTD, pit, perm) x.insert(*pit);
}


//------------------------------------------------------------------------------

template <class PolygonSetT> 
class polygon_set_gentor: public RandomGentorAT<PolygonSetT>
{
public:
    typedef typename PolygonSetT::value_type ValueTypeTD;
    typedef typename PolygonSetT::value_type DomainTD;
    typedef list<ValueTypeTD>                SampleTypeTD;

    polygon_set_gentor(): p_domainGentor(NULL) {}
    ~polygon_set_gentor() { delete p_domainGentor; }

    virtual void some(PolygonSetT& x);
    void last(PolygonSetT& x)const;
    void last_permuted(PolygonSetT& x)const;

    void setDomainGentor(RandomGentorAT<DomainTD>* gentor)
    {
        delete p_domainGentor;
        p_domainGentor = gentor; 
    }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_sampleSizeRange = interval<int>::rightopen(lwb,upb); }
    void setRangeOfSampleSize(const interval<int>& szRange)
    { BOOST_ASSERT(szRange.is(right_open)); m_sampleSizeRange = szRange; }

    void setUnique(bool truth) { m_unique = truth; }

private:
    RandomGentorAT<DomainTD>*   p_domainGentor;
    interval<int>               m_sampleSizeRange;
    SampleTypeTD                m_sample;
    int                         m_sampleSize;
    bool                        m_unique;
};


template <class PolygonSetT> 
void polygon_set_gentor<PolygonSetT>::some(PolygonSetT& x)
{
    NumberGentorT<int> intGentor;
    x.clear();
    m_sample.clear();
    m_sampleSize = intGentor(m_sampleSizeRange);

    DomainTD key, first; 

    if(m_sampleSize == 0)
        return;

    p_domainGentor->some(first);
    x.push_back(first);

    for(int i=1; i<m_sampleSize; i++)
    {
        DomainTD key;
        p_domainGentor->some(key);

        if(m_unique)
        {
            typename PolygonSetT::iterator pos_ = std::find(x.begin(), x.end(), key);
            if(pos_ == x.end())
                x.push_back(key);
        }
        else x.push_back(key);

        m_sample.push_back(key);
    }

    if(m_sampleSize > 1)
        x.push_back(first);
}


template <class PolygonSetT> 
void polygon_set_gentor<PolygonSetT>::last(PolygonSetT& x)const
{
    x.clear();
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample) x.insert(*it);
}

template <class PolygonSetT>
void polygon_set_gentor<PolygonSetT>::last_permuted(PolygonSetT& x)const
{
    x.clear();

    SampleTypeTD perm;

    NumberGentorT<int> intGentor;
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor(2) ) perm.push_back(*it);
        else perm.push_front(*it);
    }

    ITL_const_FORALL(typename SampleTypeTD, pit, perm) x.insert(*pit);
}



}} // namespace boost itl

#endif


