/*-----------------------------------------------------------------------------+    
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __NUMBERGENTORT_H_JOFA_000725__
#define __NUMBERGENTORT_H_JOFA_000725__

#if defined(_MSC_VER)
#pragma warning( disable : 4244 ) //warning C4244: 'argument' : conversion from 'T1' to 'T2', possible loss of data
#endif

#include <stdlib.h>
#include <vector>
#include <boost/validate/type/nat.hpp>
#include <boost/itl/type_traits/unon.hpp>
#include <boost/itl_xt/gentorit.hpp>

using namespace boost::itl;

#define RND_1_TO(y)      (1+(int)((double)(y)*rand()/(RAND_MAX+1.0)))
#define RND_0_TO(y)      ((int)((double)((y)+1)*rand()/(RAND_MAX+1.0)))
#define RND_WITHIN(x,y) ((x)+(int)((double)((y)-(x)+1)*rand()/(RAND_MAX+1.0)))

// -----------------------------------------------------------

#define RND_O_TO_EXCL(y) (((double)(y))*rand()/(RAND_MAX+1.0))

#define RND_WITHIN_EXUPB(x,y) ((x)+((double)((y)-(x))*rand()/(RAND_MAX+1.0)))

namespace boost{namespace itl
{

template <class NumTV>
inline NumTV rnd_0_to_excl(NumTV exclusive_upb) 
{ return (NumTV)RND_O_TO_EXCL(exclusive_upb); }


template <class NumTV>
inline NumTV rnd_within_exUpb(NumTV lwb, NumTV exclusive_upb) 
{ 
    return static_cast<NumTV>(RND_WITHIN_EXUPB(lwb,exclusive_upb)); 
}

template <class NumTV>
inline NumTV rnd_within(NumTV lwb, NumTV upb) 
{ 
    NumTV some = (NumTV)RND_WITHIN(lwb,upb); 
    return some;
}

template <class NumT>
class NumberGentorProfile : public RandomGentorProfile<NumT>
{
public:
private:
    interval<NumT> _range;
};

template <class NumTV>
class NumberGentorT : public RandomGentorAT<NumTV>
{
public:
    NumberGentorT(): 
      m_valueRange( NumTV(), unon<NumTV>::value(), right_open ) {}

    NumTV operator() (NumTV upb) { return rnd_0_to_excl<NumTV>(upb); }
    NumTV operator() (NumTV lwb, NumTV upb)  { return rnd_within_exUpb<NumTV>(lwb,upb); }
    NumTV operator() (interval<NumTV> rng) 
    { 
        BOOST_ASSERT(rng.is(right_open) || rng.is(closed_bounded));
        if(rng.is(right_open))
            return rnd_within_exUpb<NumTV>(rng.lower(), rng.upper());
        else
            return rnd_within<NumTV>(rng.lower(), rng.upper());
    }

    void setRange(interval<NumTV> rng) { m_valueRange = rng; }
    void setRange(NumTV lwb, NumTV upb) { m_valueRange = interval<NumTV>::rightopen(lwb,upb); } 

    void calibrate(const RandomGentorProfile<NumTV>& profile)
    {
        m_valueRange = profile.getValueRange();
    }

    void some(NumTV& x) { x = (*this)(m_valueRange); }

    std::string as_string()const { return "NumberGentorT";}

private:
    interval<NumTV> m_valueRange;
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
template <typename WeightsT>
class WeightedNumberGentor : public RandomGentorAT<int>
{
public:
    WeightedNumberGentor(): _size(0), _maxWeights(0){}

    WeightedNumberGentor(int size, WeightsT maxWeights):
        _size(size), _maxWeights(maxWeights),
        _weights(size, WeightsT()), _kumWeights(size+1, WeightsT()),
        _typeNames(size, std::string())
        {}

    WeightedNumberGentor(int size):
        _size(size), _maxWeights(1000),
        _weights(size, WeightsT()), _kumWeights(size+1, WeightsT()),
        _typeNames(size, std::string())
        {}

    void setSize(int size);
    int size()const { return _size; }

    void setMaxWeights(int maxWeights) { _maxWeights = maxWeights; }
    WeightsT maxWeights()const { return _maxWeights; }
    WeightsT chosenValue()const { return _chosenValue; }

    void setTypeNames(const std::vector<std::string>& names)
    { _typeNames = names ; }

    void setTypeNames(const char* denotation[])
    { 
        for(int idx=0; idx < size(); idx++)
            _typeNames[idx] = std::string(denotation[idx]);
    }

    WeightsT& operator[](int idx) 
    { BOOST_ASSERT(0<=idx && idx<_size); return _weights[idx]; }

    void setWeight(int idx, WeightsT val) { BOOST_ASSERT(0<=idx && idx<_size); _weights[idx] = val; }

    WeightsT getKumWeight(int idx)const { return _kumWeights[idx]; }

    WeightsT sumOfWeights()const;

    void init();

    int some();
    void some(int& index){ index = some(); }

    int lower_bound_index(int low, int up, WeightsT val);

    bool isRangeValid()const 
    { return _kumWeights[_chosenIndex] <= _chosenValue && _chosenValue < _kumWeights[_chosenIndex+1]; }

    std::string asString()const;

    bool is_consistent()const { return sumOfWeights() == _maxWeights; }

    std::string inconsitencyMessage(const std::string& location)const;

private:
    int                      _size;
    WeightsT                 _maxWeights;
    std::vector<WeightsT>    _weights;
    std::vector<WeightsT>    _kumWeights;
    NumberGentorT<int>       _numgentor;
    WeightsT                 _chosenValue;
    int                      _chosenIndex;
    std::vector<std::string> _typeNames;
};


template <typename WeightsT>
void WeightedNumberGentor<WeightsT>::setSize(int size)
{
    _size = size;
    _weights.resize(_size, WeightsT());
    _kumWeights.resize(_size+1, WeightsT());
}


template <typename WeightsT>
void WeightedNumberGentor<WeightsT>::init()
{
    _kumWeights[0] = WeightsT();
    for(int idx=1; idx < _size; idx++)
        _kumWeights[idx] = static_cast<WeightsT>((std::min)(_kumWeights[idx-1]+_weights[idx-1], _maxWeights));

    _kumWeights[_size] = _maxWeights;
}

template <typename WeightsT>
WeightsT WeightedNumberGentor<WeightsT>::sumOfWeights()const 
{
    WeightsT weightSum = WeightsT();
    for(int idx=0; idx < _size; idx++)
        weightSum += _weights[idx];

    return weightSum; 
}

template <typename WeightsT>
int WeightedNumberGentor<WeightsT>::some()
{
    _chosenValue = _numgentor(_maxWeights);
    _chosenIndex = lower_bound_index(0, _size, _chosenValue);
    return _chosenIndex;
}

template <typename WeightsT>
int WeightedNumberGentor<WeightsT>::lower_bound_index(int low, int up, WeightsT val)
{
    int mid = (low+up)/2;
    if(_kumWeights[mid] <= val && val < _kumWeights[mid+1])
        return mid;
    else if(val < _kumWeights[mid])
        return lower_bound_index(low, mid, val);
    else
        return lower_bound_index(mid+1, up, val);
}


template <typename WeightsT>
std::string WeightedNumberGentor<WeightsT>::asString()const
{
    std::string result;
    for(int idx=0; idx < size(); idx++)
    {
        result += to_string<int>::apply(idx);
        result += ":";
        result += _typeNames[idx];
        result += "(";
        result += to_string<int>::apply(_weights[idx]);
        result += ")\n";
    }
    return result;
}

template <typename WeightsT>
std::string WeightedNumberGentor<WeightsT>::inconsitencyMessage(const std::string& location)const
{
    std::string message;
    message += "Inconsistent typechoice in ";
    message += location + ":\n";
    message += "The sum of weights must be ";
    message += to_string<int>::apply(maxWeights());
    message += " but is ";
    message += to_string<int>::apply(sumOfWeights());
    message += "\n";
    message += "The weights defined are:\n";
    message += asString();
    return message;
}

}} // namespace itl boost

#endif // __NUMBERGENTORT_H_JOFA_000725__




