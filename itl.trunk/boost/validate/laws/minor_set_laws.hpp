/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_MINOR_SET_LAWS_HPP_JOFA_090821
#define BOOST_ITL_MINOR_SET_LAWS_HPP_JOFA_090821

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{

    template <typename Type>
    class IntersectsDefined 
        : public Law<IntersectsDefined<Type>, LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(bool,bool)> 
    {
        /** is_total<T> || (intersects(a, b) == !(a & b).empty()) : Definition of intersects predicate 
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "IntersectsDefined defined"; }
        std::string formula()const { return "is_total<T> || intersects(a, b) == !(a & b).empty()"; }

        std::string typeString()const
        {
            return "IntersectsDefined<"+type_to_string<Type>::apply()+">";
        }

    public:

        bool holds()
        {
            // is_total<T> || intersects(a, b) == !(a & b).empty()
            // --- left hand side ------------------------
            bool a_intersects_b 
                = intersects(this->template getInputValue<operand_a>(),
                             this->template getInputValue<operand_b>());
            // --- right hand side ------------------------
            Type a_sec_b = this->template getInputValue<operand_a>();
            a_sec_b &= this->template getInputValue<operand_b>();

            bool a_sec_b_non_empty = !a_sec_b.empty();

            this->template setOutputValue<lhs_result>(a_intersects_b);
            this->template setOutputValue<rhs_result>(a_sec_b_non_empty);

            if(is_total<Type>::value)
                // For a total map y, y.empty() does not mean that y is empty
                // it means that y is a null vector. In this sense total maps
                // always intersect with themselves and with key sets.
                return a_intersects_b == true;
            else
                return a_intersects_b == a_sec_b_non_empty;
        }

        bool debug_holds()
        { 
            // intersects(a, b) == !(a & b).empty() : Definition of intersects predicate 
            // --- left hand side ------------------------
            Type value_a = this->template getInputValue<operand_a>();
            Type value_b = this->template getInputValue<operand_b>();
            std::cout << "a = " << value_a << std::endl;
            std::cout << "b = " << value_b << std::endl;
            std::cout << "a&b = " << (value_a & value_b) << std::endl;

            bool a_intersects_b 
                = intersects(this->template getInputValue<operand_a>(),
                             this->template getInputValue<operand_b>());
            // --- right hand side ------------------------
            Type a_sec_b = this->template getInputValue<operand_a>();
            a_sec_b &= this->template getInputValue<operand_b>();

            bool a_sec_b_non_empty = !a_sec_b.empty();

            this->template setOutputValue<lhs_result>(a_intersects_b);
            this->template setOutputValue<rhs_result>(a_sec_b_non_empty);

            if(is_total<Type>::value)
                return a_intersects_b == true;
            else
                return a_intersects_b == a_sec_b_non_empty;
        }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };

    template <typename Type, typename CoType>
    class Interinclusion 
        : public Law<Interinclusion<Type,CoType>, LOKI_TYPELIST_2(Type,CoType), LOKI_TYPELIST_1(bool)> 
    {
        /** a.contains(a & b)
        Input  = (a := inVal1, b := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "Interinclusion"; }
        std::string formula()const { return "a.contains(a & b)"; }

        std::string typeString()const
        {
            return "Interinclusion<"+  type_to_string<Type>::apply()+","
                                    +type_to_string<CoType>::apply()+">";
        }

    public:

        bool holds()
        {
            // a.contains(a & b)
            Type   value_a = this->template getInputValue<operand_a>();
            CoType value_b = this->template getInputValue<operand_b>();
            return value_a.contains(value_a & value_b);
        }

        bool debug_holds()
        { 
            // a.contains(a & b)
            Type   value_a = this->template getInputValue<operand_a>();
            CoType value_b = this->template getInputValue<operand_b>();
            Type   a_sec_b = value_a & value_b;
            bool result = value_a.contains(value_a & value_b);
            // -------------------------------------------
            std::cout << "a = " << value_a << std::endl;
            std::cout << "b = " << value_b << std::endl;
            std::cout << "a&b = " << a_sec_b << std::endl;
            std::cout << "a.contains(a&b) = " << result << std::endl;
            // -------------------------------------------
            value_a.contains(a_sec_b);

            return result;
        }

        size_t size()const 
        { 
            return value_size<  Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<CoType>::apply(this->template getInputValue<operand_b>());
        }
    };


    template <typename Type, typename CoType>
    class AddendInclusion 
        : public Law<AddendInclusion<Type,CoType>, LOKI_TYPELIST_2(Type,CoType), LOKI_TYPELIST_1(bool)> 
    {
        /** (a + i).contains(i)
        Input  = (a := inVal1, i := inVal2)
        Output = (lhs_result, rhs_result)
        */
    public:
        std::string name()const { return "AddendInclusion"; }
        std::string formula()const { return "(a + i).contains(i)"; }

        std::string typeString()const
        {
            return "AddendInclusion<"+  type_to_string<Type>::apply()+","
                                    +type_to_string<CoType>::apply()+">";
        }

    public:

        bool holds()
        {
            // (a + i).contains(i)
            Type   value_a = this->template getInputValue<operand_a>();
            CoType value_i = this->template getInputValue<operand_b>();
            return (value_a + value_i).contains(value_i);
        }

        bool debug_holds()
        {
            return holds();
            /*
            // a.contains(a & b)
            Type   value_a = this->template getInputValue<operand_a>();
            CoType value_b = this->template getInputValue<operand_b>();
            Type   a_sec_b = value_a & value_b;
            bool result = value_a.contains(value_a & value_b);
            // -------------------------------------------
            std::cout << "a = " << value_a << std::endl;
            std::cout << "b = " << value_b << std::endl;
            std::cout << "a&b = " << a_sec_b << std::endl;
            std::cout << "a.contains(a&b) = " << result << std::endl;
            // -------------------------------------------
            value_a.contains(a_sec_b);

            return result;
            */
        }

        size_t size()const 
        { 
            return value_size<  Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<CoType>::apply(this->template getInputValue<operand_b>());
        }
    };

}} // namespace itl boost

#endif // BOOST_ITL_MINOR_SET_LAWS_HPP_JOFA_070411

