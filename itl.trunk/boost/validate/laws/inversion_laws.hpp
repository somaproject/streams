/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INVERSION_LAWS_HPP_JOFA_071124
#define BOOST_ITL_INVERSION_LAWS_HPP_JOFA_071124

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/itl/functors.hpp>

namespace boost{namespace itl
{

    template <typename Type, 
              template<class>class Combiner = inplace_plus,
              template<class>class Equality = itl::std_equal>
    class InplaceNaturalInversion 
        : public Law<InplaceNaturalInversion<Type,Combiner,Equality>, 
                     LOKI_TYPELIST_1(Type), LOKI_TYPELIST_2(Type,Type)>
    {
        //a - a == 0
        //computed using inplace operators +=
        //Input  = (a := inVal1, b := inVal2)
        //Output = (lhs_result, rhs_result)

    public:
        typedef typename inverse<Combiner<Type> >::type InverseCombinerT;

        std::string name()const { return "InplaceNaturalInversion"; }
        std::string formula()const { return "a -= a; a == 0"; }

        std::string typeString()const
        {
            return "NaturalInversion<"+type_to_string<Type>::apply()+","
                                      +unary_template_to_string<Combiner>::apply()+","
                                      +unary_template_to_string<Equality>::apply()
                                      +">";
        }

    public:

        bool holds()
        {
            Type lhs = this->template getInputValue<operand_a>();
            InverseCombinerT()(lhs, this->template getInputValue<operand_a>());

            Type rhs = Combiner<Type>::neutron();

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        { 
            return holds();
        }

        size_t size()const { return value_size<Type>::apply(this->template getInputValue<operand_a>()); }
    };


    template <typename Type, 
              template<class>class Combiner = inplace_plus,
              template<class>class Equality = itl::std_equal>
    class InplaceInverseExistence 
        : public Law<InplaceInverseExistence<Type,Combiner,Equality>, 
                     LOKI_TYPELIST_1(Type), LOKI_TYPELIST_2(Type,Type)>
    {
        //(0 - a) + a == 0
        //computed using inplace operators +=
        //Input  = (a := inVal1, b := inVal2)
        //Output = (lhs_result, rhs_result)

    public:
        typedef typename inverse<Combiner<Type> >::type InverseCombinerT;

        std::string name()const { return "InplaceInverseExistence"; }
        std::string formula()const { return "(0-a) + a == 0"; }

        std::string typeString()const
        {
            return "InverseExistence<"+type_to_string<Type>::apply()+","
                                      +unary_template_to_string<Combiner>::apply()+","
                                      +unary_template_to_string<Equality>::apply()
                                      +">";
        }

    public:

        bool holds()
        {
            Type lhs = Combiner<Type>::neutron(); 
            Type value_a = this->template getInputValue<operand_a>();
            // lhs = (0 - a)
            InverseCombinerT()(lhs, value_a);
            // lhs = (0 - a) + a
            Combiner<Type>()(lhs, value_a);

            Type rhs = Combiner<Type>::neutron();

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        { 
            return holds();
        }

        size_t size()const { return value_size<Type>::apply(this->template getInputValue<operand_a>()); }
    };

}} // namespace itl boost

#endif // BOOST_ITL_INVERSION_LAWS_HPP_JOFA_071124


