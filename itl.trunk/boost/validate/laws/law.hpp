/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_LAW_HPP_JOFA_070411
#define BOOST_ITL_LAW_HPP_JOFA_070411

#include <string>
#include <set>
#include <boost/validate/loki_xt/Tuple.h>

namespace boost{namespace itl
{

    /** abstract template class Law:
        A Law can hold for a given set of variables.
        A Law together with the set of input variables is an instance of the law.
        The evaluation of the law results in a set of output variables.
        Output variables hold interim and final results of the evaluation of the law.
    */
    template 
    <
        class SubType,
        typename InputTypes, typename OutputTypes
    >
    class Law
    {
    public:
        typedef SubType                           sub_type;
        typedef InputTypes                        input_types;
        typedef OutputTypes                       output_types;
        typedef typename Loki::tuple<InputTypes>  input_tuple;
        typedef typename Loki::tuple<OutputTypes> output_tuple;

    public:
        bool holds(){ return that()->holds(); }
        bool debug_holds(){ return that()->debug_holds(); }

        void setInstance(const input_tuple& inVars)
        { _inputTuple = inVars; }

        void getInstance(input_tuple& inVars, output_tuple& outVars)const
        { inVars = _inputTuple; outVars = _outputTuple; }

        void getInputInstance(input_tuple& inVars)const
        { inVars = _inputTuple; }

        void getOutputInstance(output_tuple& outVars)const
        { outVars = _outputTuple; }

        size_t size()const{ return that()->size(); }

        bool operator == (const Law& rhs)const
        { return size() == rhs.size(); }

        bool operator < (const Law& rhs)const
        { return size() < rhs.size(); }

        std::string name()const       { return that()->name(); }
        std::string formula()const    { return that()->formula(); }
        std::string typeString()const { return that()->typeString(); }

        template<unsigned int index>
        typename Loki::TL::TypeAt<InputTypes, index>::Result 
            setInputValue(const typename Loki::TL::TypeAt<InputTypes, index>::Result& value) 
        { return Loki::tup::refer<index>(_inputTuple)=value; }

        template<unsigned int index>
        typename Loki::TL::TypeAt<InputTypes, index>::Result getInputValue()const 
        { return Loki::tup::get<index>(_inputTuple); }

        template<unsigned int index>
        typename Loki::TL::TypeAt<OutputTypes, index>::Result 
            setOutputValue(const typename Loki::TL::TypeAt<OutputTypes, index>::Result& value) 
        { return Loki::tup::refer<index>(_outputTuple)=value; }

        template<unsigned int index>
        typename Loki::TL::TypeAt<OutputTypes, index>::Result getOutputValue()const 
        { return Loki::tup::get<index>(_outputTuple); }

    protected:
              sub_type* that()      { return static_cast      <sub_type*>(this); }
        const sub_type* that()const { return static_cast<const sub_type*>(this); }

    private:
        input_tuple  _inputTuple;
        output_tuple _outputTuple;
    };


    enum InputVarIndex  { operand_a, operand_b, operand_c, operand_d, operand_e };
    enum OutputVarIndex { lhs_result, rhs_result };

}} // namespace itl boost

#endif //BOOST_ITL_LAW_HPP_JOFA_070411

