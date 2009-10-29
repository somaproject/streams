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

#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/functors.hpp>
#include <boost/validate/validater/strict_weak_order_validater.hpp>
#include <boost/validate/validater/partial_order_validater.hpp>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;


    template <typename Type>
    class itl_order_validater : public algebra_validater
    {
    public:
        typedef strict_weak_order_validater<Type, std::less> LessValidaterT;
        typedef partial_order_validater<Type, std::less_equal> LessEqualValidaterT;
        typedef partial_order_validater<Type, itl::sub_super_set, itl::element_equal> ContainedInValidaterT;

        enum Laws 
        { 
            strictWeakStdOrder,
            partialStdOrder,
            containedInOrder,
            Laws_size 
        };

        itl_order_validater() {setProfile();}

        void setProfile()
        {
            const int sum_of_weights = 100;
            _lawChoice.setSize(sum_of_weights);
            _lawChoice.setMaxWeights(sum_of_weights);
            _lawChoice[strictWeakStdOrder]         = 33;
            _lawChoice[partialStdOrder]            = 33;
            _lawChoice[containedInOrder]           = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case strictWeakStdOrder:         return _lessValidater.chooseValidater();
            case partialStdOrder:            return _lessEqualValidater.chooseValidater();
            case containedInOrder:           return _containedInValidater.chooseValidater();
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

        LessValidaterT        _lessValidater;
        LessEqualValidaterT   _lessEqualValidater;
        ContainedInValidaterT _containedInValidater;
    };

}} // namespace itl boost

