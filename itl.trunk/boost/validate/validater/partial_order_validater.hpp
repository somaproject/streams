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
#include <boost/validate/laws/order.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;

    template <typename Type, template<class>class Relation, template<class>class Equality = itl::std_equal>
    class partial_order_validater : public algebra_validater
    {
    public:
        enum Laws 
        { 
            transitivity,
            antisymmetry,
            reflexivity,
            Laws_size 
        };

        partial_order_validater() {setProfile();}

        void setProfile()
        {
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(100);
            _lawChoice[transitivity]  = 33;
            _lawChoice[antisymmetry]  = 33;
            _lawChoice[reflexivity]   = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case transitivity: return new LawValidater<Transitivity<Type, Relation>, RandomGentor>;
            case antisymmetry: return new LawValidater<Antisymmetry<Type, Relation, Equality>, RandomGentor>;
            case reflexivity:  return new LawValidater<Reflexivity <Type, Relation>, RandomGentor>;
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

