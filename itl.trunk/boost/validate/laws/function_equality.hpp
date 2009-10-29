/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTION_EQUALITY_HPP_JOFA_091005
#define BOOST_ITL_FUNCTION_EQUALITY_HPP_JOFA_091005

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{

// -----------------------------------------------------------------------------
//      S
//    /   \
//  f|  =  |g
//    \   /
//     v v
//      T
// -----------------------------------------------------------------------------
template <typename SourceT, typename TargetT,// domain, codomain types of f,g
          template<class,class>class Function_f,
          template<class,class>class Function_g,
          template<class>class Equality = itl::std_equal> 
class FunctionEquality : 
    public Law<FunctionEquality<SourceT,TargetT,Function_f,Function_g,Equality>, 
               LOKI_TYPELIST_1(SourceT), LOKI_TYPELIST_2(TargetT,TargetT)>
{
    /** S: SourceT, T: TargetT
    For all S a: f(a) == g(a) : Equality of functions f and g. 
    Input  = (a := inVal1)
    Output = (lhs_result, rhs_result)
    */
public:
    std::string name()const { return "FunctionEquality"; }
    std::string formula()const { return "S a: f(a) == g(a)"; }

    std::string typeString()const
    {
        return
            "Eq<"+type_to_string<SourceT>::apply()+","
                 +type_to_string<TargetT>::apply()+","
                 +binary_template_to_string<Function_f>::apply()+","
                 +binary_template_to_string<Function_g>::apply()+","
                 +unary_template_to_string<Equality>::apply()   +">";
    }

public:

    bool holds()
    {
        // For all S a: f(a) == g(a) : f == g. 
        // --- left hand side --------------------------------------------------
        SourceT value_a = this->template getInputValue<operand_a>();
        TargetT lhs;
        Function_f<SourceT,TargetT>()(lhs, value_a); // lhs = f(a);

        // --- right hand side -------------------------------------------------
        TargetT rhs;
        Function_g<SourceT,TargetT>()(rhs, value_a); // rhs = g(a);

        this->template setOutputValue<lhs_result>(lhs);
        this->template setOutputValue<rhs_result>(rhs);

        return Equality<TargetT>()(lhs, rhs);
    }

    bool debug_holds()
    { 
        // For all S a: f(a) == g(a) : f == g. 
        // --- left hand side --------------------------------------------------
        SourceT value_a = this->template getInputValue<operand_a>();
        std::cout << "a= " << value_a << std::endl;
        TargetT lhs;
        Function_f<SourceT,TargetT>()(lhs, value_a); // lhs = f(a);
        std::cout << "l= " << lhs << std::endl;

        // --- right hand side -------------------------------------------------
        TargetT rhs;
        Function_g<SourceT,TargetT>()(rhs, value_a); // rhs = g(a);
        std::cout << "r= " << rhs << std::endl;

        this->template setOutputValue<lhs_result>(lhs);
        this->template setOutputValue<rhs_result>(rhs);

        return Equality<TargetT>()(lhs, rhs);
    }

    size_t size()const 
    { 
        return value_size<SourceT>::apply(this->template getInputValue<operand_a>());
    }
};

}} // namespace itl boost

#endif // BOOST_ITL_MINOR_SET_LAWS_HPP_JOFA_070411

