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
#include <boost/itl/interval_morphism.hpp>
#include <boost/validate/laws/induced_relation.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;


    template <typename Type>
    class itl_induced_relations_validater : public algebra_validater
    {
    public:
        enum Laws 
        { 
            atomize_subset,
            atomize_superset,
            atomize_element_equal,
            Laws_size 
        };

        itl_induced_relations_validater() {setProfile();}

        void setProfile()
        {
            const int volume = 100;
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(volume);
            _lawChoice[atomize_subset]          = 33;
            _lawChoice[atomize_superset]        = 33;
            _lawChoice[atomize_element_equal]   = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case atomize_subset:        return new LawValidater<InducedRelation<Type, typename Type::atomized_type, Interval::Atomize, sub_super_set>, RandomGentor>();
            case atomize_superset:      return new LawValidater<InducedRelation<Type, typename Type::atomized_type, Interval::Atomize, super_sub_set>, RandomGentor>();
            case atomize_element_equal: return new LawValidater<InducedRelation<Type, typename Type::atomized_type, Interval::Atomize, element_equal>, RandomGentor>();
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
    }; //class itl_induced_relations_validater



}} // namespace itl boost

