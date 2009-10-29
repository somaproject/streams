/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_PREDICATES_HPP_JOFA_990224
#define BOOST_ITL_PREDICATES_HPP_JOFA_990224

#include <functional>
#include <boost/itl/type_traits/type_to_string.hpp>

namespace boost{namespace itl
{
    // naming convention
    // predicate: n-ary predicate
    // property:  unary predicate
    // relation:  binary predicate

    // Unary predicates

    template <class Type>
    class property : public std::unary_function<Type,bool>{};

    template <class Type>
    class member_property : public property<Type>
    {
    public:
        member_property( bool(Type::* pred)()const ): property<Type>(), m_pred(pred){}
        bool operator()(const Type& x)const { return (x.*m_pred)(); }
    private:
        bool(Type::* m_pred)()const;
    } ;

    template <class Type>
    class empty: public property<Type>
    {
    public:
        bool operator() (const Type& x)const { return x.empty(); }
    } ;

    template <class Type>
    struct is_neutron: public property<Type>
    {
        bool operator() (const Type& x)const { return x == Type(); }
    } ;

    template <class Type>
    class content_is_neutron: public property<Type>
    {
    public:
        bool operator() (const Type& x)const 
        { return x.second == Type::second_type(); }
    } ;


    // Binary predicates: relations

    template <class LeftT, class RightT>
    class relation : public std::binary_function<LeftT,RightT,bool>{};

    template <class Type> struct std_equal : public relation<Type,Type>
    {
        bool operator()(const Type& lhs, const Type& rhs)const
        {
            return lhs == rhs;
        }
    };

    //-----------------------------------------------------------------------------

    template<>         
    inline std::string unary_template_to_string<itl::std_equal>::apply()
    { return "=="; }

    template <class Type> 
    struct element_equal : public relation<Type,Type>
    {
        bool operator()(const Type& lhs, const Type& rhs)const
        {
            return is_element_equal(lhs, rhs);
        }
    };

    template<>
    inline std::string unary_template_to_string<itl::element_equal>::apply()  
    { return "="; }

    template <class Type> 
    struct protonic_equal : public relation<Type,Type>
    {
        bool operator()(const Type& lhs, const Type& rhs)const
        {
            return is_protonic_equal(lhs, rhs);
        }
    };

    template<>
    inline std::string unary_template_to_string<itl::protonic_equal>::apply()  
    { return "==/0"; }



    /// Functor class template contained_in implements the subset relation.
    template<class Type> 
    struct sub_super_set : public relation<Type,Type>
    {
        /// Apply the subset relation.
        /** <tt>contained_in(sub, super)</tt> is true if <tt>sub</tt> 
            is contained in <tt>super</tt> */
        bool operator()(const Type& sub, const Type& super)const
        {
            return sub.contained_in(super);
        }
    };

    template<>
    inline std::string unary_template_to_string<itl::sub_super_set>::apply()  
    { return "C="; }

    /// Functor class template <b>contains</b> implements the superset relation. 
    template<class Type> 
    struct super_sub_set : public relation<Type,Type>
    {
        /// Apply the superset relation.
        /** <tt>contains(super, sub)</tt> is true if <tt>super</tt> containes 
            <tt>sub</tt> */
        bool operator()(const Type& super, const Type& sub)const
        {
            return super.contains(sub);
        }
    };

    template<>
    inline std::string unary_template_to_string<itl::super_sub_set>::apply()  
    { return "D="; }

}} // namespace itl boost

#endif

