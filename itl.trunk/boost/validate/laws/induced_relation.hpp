/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INDUCED_RELATION_HPP_JOFA_090204
#define BOOST_ITL_INDUCED_RELATION_HPP_JOFA_090204

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{
    // An InducedRelation can be represented as a triangular commuting diagram
    // where f is a unary function and R a binary predicate or relation
    // ---------------------------------------------------------------------------
    //  (a,b)--f-->(a',b')
    //     \        /
    //    R \  ==  / R
    //       \    /
    //        V  V
    //    {true,false}
    // ---------------------------------------------------------------------------
    template <typename SourceT, typename TargetT, 
              template<class,class>class FunctionT, // morphic function f 
              template<typename>class RelationT>    // binary predicate like ==
    class InducedRelation : 
        public Law<InducedRelation<SourceT,TargetT,FunctionT,RelationT>, 
                   LOKI_TYPELIST_2(SourceT, SourceT), LOKI_TYPELIST_2(bool,bool)>
    {
        /** a rel b == f(a) rel f(b)
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InducedRelation"; }
        std::string formula()const { return "cont a, cont b: a rel b == f(a) rel f(b)"; }

        std::string typeString()const
        {
            return
                "InducedRel<"+type_to_string<SourceT>::apply()+","
                          +type_to_string<TargetT>::apply()+","
                          +binary_template_to_string<FunctionT>::apply()+","
                          +unary_template_to_string<RelationT>::apply()+">";
        }

    public:

        bool holds()
        {
            // a rel b == f(a) rel f(b)
            // --- left hand side ------------------------
            // lhs := a rel b
            SourceT a = this->template getInputValue<operand_a>();
            SourceT b = this->template getInputValue<operand_b>();
            bool lhs = RelationT<SourceT>()(a,b);
            // --- right hand side -----------------------
            TargetT f_a, f_b;
            FunctionT<TargetT,SourceT>()(f_a, a);
            FunctionT<TargetT,SourceT>()(f_b, b);
            bool rhs = RelationT<TargetT>()(f_a, f_b);

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
            return holds();
        }

    }; //class InducedRelation


}} // namespace itl boost

#endif // BOOST_ITL_INDUCED_RELATION_HPP_JOFA_090204

