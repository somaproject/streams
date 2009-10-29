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

#include <boost/validate/itl/functors.hpp>
#include <boost/validate/laws/function_equality.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;


    template <typename SourceT, typename TargetT>
    class function_equality_validater : public algebra_validater
    {
    public:
        enum Laws 
        { 
            insertion_loop_hint,
            insertion_loop_copy,
            addition_loop_hint,
            addition_loop_copy,
            Laws_size 
        };

        function_equality_validater() {setProfile();}

        void setProfile()
        {
            const int volume = 100;
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(volume);
            _lawChoice[insertion_loop_hint]     = 25;
            _lawChoice[insertion_loop_copy]     = 25;
            _lawChoice[addition_loop_hint]      = 25;
            _lawChoice[addition_loop_copy]      = 25;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case insertion_loop_hint: return new LawValidater<FunctionEquality<SourceT, TargetT, base_insertion, hint_insertion>, RandomGentor>();
            case insertion_loop_copy: return new LawValidater<FunctionEquality<SourceT, TargetT, base_insertion, copy_insertion>, RandomGentor>();
            case addition_loop_hint:  return new LawValidater<FunctionEquality<SourceT, TargetT, base_addition,  hint_addition>,  RandomGentor>();
            case addition_loop_copy:  return new LawValidater<FunctionEquality<SourceT, TargetT, base_addition,  copy_addition>,  RandomGentor>();
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
            summary   += _violationsCount; 
            collector += _violations;  
        }

    private:
        ChoiceT        _lawChoice;
        LawValidaterI* _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
    }; //class function_equality_validater



}} // namespace itl boost

