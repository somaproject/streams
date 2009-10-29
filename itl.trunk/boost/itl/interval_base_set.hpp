/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __interval_base_set_h_JOFA_990223__
#define __interval_base_set_h_JOFA_990223__

#include <limits>
#include <boost/itl/interval_set_algo.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/detail/notate.hpp>

#define const_FOR_IMPL(iter) for(typename ImplSetT::const_iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)
#define FOR_IMPL(iter) for(typename ImplSetT::iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)


namespace boost{namespace itl
{

/** Implements a set as a set of intervals (base class) */
template 
<
    typename             SubType,
    typename             DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class, ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
class interval_base_set
{
public:
    //==========================================================================
    //= Associated types
    //==========================================================================
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> type;

    /// The designated \e derived or \e sub_type of this base class
    typedef SubType sub_type;

    /// Auxilliary type for overloadresolution
    typedef type overloadable_type;

    //--------------------------------------------------------------------------
    //- Associated types: Related types
    //--------------------------------------------------------------------------
    /// The atomized type representing the corresponding container of elements
    typedef typename itl::set<DomainT,Compare,Alloc> atomized_type;

    //--------------------------------------------------------------------------
    //- Associated types: Data
    //--------------------------------------------------------------------------
    /// The domain type of the set
    typedef DomainT   domain_type;
    /// The codomaintype is the same as domain_type
    typedef DomainT   codomain_type;

    /// The element type of the set
    typedef DomainT   element_type;

    /// The interval type of the set
    typedef Interval<DomainT,Compare> interval_type;
    /// The segment type of the set
    typedef interval_type   segment_type;

    //--------------------------------------------------------------------------
    //- Associated types: Size
    //--------------------------------------------------------------------------
    /// The difference type of an interval which is sometimes different form the data_type
    typedef typename interval_type::difference_type difference_type;

    /// The size type of an interval which is mostly std::size_t
    typedef typename interval_type::size_type size_type;


    //--------------------------------------------------------------------------
    //- Associated types: Order
    //--------------------------------------------------------------------------
    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

    //--------------------------------------------------------------------------
    //- Associated types: Implementation and stl related
    //--------------------------------------------------------------------------
    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// Container type for the implementation 
    typedef typename std::set<interval_type,key_compare,allocator_type> ImplSetT;

    /// key type of the implementing container
    typedef typename ImplSetT::key_type   key_type;
    /// data type of the implementing container
    typedef typename ImplSetT::key_type   data_type;
    /// value type of the implementing container
    typedef typename ImplSetT::value_type value_type;

    /// iterator for iteration over intervals
    typedef typename ImplSetT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_iterator const_iterator;
    /// iterator for reverse iteration over intervals
    typedef typename ImplSetT::reverse_iterator reverse_iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_reverse_iterator const_reverse_iterator;

    typedef typename ImplSetT::pointer                 pointer;
    typedef typename ImplSetT::const_pointer           const_pointer;
    typedef typename ImplSetT::reference               reference;
    typedef typename ImplSetT::const_reference         const_reference;

    enum { fineness = 4 }; //SubType::fineness };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor for the empty object */
    interval_base_set(){}

    /** Copy constructor */
    interval_base_set(const interval_base_set& src): _set(src._set){}

    /** Assignment operator */
    interval_base_set& operator = (const interval_base_set& src) 
    { 
        this->_set = src._set;
        return *this; 
    }

    /** swap the content of containers */
    void swap(interval_base_set& x) { _set.swap(x._set); }

    //==========================================================================
    //= Containedness
    //==========================================================================

    /** sets the container empty */
    void clear() { _set.clear(); }
    /** is the container empty? */
    bool empty()const { return _set.empty(); }

    /** Does the container contain the element \c key ? */
    bool contains(const element_type& key)const
    { return that()->contains(interval_type(key)); }

    /** Does the container contain the interval \c sub_interval ? */
    bool contains(const segment_type& sub_interval)const
    { 
        if(sub_interval.empty()) 
            return true;

        std::pair<const_iterator, const_iterator> exterior = equal_range(sub_interval);
        if(exterior.first == exterior.second)
            return false;

        const_iterator last_overlap = prior(exterior.second);

        return 
            hull(*(exterior.first), *last_overlap).contains(sub_interval)
        &&  Interval_Set::is_joinable(*this, exterior.first, last_overlap);
    }


    /** Does the container contain the subcontainer \c sub ? */
    bool contains(const interval_base_set& sub)const 
    { return sub.contained_in(*this); }

    /** Does <tt>*this</tt> container contain <tt>sub</tt>? */
    template
    <
        template<class DomT, ITL_COMPARE Comp, 
                 template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    bool contains(const IntervalSet<DomainT,Compare,Interval,Alloc>& sub)const 
    { return sub.contained_in(*that()); }

    /** Is <tt>*this</tt> container contained in <tt>super</tt>? */
    template
    <
        template<class DomT, ITL_COMPARE Comp, 
                 template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    bool contained_in(const IntervalSet<DomainT,Compare,Interval,Alloc>& super)const
    {
        return Interval_Set::is_contained_in(*this, super);
    }

    //==========================================================================
    //= Size
    //==========================================================================

    /** Number of elements in the set (cardinality). 
        Infinite for continuous domain datatyps    */
    size_type cardinality()const;

    /** An interval set's size is it's cardinality */
    size_type size()const { return cardinality(); }

    /** The length of the interval container which is the sum of interval lengths */
    difference_type length()const;

    /** Number of intervals which is also the size of the iteration over the object */
    std::size_t interval_count()const { return _set.size(); }

    /** Size of the iteration over this container */
    std::size_t iterative_size()const { return _set.size(); }

    //==========================================================================
    //= Range
    //==========================================================================

    /** lower bound of all intervals in the object */
    DomainT lower()const 
    { return empty()? interval_type().lower() : (*(_set.begin())).lower(); }
    /** upper bound of all intervals in the object */
    DomainT upper()const 
    { return empty()? interval_type().upper() : (*(_set.rbegin())).upper(); }

    /** Smallest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval 
        bounds. */
    DomainT first()const { return (*(_set.begin())).first(); }

    /** Largest element of the set (wrt. the partial ordering on DomainT).
        last() does not exist for continuous datatypes and open interval 
        bounds. */
    DomainT last()const { return (*(_set.rbegin())).last(); }

    //==========================================================================
    //= Selection
    //==========================================================================

    /** Find the interval value pair, that contains element \c key */
    const_iterator find(const element_type& key)const
    { 
        typename ImplSetT::const_iterator it_ = this->_set.find(interval_type(key)); 
        return it_; 
    }

    //==========================================================================
    //= Addition
    //==========================================================================

    /** Add a single element \c key to the set */
    SubType& add(const element_type& key) 
    { that()->add_(interval_type(key)); return *that(); }

    /** Add an interval of elements \c inter_val to the set */
    SubType& add(const segment_type& inter_val) 
    { that()->add_(inter_val); return *that(); }

    /** Add an interval of elements \c inter_val to the set. Iterator 
        \c prior_ is a hint to the position \c inter_val can be 
        inserted after. */
    iterator add(iterator prior_, const segment_type& inter_val) 
    { return that()->add_(prior_, inter_val); }

    //==========================================================================
    //= Subtraction
    //==========================================================================

    /** Subtract a single element \c key from the set */
    SubType& subtract(const element_type& key) 
    { that()->subtract_(interval_type(key)); return *that(); }

    /** Subtract an interval of elements \c inter_val from the set */
    SubType& subtract(const segment_type& inter_val) 
    { that()->subtract_(inter_val); return *that(); }

    //==========================================================================
    //= Insertion, erasure
    //==========================================================================

    /** Insert an element \c key into the set */
    SubType& insert(const element_type& key) 
    { return add(interval_type(key)); }

    /** Insert an interval of elements \c inter_val to the set */
    SubType& insert(const segment_type& inter_val) 
    { return add(inter_val); }

    /** Insert an interval of elements \c inter_val to the set. Iterator 
        \c prior_ is a hint to the position \c inter_val can be 
        inserted after. */
    iterator insert(iterator prior_, const segment_type& inter_val) 
    { return that()->add_(prior_, inter_val); }



    /** Erase an element \c key from the set */
    SubType& erase(const element_type& key) 
    { return subtract(interval_type(key)); }

    /** Erase an interval of elements \c inter_val from the set */
    SubType& erase(const segment_type& inter_val) 
    { return subtract(inter_val); }

    /** Erase the interval that iterator \c position points to. */
    void erase(iterator position){ _set.erase(position); }

    /** Erase all intervals in the range <tt>[first,past)</tt> of iterators. */
    void erase(iterator first, iterator past){ _set.erase(first, past); }


    //==========================================================================
    //= Intersection
    //==========================================================================

    /** The intersection of \c key in \c *this set is added to \c section. 
        The function can be used as a find function. */
    void add_intersection(interval_base_set& section, const element_type& key)const
    { add_intersection(section, interval_type(key)); }

    /** The intersection of \c inter_val in \c *this set is added to \c section. */
    void add_intersection(interval_base_set& section, const segment_type& inter_val)const;


    /** The intersection of set \c sectant with \c *this set is added to \c section. */
    template
    <
        template<class DomT, ITL_COMPARE Comp, 
                 template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void add_intersection
    (
        interval_base_set& intersection,
        const IntervalSet<DomainT,Compare,Interval,Alloc>& sectant
    )const;


    /** Returns \c true, if element \c key is found in \c *this map.
        Complexity: logarithmic. */
    bool intersects(const element_type& key)const
    { return _set.find(interval_type(key)) != _set.end(); }

    /** Returns \c true, if \c inter_val intersects with \c *this map.
        Complexity: logarithmic. */
    bool intersects(const interval_type& inter_val)const
    { return _set.find(inter_val) != _set.end(); }

    //==========================================================================
    //= Symmetric difference
    //==========================================================================

    /** If \c *this set contains \c key it is erased, otherwise it is added. */
    SubType& flip(const element_type& key)
    { return flip(interval_type(key)); }

    /** If \c *this set contains \c inter_val it is erased, otherwise it is added. */
    SubType& flip(const segment_type& inter_val);

    /** The intersection of \c *this and \c operand is erased from \c *this. 
        The complemenary elements are added to \c *this. */
    template<class SubType2>
    SubType& flip(const interval_base_set<SubType2,DomainT,Compare,Interval,Alloc>& operand);

    //==========================================================================
    //= Iterator related
    //==========================================================================

    iterator begin() { return _set.begin(); }
    iterator end()   { return _set.end(); }
    const_iterator begin()const { return _set.begin(); }
    const_iterator end()const   { return _set.end(); }
    reverse_iterator rbegin() { return _set.rbegin(); }
    reverse_iterator rend()   { return _set.rend(); }
    const_reverse_iterator rbegin()const { return _set.rbegin(); }
    const_reverse_iterator rend()const   { return _set.rend(); }

    iterator lower_bound(const value_type& interval)
    { return _set.lower_bound(interval); }

    iterator upper_bound(const value_type& interval)
    { return _set.upper_bound(interval); }

    const_iterator lower_bound(const value_type& interval)const
    { return _set.lower_bound(interval); }

    const_iterator upper_bound(const value_type& interval)const
    { return _set.upper_bound(interval); }

    std::pair<iterator,iterator> equal_range(const key_type& interval)
    { return _set.equal_range(interval); }

    std::pair<const_iterator,const_iterator> equal_range(const key_type& interval)const
    { return _set.equal_range(interval); }

    //==========================================================================
    //= Morphisms
    //==========================================================================
    
    /** Join bordering intervals */
    interval_base_set& join();

    /** Set interval bounds to the type <tt>bt</tt> for intervals in the set.
        Interval bounds of different types are created by opeations on
        interval sets. This function allows to reset them uniformly without,
        of course, changing their value. This is only possible for discrete
        domain datatypes.
    */
    void uniform_bounds(itl::bound_type bounded);


    //==========================================================================
    //= Representation
    //==========================================================================
    
    /** Interval container's string representation */
    const std::string as_string()const
    { std::string res(""); const_FOR_IMPL(it_) res += (*it_).as_string(); return res; }

    
    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    template<typename IteratorT>
    static const key_type& key_value(IteratorT value_){ return (*value_); }

    template<typename IteratorT>
    static codomain_type codomain_value(IteratorT value_)
    { return (*value_).empty()? codomain_type() : (*value_).lower(); }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return key_compare()(*lhs_, *rhs_); }

    static value_type make_domain_element(const domain_type& dom_val, const codomain_type& codom_val)
    { return value_type(interval_type(dom_val)); }

protected:
    sub_type* that() { return static_cast<sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

public:
    sub_type& self() { return *that(); }

protected:
    iterator prior(iterator it_)
    { return it_ == this->_set.begin() ? this->_set.end() : --it_; }

    const_iterator prior(const_iterator it_)const
    { return it_ == this->_set.begin() ? this->_set.end() : --it_; }

    iterator gap_insert(iterator prior_, const interval_type& inter_val)
    {
        // inter_val is not conained in this map. Insertion will be successful
        BOOST_ASSERT(this->_set.find(inter_val) == this->_set.end());
        return this->_set.insert(prior_, inter_val);
    }

protected:
    ImplSetT _set;
} ;



template
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::size_type 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::cardinality()const
{
    using namespace boost::mpl;
    return if_<
                bool_<is_continuous<DomainT>::value>,
                continuous_interval_container,
                discrete_interval_container
              >
              ::type::cardinality(*this);
}

template
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename 
    interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::difference_type 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::length()const
{
    difference_type length = neutron<difference_type>::value();
    const_FOR_IMPL(it_)
        length += (*it_).length();
    return length;
}


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::add_intersection(interval_base_set& section, const segment_type& inter_val)const
{
    // any intersection with the empty intervall is empty
    if(inter_val.empty()) 
        return;

    const_iterator first_ = _set.lower_bound(inter_val);
    const_iterator end_   = _set.upper_bound(inter_val);

    iterator prior_ = section.end();
    for(const_iterator it_=first_; it_ != end_; it_++) 
    {
        interval_type common_interval = (*it_) & inter_val;
        if(!common_interval.empty())
            prior_ = section.gap_insert(prior_, common_interval);
    }
}


template<class SubType, 
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template
    <
        template<class DomT, ITL_COMPARE Comp,
                 template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::add_intersection
(
    interval_base_set& intersection,
    const IntervalSet<DomainT,Compare,Interval,Alloc>& operand
)const
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
        return;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, *this))
        return;

    typename operand_type::const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        add_intersection(intersection, *it_++);
}

//==============================================================================
//= Symmetric difference
//==============================================================================

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
SubType& interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::flip(const segment_type& inter_val)
{
    // That which is common shall be subtracted
    // That which is not shall be added
    // So x has to be 'complementary added' or flipped
    interval_type span = inter_val;
    typename ImplSetT::const_iterator fst_ = _set.lower_bound(span);
    typename ImplSetT::const_iterator end_ = _set.upper_bound(span);

    interval_type covered, left_over;
    typename ImplSetT::const_iterator it_ = fst_;
    while(it_ != end_) 
    {
        covered = *it_++; 
        //[a      ...  : span
        //     [b ...  : covered
        //[a  b)       : left_over
        left_over = right_subtract(span, covered);
        subtract(span & covered); //That which is common shall be subtracted
        add(left_over);                //That which is not shall be added

        //...      d) : span
        //... c)      : (*it_); span.left_subtract(*it_);
        //     [c  d) : span'
        span.left_subtract(covered);
    }

    //If span is not empty here, it_ is not in the set so it_ shall be added
    add(span);
    return *that();
}


template
<
    class SubType, 
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
    template<class SubType2>
SubType& interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::flip(const interval_base_set<SubType2,DomainT,Compare,Interval,Alloc>& operand)
{
    typedef interval_base_set<SubType2,DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
        return *that();

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, *this))
        return *that() += operand;

    typename operand_type::const_iterator it_ = operand.begin();

    // All elements of operand left of the common range are added
    while(it_ != common_lwb)
        add(*it_++);
    // All elements of operand in the common range are symmertrically subtracted
    while(it_ != common_upb)
        flip(*it_++);
    // All elements of operand right of the common range are added
    while(it_ != operand.end())
        add(*it_++);

    return *that();
}


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::join()
{
    iterator it_=_set.begin();
    if(it_==_set.end()) 
        return *this;

    iterator next_=it_; next_++;
    //if(next_==_set.end()) 
    //    return *this;

    while(next_ != _set.end())
    {
        if( (*it_).touches(*next_) )
        {
            iterator fst_mem = it_;  // hold the first member
            
            // Go on while touching members are found
            it_++; next_++;
            while(     next_ != _set.end()
                    && (*it_).touches(*next_) )
            { it_++; next_++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence
            const_cast<interval_type&>(*it_).extend(*fst_mem);
            _set.erase(fst_mem, it_);

            it_++; next_=it_; 
            if(next_!=_set.end())
                next_++;
        }
        else { it_++; next_++; }
    }
    return *this;
}



template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::uniform_bounds(itl::bound_type bounded)
{
    // I can do this only, because I am shure that the contents and the
    // ordering < on interval is invariant wrt. this transformation on bounds
    FOR_IMPL(it_) const_cast<interval_type&>(*it_).as(bounded);
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator == (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    //MEMO PORT: This implementation worked with stlport, sgi and gnu 
    // implementations of the stl. But using MSVC-implementation
    // results in runtime error! So I had to provide an independent
    // safe implementation.
    //return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return Set::lexicographical_equal(lhs, rhs);
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator != (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs == rhs); }


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator < (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
        Compare<Interval<DomainT,Compare> >()//NOTE DESIGN TTP: Why Compare is needs to be ttp
        );
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator > (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return rhs < lhs; }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator <= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs > rhs); }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator >= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs < rhs); }

template<class CharType, class CharTraits, 
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> IntervalSetT;
    stream << "{";
    ITL_const_FORALL(typename IntervalSetT, it_, object)
        stream << (*it_);

    return stream << "}";
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};



}} // namespace itl boost

#endif


