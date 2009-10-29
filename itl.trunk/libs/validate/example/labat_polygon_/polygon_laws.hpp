/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_POLYGON_LAWS_HPP_JOFA_090901
#define BOOST_ITL_POLYGON_LAWS_HPP_JOFA_090901

#define LAW_BASED_TEST_BOOST_POLYGON

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>
#include <boost/polygon/polygon.hpp>
#include "custom_polygon.hpp"

namespace boost{namespace itl
{

    template <typename Type>
    class PolygonCommutativity 
        : public Law<PolygonCommutativity<Type>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        // a o b == b o a computed as
        //lsum=a; lsum+=b; rsum=b; rsum+=a => lsum==rsum 
        //Input  = (a := inVal1, b := inVal2)
        //Output = (lhs_result, lhs_result)
        
    public:
        std::string name()const { return "Polygon Commutativity"; }
        std::string formula()const { return "a o b == b o a 'inplace'"; }

        std::string typeString()const
        {
            return "Commutativity<"+type_to_string<Type>::apply()+">";
        }

    public:

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }

        bool holds()
        {
            using namespace boost::polygon;
            using namespace boost::polygon::operators;

            Type value_a = this->template getInputValue<operand_a>();
            Type value_b = this->template getInputValue<operand_b>();

            Type left  = value_a;
            boost::polygon::operators::operator+=(left, value_b);
            Type right = value_b;
            boost::polygon::operators::operator+=(right, value_a);

            this->template setOutputValue<lhs_result>(left);
            this->template setOutputValue<rhs_result>(right);

            return equivalence(left, right);
        }

        bool debug_holds()
        {
            // If law violations are found, this function is called
            // for the smallest violated law instance.
            // You may replace the call of 
            // holds();
            // by your own code that is providing additional debugging
            // information.
            return holds();
        }

    };


    template <typename Type> //Type can be more than one parameter for mixed laws with different types
    class PolygonSymmetricDifference 
        : public Law<PolygonSymmetricDifference<Type>, 
                     // Input type list          Result type list
                     // Types of the variables   Tpyes of the righthand and the lefthand side.
                     // in a law                 Can be more, if you want to see interim results.
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_2(Type,Type)>
    {
        // (a+b) - (a&b) == (a-b) + (b-a)
        //Input  = (a := inVal1, b := inVal2)
        //Output = (sum_lhs, sum_rhs)
        
    public:
        // These are descriptive informations to create readable output.
        std::string name()const { return "Polygon Symmetric Difference"; }
        std::string formula()const { return "(a+b) - (a&b) == (a-b) + (b-a)"; }

        std::string typeString()const
        {
            return "SymmetricDifference<"+type_to_string<Type>::apply()+">";
        }

    public:

        // Define a size measure for the laws input variables. According to
        // this size the smallest law instances are collected if violations occur.
        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }

        // This function has to be coded to test the validity of a law instance.
        bool holds()
        {
            using namespace boost::polygon;
            using namespace boost::polygon::operators;

            // There are predifined constants operand_a, _b, ..., lhs_reult, rhs_result in the base class.
            //std::cout << this->template getInputValue<operand_a>().as_string() << std::endl;

            // --- left hand side ------------------------
            Type a_plus_b = this->template getInputValue<operand_a>();
            boost::polygon::operators::operator+=(a_plus_b, this->template getInputValue<operand_b>());

            Type a_sec_b = this->template getInputValue<operand_a>();
            boost::polygon::operators::operator&=(a_sec_b, this->template getInputValue<operand_b>());

            Type lhs = a_plus_b;
            boost::polygon::operators::operator-=(lhs, a_sec_b);

            // --- right hand side -----------------------
            Type a_minus_b = this->template getInputValue<operand_a>();
            boost::polygon::operators::operator-=(a_minus_b, this->template getInputValue<operand_b>());

            Type b_minus_a = this->template getInputValue<operand_b>();
            boost::polygon::operators::operator-=(b_minus_a, this->template getInputValue<operand_a>());

            Type rhs = a_minus_b;
            boost::polygon::operators::operator+=(rhs, b_minus_a);

            // Set the output variables of this law instance.
            this->template setOutputValue<lhs_result>(lhs);
            this->template setOutputValue<rhs_result>(rhs);

            return equivalence(lhs, rhs);
        }

        bool debug_holds()
        {
            // If law violations are found, this function is called
            // for the smallest violated law instance.
            // You may replace the call of 
            // holds();
            // by your own code that is providing additional debugging
            // information.
            return holds();
        }

    };

}} // namespace itl boost

#endif // BOOST_ITL_POLYGON_LAWS_HPP_JOFA_070411

