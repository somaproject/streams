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

#include <iostream>
#include <stdio.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/validate/loki_xt/Tuple.h>
#include <boost/itl/set.hpp>
#include <boost/itl/map.hpp>
#include <boost/validate/gentor/randomgentor.hpp>

#include <boost/validate/validation_counts.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/laws/law_violations.hpp>

namespace boost{namespace itl
{
    typedef itl::map<std::string, validation_counts> ValidationCounterT;
    typedef itl::map<std::string, int> ViolationCounterT;
    typedef itl::map<std::string, PolyLawViolations> ViolationMapT;

    class LawValidaterI
    {
    public:
        virtual ~LawValidaterI(){}
        virtual void init()=0;
        virtual void run()=0;
        virtual void addFrequencies(ValidationCounterT&)=0;
        virtual void addViolations(ViolationCounterT&, ViolationMapT&)=0;
    };


    template <class LawT, template<typename>class GentorT>
    class LawValidater : public LawValidaterI
    {
    public:
        typedef typename LawT::input_types    input_types;
        typedef typename LawT::output_types   output_types;
        typedef typename LawT::input_tuple    input_tuple;
        typedef typename LawT::output_tuple   output_tuple;
        
        typedef typename Loki::TL::MapType<GentorT, input_types>::Result gentor_types;
        typedef typename Loki::tuple<gentor_types> input_gentor;

    public:
#ifdef _DEBUG
        static const int default_trials_count = 10;
        static const int default_repeat_count = 20;
#else
        static const int default_trials_count = 20;
        static const int default_repeat_count = 10;
#endif
        LawValidater()
        { 
            _repeat_count   = GentorProfileSgl::it()->repeat_count(); 
            _trials_count   = GentorProfileSgl::it()->trials_count(); 
        }

        void set_trials_count(int trials) 
        {
            _trials_count = trials;
            _silent_trials_count = std::max(1, _trials_count / 10);
        }

        void set_repeat_count(int repeats) 
        {
            _repeat_count = std::max(1, repeats);
        }

        void set_silent_trials_count(int trials) { _trials_count = trials; }

        void init();
        void run();
        void addFrequencies(ValidationCounterT& summary) { summary += _frequencies; }
        
        void addViolations(ViolationCounterT& summary, ViolationMapT& collector)   
        {
            summary += ViolationCounterT::value_type(lawType(), static_cast<int>(_lawViolations.size())); 
            if(!_lawViolations.empty())
                collector += ViolationMapT::value_type(lawType(), PolyLawViolations(new LawViolationsT(_lawViolations)));
        }

        std::string lawType()const{ return _law.typeString(); }

        void reportLawInfo()const;
        void reportProgress()const;
        void reportViolations()const;
        void reportSuccess()const;

    private:
        typedef LawViolations<LawT> LawViolationsT;

    private:
        input_gentor _gentor;
        LawT         _law;

        int _trials_count;
        int _silent_trials_count;
        int _repeat_count;

        LawViolationsT     _lawViolations;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
    }; //class LawValitater


    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::init()
    {
        _frequencies.clear();
        _lawViolations.clear();
        _violations.clear();
    }

    // Runs law_instance_count * repeat_count validations on the law LawT
    // law_instance_count: Number of instances that are validated for LawT
    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::run()
    {
        // apply Calibrater.apply to each element of the generator-tuple _gentor
        // This is just an initialisation of the generators according to system
        // defaults for convenience purposes.
        _gentor.template apply<GentorT, Calibrater, input_tuple>();

        // Input values that are to be generated on every iteration
        input_tuple values;
        posix_time::ptime start, stop;
        double validation_time = 0.0; //microseconds

        for(int idx=0; idx<_trials_count; idx++)
        {
            // Apply the function SomeValue to each component of the input tuple
            _gentor.template map_template<GentorT, SomeValue>(values);
            _law.setInstance(values);

            bool law_is_violated = false;
            start = posix_time::ptime(posix_time::microsec_clock::local_time());
            // In order to measure small time intervals, evaluation must be repeated.
            for(int repeat=0; repeat<_repeat_count; repeat++)
                law_is_violated = !_law.holds();

            stop = posix_time::ptime(posix_time::microsec_clock::local_time());
            validation_time += static_cast<double>((stop - start).total_microseconds());

            if(law_is_violated)
                _lawViolations.insert(_law);

        }

        // Average time for one law evaluation in micro seconds
        double avg_validation_time = validation_time/(_trials_count * _repeat_count);

        if(!_lawViolations.empty())
        {
            reportViolations();
            LawT violation = *(_lawViolations.begin());

            input_tuple  inVars;
            output_tuple outVars;
            violation.getInstance(inVars, outVars);
            std::cout << "Violation of: " << _law.typeString() << std::endl;
            std::cout << _law.formula()      << std::endl;
            std::cout <<  inVars.as_string() << std::endl;
            std::cout << outVars.as_string() << std::endl;

            violation.debug_holds();
        }
        else
        {
            //reportSuccess();
            _frequencies +=
                ValidationCounterT::value_type(lawType(), 
                                               validation_counts(avg_validation_time));
        }

    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportLawInfo()const
    {
        std::cout << "----------------------------------------------------------" << std::endl;
        std::cout << "Validation of law: " << std::endl;
        std::cout << _law.name() << std::endl;
        std::cout << _law.formula() << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportProgress()const
    {
        typename LawT::input_tuple input_values;
        _law.getInputInstance(input_values);
        //JODO think about more general mechanics of progress reporting here
        std::cout << "SomeValues " << input_values.as_string() << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportViolations()const
    {
        std::cout << static_cast<int>(_lawViolations.getViolationsCount()) << " violations" << std::endl;
    }

    template <class LawT, template<typename>class GentorT>
    void LawValidater<LawT, GentorT>::reportSuccess()const
    {
        std::cout << "Law successfully validated for " << _trials_count << " cases" << std::endl;
    }

}} // namespace itl boost

// ----------------------------------------------------------------------------

