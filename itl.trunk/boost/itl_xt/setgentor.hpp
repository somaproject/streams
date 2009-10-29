/*-----------------------------------------------------------------------------+    
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+-----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------
class SetGentorT
    A random generator for Sets.
--------------------------------------------------------------------*/
#ifndef __SETGENTOR_H_JOFA_000724__
#define __SETGENTOR_H_JOFA_000724__

#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/list.hpp>

namespace boost{namespace itl
{

template <class SetTV> 
class SetGentorT: public RandomGentorAT<SetTV>
{
public:
    typedef typename SetTV::value_type    ValueTypeTD;
    typedef typename SetTV::key_type    DomainTD;
    typedef list<ValueTypeTD>    SampleTypeTD;
    typedef RandomGentorAT<DomainTD>  DomainGentorT;
    typedef DomainGentorT*        DomainGentorPT;

    SetGentorT(): p_domainGentor(NULL) {}
    ~SetGentorT() { delete p_domainGentor; }

    virtual void some(SetTV& x);
    void last(SetTV& x)const;
    void last_permuted(SetTV& x)const;

    void setDomainGentor(RandomGentorAT<DomainTD>* gentor)
    { 
        delete p_domainGentor;
        p_domainGentor = gentor; 
    }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_sampleSizeRange = interval<int>::rightopen(lwb,upb); }
    void setRangeOfSampleSize(const interval<int>& szRange)
    { BOOST_ASSERT(szRange.is(right_open)); m_sampleSizeRange = szRange; }

    DomainGentorPT domainGentor()const { return p_domainGentor; } 

private:
    RandomGentorAT<DomainTD>*  p_domainGentor;
    interval<int>              m_sampleSizeRange;
    SampleTypeTD               m_sample;
    int                        m_sampleSize;
};


template <class SetTV> 
void SetGentorT<SetTV>::some(SetTV& x)
{
    NumberGentorT<int> intGentor;
    x.clear();
    m_sample.clear();
    m_sampleSize = intGentor(m_sampleSizeRange);

    for(int i=0; i<m_sampleSize; i++)
    {
        DomainTD key;
        domainGentor()->some(key);
        x += key;
//JODO URG REV        m_sample.push_back(key);
    }
}


template <class SetTV> 
void SetGentorT<SetTV>::last(SetTV& x)const
{
    x.clear();
    ITL_const_FORALL(typename SampleTypeTD, it, m_sample) 
        x += *it;
}

template <class SetTV>
void SetGentorT<SetTV>::last_permuted(SetTV& x)const
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
template <class SetTV> 
void SetGentorT<SetTV>::lastSample(SampleTypeTD& sam)const
{ sam = m_sample; }


template <class SetTV> 
void SetGentorT<SetTV>::lastSample_permuted(SampleTypeTD& sam)
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

#endif


