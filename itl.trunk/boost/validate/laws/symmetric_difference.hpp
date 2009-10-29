/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SYMMETRIC_DIFFERENCE_HPP_JOFA_071124
#define BOOST_ITL_SYMMETRIC_DIFFERENCE_HPP_JOFA_071124

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/itl/functors.hpp>

namespace boost{namespace itl
{

    // ---------------------------------------------------------------------------
    template <typename Type,
              template<class>class Addition     = itl::inplace_plus,
              template<class>class Subtraction  = itl::inplace_minus,
              template<class>class Intersection = itl::inplace_et,
              template<class>class Equality     = itl::std_equal>
    class InplaceSymmetricDifference 
        : public Law<InplaceSymmetricDifference<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** (a + b) - (a & b) == (a - b) + (b - a)
        computed using inplace operators +=, -= and &=
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "Inplace Symmetric Difference"; }
        std::string formula()const { return "(a+b) - (a&b) == (a-b) + (b-a) 'inplace'"; }

        std::string typeString()const
        {
            return "SymmetricDifference<"+type_to_string<Type>::apply()+","
                                         +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds() 
        {
            // --- left hand side ------------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Addition<Type>()(a_plus_b, this->template getInputValue<operand_b>());

            Type a_sec_b = this->template getInputValue<operand_a>();
            Intersection<Type>()(a_sec_b, this->template getInputValue<operand_b>());

            Type lhs = a_plus_b;
            Subtraction<Type>()(lhs, a_sec_b);

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_b, this->template getInputValue<operand_b>());

            Type b_minus_a = this->template getInputValue<operand_b>();
            Subtraction<Type>()(b_minus_a, this->template getInputValue<operand_a>());

            Type rhs = a_minus_b;
            Addition<Type>()(rhs, b_minus_a);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }


        bool debug_holds()
        {
            // --- left hand side ------------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            Addition<Type>()(a_plus_b, this->template getInputValue<operand_b>());

            std::cout << "a_plus_b=" << a_plus_b.as_string() << std::endl;

            Type a_sec_b = this->template getInputValue<operand_a>();
            Intersection<Type>()(a_sec_b, this->template getInputValue<operand_b>());

            std::cout << "a_sec_b=" << a_sec_b.as_string() << std::endl;

            Type lhs = a_plus_b;
            Subtraction<Type>()(lhs, a_sec_b);

            std::cout << "lhs=" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            Subtraction<Type>()(a_minus_b, this->template getInputValue<operand_b>());
            std::cout << "a_minus_b=" << a_minus_b.as_string() << std::endl;

            Type b_minus_a = this->template getInputValue<operand_b>();
            Subtraction<Type>()(b_minus_a, this->template getInputValue<operand_a>());
            std::cout << "b_minus_a=" << b_minus_a.as_string() << std::endl;

            Type rhs = a_minus_b;
            Addition<Type>()(rhs, b_minus_a);
            std::cout << "rhs=" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Equality = itl::std_equal>
    class InplaceFlip 
        : public Law<InplaceFlip<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        /** a ^ b == (a - b) + (b - a)
        computed using inplace operators +=, -= and &=
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "Inplace Flip"; }
        std::string formula()const { return "a ^ b == (a-b) + (b-a) 'inplace'"; }

        std::string typeString()const
        {
            return "Flip<"+type_to_string<Type>::apply()+","
                          +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds() 
        {
            // --- left hand side ------------------------
            Type lhs = this->template getInputValue<operand_a>();
            lhs ^= this->template getInputValue<operand_b>();

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            a_minus_b -= this->template getInputValue<operand_b>();

            Type b_minus_a = this->template getInputValue<operand_b>();
            b_minus_a -= this->template getInputValue<operand_a>();

            Type rhs = a_minus_b;
            rhs += b_minus_a;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }


        bool debug_holds()
        {
            // --- left hand side ------------------------
            Type lhs = this->template getInputValue<operand_a>();
            lhs ^= this->template getInputValue<operand_b>();
            std::cout << "lhs=" << lhs.as_string() << std::endl;

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            a_minus_b -= this->template getInputValue<operand_b>();
            std::cout << "a_minus_b=" << a_minus_b.as_string() << std::endl;

            Type b_minus_a = this->template getInputValue<operand_b>();
            b_minus_a -= this->template getInputValue<operand_a>();
            std::cout << "b_minus_a=" << b_minus_a.as_string() << std::endl;

            Type rhs = a_minus_b;
            rhs += b_minus_a;
            std::cout << "rhs=" << rhs.as_string() << std::endl;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return Equality<Type>()(lhs, rhs);
        }

        size_t size()const 
        { 
            return 
                value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };

}} // namespace itl boost

#endif // BOOST_ITL_SYMMETRIC_DIFFERENCE_HPP_JOFA_071124


