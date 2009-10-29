/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SET_LAWS_HPP_JOFA_071124
#define BOOST_ITL_SET_LAWS_HPP_JOFA_071124

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/itl/functors.hpp>

namespace boost{namespace itl
{

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Operator1 = inplace_plus, 
                             template<class>class Operator2 = inplace_et,
                             template<class>class Equality  = itl::std_equal>
    class InplaceDistributivity 
        : public Law<InplaceDistributivity<Type,Operator1,Operator2,Equality>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a + (b * c) == (a + b) * (a + c)
            a(1)(b(2)c) == (a(1)b)(2)(a(1)c)
        computed using inplace operators +=, += and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplaceDistributivity"; }
        std::string formula()const { return "a + (b * c) == (a + b) * (a + c) 'inplace'"; }

        std::string typeString()const
        {
            return "Distributivity<"+type_to_string<Type>::apply()+","
                                    +unary_template_to_string<Operator1>::apply()+","
                                    +unary_template_to_string<Operator2>::apply()+","
                                    +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            // a + (b * c) == (a + b) * (a + c)
            // --- left hand side ------------------------
            Type b_star_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_star_c, this->template getInputValue<operand_c>());

            // lhs := a + (b * c)
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, b_star_c);

            // --- right hand side -----------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_b, this->template getInputValue<operand_b>());

            Type a_plus_c = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_c, this->template getInputValue<operand_c>());

            // rhs := (a + b) * (a + c)
            Type rhs = a_plus_b;
            Operator2<Type>()(rhs, a_plus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        {
            // a + (b * c) == (a + b) * (a + c)
            std::cout << "a:" << this->template getInputValue<operand_a>().as_string() << std::endl;
            std::cout << "b:" << this->template getInputValue<operand_b>().as_string() << std::endl;
            std::cout << "c:" << this->template getInputValue<operand_c>().as_string() << std::endl;
            // --- left hand side ------------------------
            Type b_star_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_star_c, this->template getInputValue<operand_c>());
            std::cout << "b*c:" << b_star_c.as_string() << std::endl;

            // lhs := a + (b * c)
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, b_star_c);
            std::cout << "l=a+(b*c):" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_b, this->template getInputValue<operand_b>());
            std::cout << "a+b:" << a_plus_b.as_string() << std::endl;

            Type a_plus_c = this->template getInputValue<operand_a>();
            Operator1<Type>()(a_plus_c, this->template getInputValue<operand_c>());
            std::cout << "a+c:" << a_plus_c.as_string() << std::endl;

            // rhs := (a + b) * (a + c)
            Type rhs = a_plus_b;
            Operator2<Type>()(rhs, a_plus_c);
            std::cout << "r=(a+b)*(a+c):" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Operator1   = inplace_plus, 
                             template<class>class Operator2   = inplace_et, 
                             template<class>class Subtraction = inplace_minus, 
                             template<class>class Equality    = itl::std_equal>
    class InplaceDeMorgan 
        : public Law<InplaceDeMorgan<Type,Operator1,Operator2,Subtraction,Equality>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a - (b + c) == (a - b) & (a - c)
            a - (b(1)c) == (a - b)(2)(a - c)
        computed using inplace operators +=, += and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplacePlusDeMorgan"; }
        std::string formula()const { return "a - (b + c) == (a - b) & (a - c) 'inplace'"; }

        std::string typeString()const
        {
            return "DeMorgan<"+type_to_string<Type>::apply()+","
                              +unary_template_to_string<Operator1>::apply()+","
                              +unary_template_to_string<Operator2>::apply()+","
                              +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            // a - (b + c) == (a - b) & (a - c)
            // --- left hand side ------------------------
            Type b_plus_c = this->template getInputValue<operand_b>();
            Operator1<Type>()(b_plus_c, this->template getInputValue<operand_c>());

            // lhs := a - (b + c)
            Type lhs = this->template getInputValue<operand_a>();
            Subtraction<Type>()(lhs, b_plus_c);

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_b, this->template getInputValue<operand_b>());

            Type a_minus_c = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_c, this->template getInputValue<operand_c>());

            // rhs := (a - b) & (a - c)
            Type rhs = a_minus_b;
            Operator2<Type>()(rhs, a_minus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        { 
            // a - (b + c) == (a - b) & (a - c)
            Type val_a = this->template getInputValue<operand_a>();
            Type val_b = this->template getInputValue<operand_b>();
            Type val_c = this->template getInputValue<operand_c>();
            std::cout << "a = " << val_a << std::endl;
            std::cout << "b = " << val_b << std::endl;
            std::cout << "c = " << val_c << std::endl;
            // --- left hand side ------------------------
            Type b_plus_c = val_b;
            Operator1<Type>()(b_plus_c, val_c);

            // lhs := a - (b + c)
            Type lhs = this->template getInputValue<operand_a>();
            Subtraction<Type>()(lhs, b_plus_c);

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_b, this->template getInputValue<operand_b>());
            std::cout << "a-b = " << a_minus_b << std::endl;

            Type a_minus_c = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_c, this->template getInputValue<operand_c>());
            std::cout << "a-c = " << a_minus_c << std::endl;

            // rhs := (a - b) & (a - c)
            Type rhs = a_minus_b;
            Operator2<Type>()(rhs, a_minus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, 
              template<class>class Operator1 = inplace_plus, 
              template<class>class Operator2 = inplace_minus,
              template<class>class Equality = itl::std_equal>
    class InplaceRightDistributivity 
        : public Law<InplaceRightDistributivity<Type,Operator1,Operator2>, 
                     LOKI_TYPELIST_3(Type,Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** (a + b) - c == (a - c) + (b - c)
        computed using inplace operators +=, -= and &=
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "InplaceRightDistributivity"; }
        std::string formula()const { return "(a + b) - c == (a - c) + (b - c) 'inplace'"; }

        std::string typeString()const
        {
            return "RightDistributivity<"+type_to_string<Type>::apply()+","
                                         +unary_template_to_string<Operator1>::apply()+","
                                         +unary_template_to_string<Operator2>::apply()+","
                                         +unary_template_to_string<Equality>::apply() +">";
        }

    public:

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }

        bool holds()
        {
            // (a + b) - c == (a - c) + (b - c)
            // --- left hand side ------------------------
            // lhs := (a + b) - c
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, this->template getInputValue<operand_b>());
            Operator2<Type>()(lhs, this->template getInputValue<operand_c>());

            // --- right hand side -----------------------
            Type a_minus_c = this->template getInputValue<operand_a>();
            Operator2<Type>()(a_minus_c, this->template getInputValue<operand_c>());

            Type b_minus_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_minus_c, this->template getInputValue<operand_c>());

            // rhs := (a - c) + (b - c)
            Type rhs = a_minus_c;
            Operator1<Type>()(rhs, b_minus_c);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        bool debug_holds()
        {
            // (a + b) - c == (a - c) + (b - c)
            std::cout << "a:" << this->template getInputValue<operand_a>().as_string() << std::endl;
            std::cout << "b:" << this->template getInputValue<operand_b>().as_string() << std::endl;
            std::cout << "c:" << this->template getInputValue<operand_c>().as_string() << std::endl;
            // --- left hand side ------------------------
            // lhs := (a + b) - c
            Type lhs = this->template getInputValue<operand_a>();
            Operator1<Type>()(lhs, this->template getInputValue<operand_b>());
            std::cout << "a+b:" << lhs.as_string() << std::endl;
            Operator2<Type>()(lhs, this->template getInputValue<operand_c>());
            std::cout << "l=(a+b)-c:" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_minus_c = this->template getInputValue<operand_a>();
            Operator2<Type>()(a_minus_c, this->template getInputValue<operand_c>());
            std::cout << "a-c:" << a_minus_c.as_string() << std::endl;

            Type b_minus_c = this->template getInputValue<operand_b>();
            Operator2<Type>()(b_minus_c, this->template getInputValue<operand_c>());
            std::cout << "b-c:" << b_minus_c.as_string() << std::endl;

            // rhs := (a - c) + (b - c)
            Type rhs = a_minus_c;
            Operator1<Type>()(rhs, b_minus_c);
            std::cout << "r=(a-c)+(b-c):" << b_minus_c.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }
    };

}} // namespace itl boost

#endif // BOOST_ITL_SET_LAWS_HPP_JOFA_071124


