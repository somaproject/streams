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

#include <boost/itl/functors.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/laws/symmetric_difference.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{

typedef WeightedNumberGentor<int> ChoiceT;

template <typename Type>
class itl_set_validater : public algebra_validater
{
public:

    enum Laws 
    { 
        inplacePlusAssociativity,
        inplacePlusNeutrality,
        inplacePlusCommutativity,
        inplaceEtAssociativity,
        inplaceEtCommutativity,
        inplaceNaturalInversion,
        inplaceSymmetricDifference,
        inplaceFlip,
        inplacePlusDistributivity,
        inplaceEtDistributivity,
        inplacePlusDashRightDistrib,
        inplaceEtDashRightDistrib,
        inplacePlusDeMorgan,
        inplaceEtDeMorgan,
        Laws_size 
    };

    itl_set_validater() 
    {
        setProfile();
    }

    void setProfile()
    {
        const int sum_of_shares = 100;
        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[inplacePlusAssociativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusNeutrality]      = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusCommutativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtAssociativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtCommutativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceNaturalInversion]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceSymmetricDifference] = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceFlip]                = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDistributivity]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDistributivity]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDashRightDistrib]= share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDashRightDistrib]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDeMorgan]        = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDeMorgan]          = share(Laws_size, item_index, rest_shares);
        _lawChoice.init();
    }


    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case inplacePlusAssociativity:  return new LawValidater<InplaceAssociativity<Type, inplace_plus>, RandomGentor>;
        case inplacePlusNeutrality:     return new LawValidater<InplaceNeutrality   <Type>,               RandomGentor>;
        case inplacePlusCommutativity:  return new LawValidater<InplaceCommutativity<Type>,               RandomGentor>;
        case inplaceEtAssociativity:    return new LawValidater<InplaceAssociativity<Type, inplace_et>,   RandomGentor>;
        case inplaceEtCommutativity:    return new LawValidater<InplaceCommutativity<Type, inplace_et>,   RandomGentor>;
        case inplaceSymmetricDifference:return new LawValidater<InplaceSymmetricDifference<Type>,         RandomGentor>;
        case inplaceFlip:               return new LawValidater<InplaceFlip<Type>,   RandomGentor>;
        case inplaceEtDistributivity:  
            if(itl::is_interval_splitter<Type>::value)
                                        return new LawValidater<InplaceDistributivity<Type, inplace_et, inplace_plus, element_equal>, RandomGentor>;
            else                        return new LawValidater<InplaceDistributivity<Type, inplace_et, inplace_plus, std_equal>, RandomGentor>;
        case inplacePlusDashRightDistrib:
            if(itl::is_interval_splitter<Type>::value)
                                        return new LawValidater<InplaceRightDistributivity<Type, inplace_plus, inplace_minus, element_equal>, RandomGentor>;
            else                        return new LawValidater<InplaceRightDistributivity<Type, inplace_plus, inplace_minus, std_equal>, RandomGentor>;
        case inplaceEtDashRightDistrib:    return new LawValidater<InplaceRightDistributivity<Type, inplace_et, inplace_minus>, RandomGentor>;
        case inplacePlusDeMorgan:         return new LawValidater<InplaceDeMorgan<Type, inplace_plus, inplace_et, itl::std_equal>, RandomGentor>;
        case inplaceEtDeMorgan:        
            if(itl::is_interval_splitter<Type>::value || itl::is_interval_separator<Type>::value)
                                        return new LawValidater<InplaceDeMorgan<Type, inplace_et, inplace_plus, itl::element_equal>, RandomGentor>;
            else                        return new LawValidater<InplaceDeMorgan<Type, inplace_et, inplace_plus, itl::std_equal>, RandomGentor>;

        default: return NULL;
        }
    }

    void validate()
    {
        _validater = chooseValidater();
        if(_validater)
        {
            _validater->run();
            _validater->addFrequencies(_frequencies);
            _validater->addViolations(_violationsCount, _violations);
            delete _validater;
        }
    }

    void addFrequencies(ValidationCounterT& summary) { summary += _frequencies; }
    void addViolations(ViolationCounterT& summary, ViolationMapT& collector)
    { 
        summary += _violationsCount; 
        collector += _violations;  
    }

private:
    ChoiceT        _lawChoice;
    LawValidaterI* _validater;
    ValidationCounterT _frequencies;
    ViolationCounterT  _violationsCount;
    ViolationMapT      _violations;
};


}} // namespace itl boost

