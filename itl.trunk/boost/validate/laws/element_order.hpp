/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_ELEMENT_ORDER_HPP_JOFA_090202
#define BOOST_ITL_ELEMENT_ORDER_HPP_JOFA_090202

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/interval_morphism.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{

    // ---------------------------------------------------------------------------
    template <typename Type>
    class ElementEqualDefined 
        : public Law<ElementEqualDefined<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(bool,bool)> 
    {
        /** is_element_equal(a, a.join()) 
        Input  = (a := inVal1)
        Output = ()
        */
    public:
        std::string name()const { return "ElementEqualDefined"; }
        std::string formula()const { return "is_element_equal(a, a.join())"; }

        std::string typeString()const
        {
            return "ElementEqualDefined<"+type_to_string<Type>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_equal(a, b);
            bool rhs = (a_atomic == b_atomic);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_equal(a, b);
            bool rhs = (a_atomic == b_atomic);

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>());
        }
    };


    // ---------------------------------------------------------------------------
    template <typename Type>
    class ElementLessDefined 
        : public Law<ElementLessDefined<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(bool,bool)> 
    {
        /** is_element_less(a, b) == a.join() < b.join() 
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs, rhs)
        */
    public:
        std::string name()const { return "ElementLessDefined"; }
        std::string formula()const { return "is_element_less(a,b) == a.atomic() < b.atomic()"; }

        std::string typeString()const
        {
            return "ElementLessDefined<"+type_to_string<Type>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_less(a, b);
            bool rhs = a_atomic < b_atomic;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_less(a, b);
            bool rhs = a_atomic < b_atomic;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }


        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };


    // ---------------------------------------------------------------------------
    template <typename Type>
    class ElementGreaterDefined 
        : public Law<ElementLessDefined<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(bool,bool)> 
    {
        /** is_element_greater(a, b) == a.join() > b.join() 
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs, rhs)
        */
    public:
        std::string name()const { return "ElementGreaterDefined"; }
        std::string formula()const { return "is_element_greater(a,b) == a.atomic() > b.atomic()"; }

        std::string typeString()const
        {
            return "ElementGreaterDefined<"+type_to_string<Type>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_greater(a, b);
            bool rhs = a_atomic > b_atomic;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }

        bool debug_holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();
            typename Type::atomized_type a_atomic;
            typename Type::atomized_type b_atomic;
            Interval::atomize(a_atomic, a);
            Interval::atomize(b_atomic, b);

            bool lhs = is_element_greater(a, b);
            bool rhs = a_atomic > b_atomic;

            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return lhs == rhs;
        }


        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };


}} // namespace itl boost

#endif // BOOST_ITL_ELEMENT_ORDER_HPP_JOFA_090202

