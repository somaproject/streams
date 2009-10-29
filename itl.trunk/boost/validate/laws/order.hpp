/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_ORDER_HPP_JOFA_071129
#define BOOST_ITL_ORDER_HPP_JOFA_071129

#include <boost/itl/type_traits/value_size.hpp>
#include <boost/validate/laws/law.hpp>

namespace boost{namespace itl
{

    /*  Orders (Orderings): .<. : M x M -> bool */


    template <typename Type, template<class>class Relation>
    class Reflexivity 
        : public Law<Reflexivity<Type,Relation>, 
                     LOKI_TYPELIST_1(Type), Loki::NullType> 
    {
    public:
        std::string name()const { return "Reflexivity"; }
        std::string formula()const { return "a <= a"; }

        std::string typeString()const
        {
            return "Reflexivity<"+type_to_string<Type>::apply()+","
                                 +unary_template_to_string<Relation>::apply()+">";
        }

    public:
        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            return Relation<Type>()(a,a);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { return value_size<Type>::apply(this->template getInputValue<operand_a>());    }
    };

    template<> 
    std::string unary_template_to_string<std::less_equal>::apply()  { return "<="; }
    template<> 
    std::string unary_template_to_string<std::less>::apply()        { return "<"; }

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Relation>
    class Irreflexivity 
        : public Law<Irreflexivity<Type,Relation>, 
                     LOKI_TYPELIST_1(Type), Loki::NullType> 
    {
    public:
        std::string name()const { return "Irreflexivity"; }
        std::string formula()const { return "!(a < a)"; }

        std::string typeString()const
        {
            return "Irreflexivity<"+type_to_string<Type>::apply()+","
                                   +unary_template_to_string<Relation>::apply()+">";
        }

    public:
        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            return !Relation<Type>()(a,a);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { return value_size<Type>::apply(this->template getInputValue<operand_a>());    }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, 
              template<class>class Relation,
              template<class>class Equality = itl::std_equal>
    class Antisymmetry 
        : public Law<Antisymmetry<Type,Relation,Equality>, 
                     LOKI_TYPELIST_2(Type,Type), Loki::NullType> 
    {
        /** a <= b && b <= a  =>  a == b 
        Input  = (a := inVal1, b := inVal2)
        Output = ()
        */
    public:
        std::string name()const { return "Antisymmetry"; }
        std::string formula()const { return "(a <= b && b <= a)  =>  a == b"; }

        std::string typeString()const
        {
            return "Antisymmetry<"+type_to_string<Type>::apply()+"," 
                                  +unary_template_to_string<Relation>::apply()+","
                                  +unary_template_to_string<Equality>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_b>();

            return !(Relation<Type>()(a,b) && Relation<Type>()(b,a)) || Equality<Type>()(a,b);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                   value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };

    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Relation>
    class Asymmetry 
        : public Law<Asymmetry<Type,Relation>, 
                     LOKI_TYPELIST_2(Type,Type), LOKI_TYPELIST_1(Type)> 
    {
        /** a < b  => !(b < a) 
        Input  = (a := inVal1, b := inVal2)
        Output = ()
        */
    public:
        std::string name()const { return "Asymmetry"; }
        std::string formula()const { return " a < b  => !(b < a)"; }

        std::string typeString()const
        {
            return "Asymmetry<"+type_to_string<Type>::apply()+","
                               +unary_template_to_string<Relation>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_a>();

            return !(Relation<Type>()(a,b) && Relation<Type>()(b,a));
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>());
        }
    };


    // ---------------------------------------------------------------------------
    template <typename Type, template<class>class Relation>
    class Transitivity 
        : public Law<Transitivity<Type,Relation>, 
                     LOKI_TYPELIST_3(Type,Type,Type), Loki::NullType> 
    {
        /** a < b && b < c  =>  a < c 
        Input  = (a := inVal1, b := inVal2, c := inVal3)
        Output = ()
        */
    public:
        std::string name()const { return "Transitivity"; }
        std::string formula()const { return "a < b && b < c  =>  a < c"; }

        std::string typeString()const
        {
            return "Transitivity<"+type_to_string<Type>::apply()+","
                                  +unary_template_to_string<Relation>::apply()+">";
        }

    public:

        bool holds()
        {
            Type a = this->template getInputValue<operand_a>();
            Type b = this->template getInputValue<operand_a>();
            Type c = this->template getInputValue<operand_c>();

            return !(Relation<Type>()(a,b) && Relation<Type>()(b,c)) || Relation<Type>()(a,c);
        }

        bool debug_holds(){ return holds(); }

        size_t size()const 
        { 
            return value_size<Type>::apply(this->template getInputValue<operand_a>())+
                value_size<Type>::apply(this->template getInputValue<operand_b>())+
                value_size<Type>::apply(this->template getInputValue<operand_c>());
        }
    };

}} // namespace itl boost

#endif // BOOST_ITL_ORDER_HPP_JOFA_071129

