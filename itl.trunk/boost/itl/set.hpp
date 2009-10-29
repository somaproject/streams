/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SET_HPP_JOFA_070519
#define BOOST_ITL_SET_HPP_JOFA_070519

#include <string>
#include <set>
#include <boost/itl/detail/concept_check.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/is_set.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/absorbs_neutrons.hpp>
#include <boost/itl/type_traits/is_total.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/detail/subset_comparer.hpp>
#include <boost/itl/set_algo.hpp>
#include <boost/itl/predicates.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/type_traits/is_same.hpp>

namespace boost{namespace itl
{

/** adds addition, subtraction and intersection to std::set */
template 
<
    typename DomainT, 
    ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_ALLOC   Alloc   = std::allocator 
>
class set: private std::set<DomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> >
{
public:
    typedef typename itl::set<DomainT, Compare,       Alloc >       type;
    typedef typename std::set<DomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> > base_type;

public:
    typedef DomainT     domain_type;
    typedef DomainT     codomain_type;
    typedef DomainT     element_type;
    typedef DomainT     key_type;
    typedef DomainT     value_type;
    typedef DomainT     data_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) key_compare;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) value_compare;
    typedef Alloc<DomainT>   allocator_type;

public:        
    typedef typename base_type::pointer                pointer;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    enum { fineness = 4 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    set()
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    explicit set(const domain_compare& comp): 
    std::set<DomainT, domain_compare, Alloc<DomainT> >(comp){}

    template <class InputIterator>
    set(InputIterator first, InputIterator past): 
    std::set<InputIterator>(first,past){}

    template <class InputIterator>
    set(InputIterator first, InputIterator past, const key_compare& comp): 
    std::set<InputIterator>(first, past, comp){}

    set(const set& src): base_type::set(src)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    explicit set(const element_type& key): base_type::set()
    { 
        insert(key); 
    }

    set& operator=(const set& src) { base_type::operator=(src); return *this; } 
    void swap(set& src) { base_type::swap(src); }

    /** Copy the elements in set \c src to which property \c hasProperty applies 
        into \c *this set. */
    template<class Predicate>
    set& assign_if(const set& src, const Predicate&);

    //==========================================================================
    using base_type::empty;
    using base_type::clear;

    using base_type::begin;
    using base_type::end;
    using base_type::rbegin;
    using base_type::rend;

    using base_type::size;
    using base_type::max_size;

    using base_type::key_comp;
    using base_type::value_comp;

    using base_type::insert;
    using base_type::erase;
    using base_type::find;
    using base_type::count;

    using base_type::lower_bound;
    using base_type::upper_bound;
    using base_type::equal_range;

public:
    //==========================================================================
    //= Containedness
    //==========================================================================

    /// Checks if the element \c value is in the set
    bool contains(const element_type& value)const 
    { return !(find(value) == end()); }

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool contained_in(const set& super)const 
    { return Set::contained_in(*this, super); }

    /** Does <tt>*this</tt> contain <tt>sub</tt>? */
    bool contains(const set& sub)const { return Set::contained_in(sub, *this); }

    /** <tt>*this</tt> and <tt>x2</tt> are disjoint, if their intersection is empty */
    bool is_disjoint(const set& x2)const { return Set::is_disjoint(*this, x2); }

    //==========================================================================
    //= Size
    //==========================================================================
    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    size_t iterative_size()const { return size(); }

    size_t cardinality()const { return size(); }

    //==========================================================================
    //= Addition, subtraction
    //==========================================================================
    /** Add an \c element to the set. */
    set& add(const element_type& element) { insert(element); return *this; } 

    /** Add an element \c element after \c prior to the set. */
    iterator add(iterator prior, const element_type& element) 
    { return insert(prior, element); } 

    /** Subtract an \c element from the set. */
    set& subtract(const element_type& element);

    //==========================================================================
    //= Insertion, erasure
    //==========================================================================
    /** Erase the elements in *this set to which property \c hasProperty applies. 
        Keep all the rest. */
    template<class Predicate>
    set& erase_if(const Predicate&);

    //==========================================================================
    //= Intersection, symmetric difference
    //==========================================================================

    /** Add \c element to \c section, if \c element is in \c *this set */
    void add_intersection(set& section, const element_type& element)const;

    /** The intersection of set \c sectant with \c *this set is added 
        to \c section. */
    void add_intersection(set& section, const set& sectant)const;

    /** Returns true, if there is an intersection of \c element and \c *this set.
        Functions \c intersects and \c contains are identical on arguments
        of type \c element_type. Complexity: Logarithmic in container size. */
    bool intersects(const element_type& element)const { return contains(element); }

    /** If \c *this set contains \c element it is erased, otherwise it is added. */
    set& flip(const element_type& element);

    //==========================================================================
    //= Representation
    //==========================================================================

    /** Represent this set as a string */
    std::string as_string(const char* sep = " ")const;

    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    /** \c key_value allows for a uniform access to \c key_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const key_type& key_value(IteratorT value_){ return (*value_); }

    /** \c data_value allows for a uniform access to \c data_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const data_type& data_value(IteratorT value_){ return (*value_); }

    /** \c key_less allows for a uniform notation of key comparison which
        is used for common algorithms on sets and maps. */
    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return key_compare()(*lhs_, *rhs_); }

    /** \c make_element allows for a uniform notation of key comparison which
        is used for common algorithms on sets and maps. */
    static value_type make_element(const key_type& key_val, const data_type& data_val)
    { return key_val; }

};



template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
set<DomainT,Compare,Alloc>&
    set<DomainT,Compare,Alloc>::subtract(const element_type& val)
{
    iterator it_ = find(val);
    if(it_ != end())
        erase(it_);

    return *this;
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
void set<DomainT,Compare,Alloc>
    ::add_intersection(set& section, const element_type&sectant)const
{
    const_iterator it_ = find(sectant);
    if(it_ != end()) 
        section.add(*it_);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
void set<DomainT,Compare,Alloc>
    ::add_intersection(set& section, const set& sectant)const
{
    const_iterator common_lwb_;
    const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sectant, *this))
        return;

    const_iterator sec_ = common_lwb_;
    while(sec_ != common_upb_)
        add_intersection(section, *sec_++);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
set<DomainT,Compare,Alloc>::flip(const element_type& operand)
{
    std::pair<iterator,bool> insertion = insert(operand);
    if(!insertion.second)
        erase(insertion.first);

    return *this;
} 


template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
std::string set<DomainT,Compare,Alloc>::as_string(const char* sep)const
{ 
    const_iterator it_ = begin();
    
    if(it_ == end()) return std::string();
    else
    {
        std::string y = to_string<DomainT>::apply(*it_++);
        while(it_ != end()) { y += sep; y += to_string<DomainT>::apply(*it_++); }
        return y;
    }
}


template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    template <class Predicate>
set<DomainT,Compare,Alloc>& set<DomainT,Compare,Alloc>
    ::erase_if(const Predicate& pred)
{
    iterator it = begin();
    while(it != end())
        if(pred(*it))
            erase(it++); 
        else ++it;
    return *this;

}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    template <class Predicate>
set<DomainT,Compare,Alloc>& set<DomainT,Compare,Alloc>
    ::assign_if(const set<DomainT,Compare,Alloc>& src, const Predicate& pred)
{
    clear();
    const_iterator it = src.begin();
    while(it != src.end()) {
        if(pred(*it)) 
            add(*it++);
    }
    return *this;
}

//-----------------------------------------------------------------------------
// non member functions
//-----------------------------------------------------------------------------
//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Standard equality, which is lexicographical equality of the sets
    as sequences, that are given by their Compare order. */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator == (const itl::set<DomainT,Compare,Alloc>& lhs,
                         const itl::set<DomainT,Compare,Alloc>& rhs)
{
    typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator != (const itl::set<DomainT,Compare,Alloc>& lhs,
                         const itl::set<DomainT,Compare,Alloc>& rhs)
{ return !(lhs == rhs); }

/** Element equality. Two sets are equal if they contain the same elements */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool is_element_equal(const itl::set<DomainT,Compare,Alloc>& lhs,
                             const itl::set<DomainT,Compare,Alloc>& rhs)
{
    typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

/** Strict weak less ordering which is given by the Compare order */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator < (const itl::set<DomainT,Compare,Alloc>& lhs,
                        const itl::set<DomainT,Compare,Alloc>& rhs)
{
    typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator<((const base_type&)lhs, (const base_type&)rhs);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator > (const itl::set<DomainT,Compare,Alloc>& lhs,
                        const itl::set<DomainT,Compare,Alloc>& rhs)
{ return rhs < lhs; }

/** Partial ordering which is induced by Compare */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator <= (const itl::set<DomainT,Compare,Alloc>& lhs,
                         const itl::set<DomainT,Compare,Alloc>& rhs)
{ return !(lhs > rhs); }

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline bool operator >= (const itl::set<DomainT,Compare,Alloc>& lhs,
                         const itl::set<DomainT,Compare,Alloc>& rhs)
{ return !(lhs < rhs); }

//--------------------------------------------------------------------------

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator += (      itl::set<DomainT,Compare,Alloc>& object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{ return object.add(operand); } 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator +  (      itl::set<DomainT,Compare,Alloc>                object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    return object += operand; 
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator +  (const typename itl::set<DomainT,Compare,Alloc>::element_type& operand,
                            itl::set<DomainT,Compare,Alloc>                object)
{
    return object += operand; 
}

/// Add a set \c operand to set \c object.
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator += (      itl::set<DomainT,Compare,Alloc>& object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{ Set::add(object, operand); return object; }

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
itl::set<DomainT,Compare,Alloc> 
operator +  (      itl::set<DomainT,Compare,Alloc>  object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{
    return object += operand; 
}

//--------------------------------------------------------------------------

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator |= (      itl::set<DomainT,Compare,Alloc>& object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{ return object.add(operand); } 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
itl::set<DomainT,Compare,Alloc> 
operator |  (      itl::set<DomainT,Compare,Alloc>                object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    return object |= operand; 
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator |  (const typename itl::set<DomainT,Compare,Alloc>::element_type& operand,
                            itl::set<DomainT,Compare,Alloc>                object)
{
    return object |= operand; 
}

/// Add a set \c operand to this set \object.
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator |= (      itl::set<DomainT,Compare,Alloc>& object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{ Set::add(object, operand); return object; }

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator |  (      itl::set<DomainT,Compare,Alloc>  object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{
    return object |= operand; 
}

//--------------------------------------------------------------------------

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator -= (      itl::set<DomainT,Compare,Alloc>& object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{ return object.subtract(operand); } 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator -  (      itl::set<DomainT,Compare,Alloc>                object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    return object -= operand; 
}

/// Subtract a set \c x2 from this set.
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator -= (      itl::set<DomainT,Compare,Alloc>& object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{ Set::subtract(object, operand); return object; }

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator -  (      itl::set<DomainT,Compare,Alloc>  object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{
    return object -= operand; 
}

//--------------------------------------------------------------------------
// itl::set::intersection operators &=, &
//--------------------------------------------------------------------------
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator &= (      itl::set<DomainT,Compare,Alloc>& object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    itl::set<DomainT,Compare,Alloc> section;
    object.add_intersection(section, operand);
    object.swap(section);
    return object; 
} 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator &  (      itl::set<DomainT,Compare,Alloc>                object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    return object &= operand; 
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator &  (const typename itl::set<DomainT,Compare,Alloc>::element_type& operand,
                            itl::set<DomainT,Compare,Alloc>                object)
{
    return object &= operand; 
}



/** Intersect set \c object with \c operand. 
    So \c object becomes the intersection of \c object and \c operand */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator &= (      itl::set<DomainT,Compare,Alloc>& object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{
    itl::set<DomainT,Compare,Alloc> section;
    object.add_intersection(section, operand);
    object.swap(section);
    return object; 
} 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator &  (      itl::set<DomainT,Compare,Alloc>  object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{
    return object &= operand; 
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(const itl::set<DomainT,Compare,Alloc>&               object, 
       const typename itl::set<DomainT,Compare,Alloc>::element_type& element)
{
    return object.intersects(element);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(
      const typename itl::set<DomainT,Compare,Alloc>::element_type& element,
               const itl::set<DomainT,Compare,Alloc>&               object)
{
    return object.intersects(element);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
bool intersects(const itl::set<DomainT,Compare,Alloc>& left, 
                const itl::set<DomainT,Compare,Alloc>& right)
{
    if(left.iterative_size() < right.iterative_size())
        return Set::intersects(right, left);
    else
        return Set::intersects(left, right);
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc,
          class LeftT, class RightT>
enable_if<mpl::or_<is_same< typename itl::set<DomainT,Compare,Alloc>, LeftT >,
                   is_same< typename itl::set<DomainT,Compare,Alloc>, RightT>
                  >, bool>
is_disjoint(const LeftT& left, const RightT& right)
{
    return !intersects(left,right);
}



//--------------------------------------------------------------------------
// itl::set::symmetric_difference operators ^=, ^
//--------------------------------------------------------------------------
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator ^= (      itl::set<DomainT,Compare,Alloc>& object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{
    return object.flip(operand);
} 

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator ^  (      itl::set<DomainT,Compare,Alloc>                object,
    const typename itl::set<DomainT,Compare,Alloc>::element_type& operand)
{ 
    return object ^= operand; 
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc> 
operator ^  (const typename itl::set<DomainT,Compare,Alloc>::element_type& operand,
                            itl::set<DomainT,Compare,Alloc>                object)
{
    return object ^= operand; 
}


/** Intersect set \c object with \c operand. 
    So \c object becomes the intersection of \c object and \c operand */
template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
inline itl::set<DomainT,Compare,Alloc>& 
operator ^= (      itl::set<DomainT,Compare,Alloc>& object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{ 
    Set::flip(object, operand); return object; 
}

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
itl::set<DomainT,Compare,Alloc> 
operator ^  (      itl::set<DomainT,Compare,Alloc>  object,
             const itl::set<DomainT,Compare,Alloc>& operand)
{ 
    return object ^= operand; 
}


template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
set<DomainT,Compare,Alloc>& 
    insert(      set<DomainT,Compare,Alloc>& object, 
           const set<DomainT,Compare,Alloc>& operand) 
{ return object += operand; }

template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
set<DomainT,Compare,Alloc>& 
    erase(      set<DomainT,Compare,Alloc>& object, 
          const set<DomainT,Compare,Alloc>& operand) 
{ return object -= operand; }


//==============================================================================
//= Streaming
//==============================================================================
template<class CharType, class CharTraits, 
    class DomainT, ITL_COMPARE Compare,    ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
  const itl::set<DomainT,Compare,Alloc>& object)
{
    typedef itl::set<DomainT,Compare,Alloc> ObjectT;
    stream << "{";
    typename ObjectT::const_iterator it = object.begin();
    if(it != object.end())
        stream << *it++;
    while(it != object.end())
        stream << " " << *it++;

    return stream << "}";
}



//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class Type>
struct is_set<itl::set<Type> >
{ 
    typedef is_set<itl::set<Type> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class Type>
struct is_interval_container<itl::set<Type> >
{ 
    typedef is_interval_container<itl::set<Type> > type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class Type>
struct is_interval_splitter<itl::set<Type> >
{ 
    typedef is_interval_splitter type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class Type>
struct absorbs_neutrons<itl::set<Type> >
{ 
    typedef absorbs_neutrons type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class Type>
struct is_total<itl::set<Type> >
{ 
    typedef is_total type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class Type>
struct type_to_string<itl::set<Type> >
{
    static std::string apply()
    { return "set<"+ type_to_string<Type>::apply() +">"; }
};


}} // namespace itl boost

#endif // BOOST_ITL_SET_HPP_JOFA_070519

