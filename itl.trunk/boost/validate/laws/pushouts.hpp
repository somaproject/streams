/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_PUSHOUTS_HPP_JOFA_071124
#define BOOST_ITL_PUSHOUTS_HPP_JOFA_071124

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{

    // ---------------------------------------------------------------------------
    //  (a,b) ---o---> c
    //    |            |
    //    |f           |f
    //    V            V
    // (a',b')---o---> c'
    // ---------------------------------------------------------------------------
    template <typename SourceT, typename TargetT, 
              template<class,class>class FunctionT, // morphic function f 
              template<typename>class OperatorT>    // operator like +=
    class BinaryPushout : 
        public Law<BinaryPushout<SourceT,TargetT,FunctionT,OperatorT>, 
                   LOKI_TYPELIST_2(SourceT, SourceT), LOKI_TYPELIST_2(TargetT,TargetT)>
    {
        /** f(a o b) == f(a) o f(b)
        computed using inplace operators  o=
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "Pushout"; }
        std::string formula()const { return "cont a, cont b: f(a o b) == f(a) o f(b) 'inplace'"; }

        std::string typeString()const
        {
            return
                "Pushout<"+type_to_string<SourceT>::apply()+","
                          +type_to_string<TargetT>::apply()+","
                          +binary_template_to_string<FunctionT>::apply()+","
                          +unary_template_to_string<OperatorT>::apply()+">";
        }

    public:

        bool holds()
        {
            // f(a o b) == f(a) o f(b)
            // --- left hand side ------------------------
            // lhs := f(a o b)
            SourceT a_o_b = this->template getInputValue<operand_a>();
            // a_o_b *=  this->template getInputValue<operand_b>();
            OperatorT<SourceT>()(a_o_b, this->template getInputValue<operand_b>());

            TargetT lhs;
            FunctionT<TargetT,SourceT>()(lhs, a_o_b);

            // --- right hand side -----------------------
            // rhs := atomize(a) * atomize(b)
            SourceT value_a = this->template getInputValue<operand_a>();
            TargetT atomic_a;
            FunctionT<TargetT,SourceT>()(atomic_a, value_a);
            SourceT value_b = this->template getInputValue<operand_b>();
            TargetT atomic_b;
            FunctionT<TargetT,SourceT>()(atomic_b, value_b);
            TargetT rhs = atomic_a;
            OperatorT<TargetT>()(rhs, atomic_b);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        size_t size()const 
        { 
            return 
                value_size<SourceT>::apply(this->template getInputValue<operand_a>())+
                value_size<SourceT>::apply(this->template getInputValue<operand_b>());
        }

        bool debug_holds()
        {
            std::cout << typeString() << std::endl;
            std::cout << formula() << std::endl;
            std::cout << "a: " << this->template getInputValue<operand_a>().as_string() << std::endl;
            std::cout << "b: " << this->template getInputValue<operand_b>().as_string() << std::endl;

            // f(a o b) == f(a) o f(b)
            // --- left hand side ------------------------
            // lhs := f(a o b)
            SourceT a_o_b = this->template getInputValue<operand_a>();
            // a_o_b o=  this->template getInputValue<operand_b>();
            OperatorT<SourceT>()(a_o_b, this->template getInputValue<operand_b>());
            std::cout << "a o b:" << a_o_b.as_string() << std::endl;

            TargetT lhs;
            FunctionT<TargetT,SourceT>()(lhs, a_o_b);
            std::cout << "f(a o b):" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            // rhs := atomize(a) * atomize(b)
            SourceT value_a = this->template getInputValue<operand_a>();
            TargetT atomic_a;
            FunctionT<TargetT,SourceT>()(atomic_a, value_a);
            std::cout << "f(a):" << atomic_a.as_string() << std::endl;
            SourceT value_b = this->template getInputValue<operand_b>();
            TargetT atomic_b;
            FunctionT<TargetT,SourceT>()(atomic_b, value_b);
            std::cout << "f(b):" << atomic_b.as_string() << std::endl;
            TargetT rhs = atomic_a;
            OperatorT<TargetT>()(rhs, atomic_b);
            std::cout << "f(a) o f(b):" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

    }; //class BinaryPushout


}} // namespace itl boost

#endif // BOOST_ITL_PUSHOUTS_HPP_JOFA_071124

