/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __split_interval_set_JOFA_990223__
#define __split_interval_set_JOFA_990223__

#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set.hpp>

namespace boost{namespace itl
{

/** implements a set as a set of intervals - on insertion overlapping intervals are split */
template 
<
    typename             DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
class split_interval_set: 
    public interval_base_set<split_interval_set<DomainT,Compare,Interval,Alloc>, 
                             DomainT,Compare,Interval,Alloc>
{
public:
    typedef interval_base_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc>,
                              DomainT,Compare,Interval,Alloc> base_type;

    typedef split_interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef interval_set<DomainT,Compare,Interval,Alloc> joint_type;
    typedef type overloadable_type;

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

    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// The corresponding atomized type representing this interval container of elements
    typedef typename itl::set<DomainT,Compare,Alloc> atomized_type;

    /// Container type for the implementation 
    typedef typename itl::set<interval_type,ITL_EXCLUSIVE_LESS(interval_type),Alloc> ImplSetT;

    /// key type of the implementing container
    typedef typename ImplSetT::key_type   key_type;
    /// data type of the implementing container
    typedef typename ImplSetT::data_type  data_type;
    /// value type of the implementing container
    typedef typename ImplSetT::value_type value_type;

    /// iterator for iteration over intervals
    typedef typename ImplSetT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_iterator const_iterator;

    enum { fineness = 3 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    split_interval_set(): base_type() {}

    /// Copy constructor
    split_interval_set(const split_interval_set& src): base_type(src) {}

    /// Copy constructor for base_type
    template<class SubType>
    split_interval_set
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { assign(src); }

    /// Constructor for a single element
    explicit split_interval_set(const interval_type& elem): base_type() { add(elem); }
    /// Constructor for a single interval
    explicit split_interval_set(const domain_type& itv): base_type() { add(itv); }

    /// Assignment operator
    template<class SubType>
    split_interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        this->clear();
        this->_set.insert(src.begin(), src.end());
    }
    
private:
    friend class 
        interval_base_set<split_interval_set<DomainT,Compare,Interval,Alloc>,
                                             DomainT,Compare,Interval,Alloc>;

    /// Does the set contain the interval  <tt>sub</tt>?
    bool contains_(const interval_type& sub)const;

    /// Addition of an interval <tt>addend</tt>
    void add_(const value_type& addend);
    iterator add_(iterator prior_, const value_type& addend);

    /// Subtraction of an interval <tt>minuend</tt>
    void subtract_(const value_type& minuend);

private:
    /// Treatment of adjoint intervals on insertion
    void handle_neighbours(const iterator& it_){}

    void add_front(const interval_type& inter_val, iterator& first_);
    void add_main(interval_type& inter_val, iterator& it_, const iterator& last_);
    void add_segment(const interval_type& inter_val, iterator& it_);
    void add_rear(const interval_type& inter_val, iterator& it_);

    void subtract_rest(const interval_type& x_itv, iterator& it_, iterator& end_  );
} ;


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    if(addend.empty()) return;

    std::pair<iterator,bool> insertion = this->_set.insert(addend);

    if(!insertion.second)
    {
        iterator first_ = this->_set.lower_bound(addend),
                 last_  = insertion.first;
        //BOOST_ASSERT(next(last_) == this->_set.upper_bound(inter_val));

        iterator it_ = first_;
        interval_type rest_interval = addend;

        add_front(rest_interval, it_);
        add_main (rest_interval, it_, last_);
        add_rear (rest_interval, it_);
    }
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline typename split_interval_set<DomainT,Compare,Interval,Alloc>::iterator
    split_interval_set<DomainT,Compare,Interval,Alloc>::add_(iterator prior_, const value_type& addend)
{
    if(addend.empty()) 
        return prior_;

    iterator insertion = this->_set.insert(prior_, addend);

    if(*insertion == addend)
        return insertion;
    else
    {
        std::pair<iterator,iterator> overlap = this->_set.equal_range(addend);
        iterator first_ = overlap.first,
                 end_   = overlap.second,
                 last_  = end_; --last_;

        iterator it_ = first_;
        interval_type rest_interval = addend;

        add_front(rest_interval, it_);
        add_main (rest_interval, it_, last_);
        add_rear (rest_interval, it_);

        return it_;
    }
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>
    ::add_front(const interval_type& inter_val, iterator& first_)
{
    // If the collision sequence has a left residual 'left_resid' it will
    // be split, to provide a standardized start of algorithms:
    // The addend interval 'inter_val' covers the beginning of the collision sequence.

    // only for the first there can be a left_resid: a part of *first_ left of inter_val
    interval_type left_resid = right_subtract(*first_, inter_val);

    if(!left_resid.empty())
    {   //            [------------ . . .
        // [left_resid---first_ --- . . .
        iterator prior_ = this->prior(first_);
        const_cast<interval_type&>(*first_).left_subtract(left_resid);
        //NOTE: Only splitting
        iterator insertion_ = this->_set.insert(prior_, left_resid);
    }

    //POST:
    // [----- inter_val ---- . . .
    // ...[-- first_ --...
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>
    ::add_main(interval_type& x_rest, iterator& it_, const iterator& last_)
{
    interval_type cur_interval;
    while(it_!=last_)
    {
        cur_interval = *it_ ;
        add_segment(x_rest, it_);
        // shrink interval
        x_rest.left_subtract(cur_interval);
    }
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>
    ::add_segment(const interval_type& inter_val, iterator& it_)
{
    interval_type lead_gap = right_subtract(inter_val, *it_);
    if(!lead_gap.empty())
        //           [lead_gap--- . . .
        // [prior_)           [-- it_ ...
        this->_set.insert(prior(it_), lead_gap);

    // . . . --------- . . . addend interval
    //      [-- it_ --)      has a common part with the first overval
    ++it_;
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>
    ::add_rear(const interval_type& inter_val, iterator& it_)
{
    iterator prior_ = this->prior(it_);
    interval_type cur_itv = *it_;

    interval_type lead_gap = right_subtract(inter_val, cur_itv);
    if(!lead_gap.empty())
        //          [lead_gap--- . . .
        // [prior_)          [-- it_ ...
        this->_set.insert(prior_, lead_gap);
    
    interval_type end_gap = left_subtract(inter_val, cur_itv);
    if(!end_gap.empty())
        // [---------------end_gap)
        //      [-- it_ --)
        it_ = this->_set.insert(it_, end_gap);
    else
    {
        // only for the last there can be a right_resid: a part of *it_ right of addend
        interval_type right_resid = left_subtract(cur_itv, inter_val);

        if(!right_resid.empty())
        {
            // [--------------)
            //      [-- it_ --right_resid)
            const_cast<interval_type&>(*it_).right_subtract(right_resid);
            it_ = this->_set.insert(it_, right_resid);
        }
    }
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
{
    if(minuend.empty()) return;
    iterator first_ = this->_set.lower_bound(minuend);
    if(first_==this->_set.end()) return;
    iterator end_   = this->_set.upper_bound(minuend);
    iterator second_= first_; ++second_;
    iterator last_  = end_; --last_;

    interval_type leftResid = right_subtract(*first_, minuend);
    interval_type rightResid; 
    if(first_ != end_  )
        rightResid = left_subtract(*last_ , minuend);

    this->_set.erase(first_, end_  );
    
    if(!leftResid.empty())
        this->_set.insert(leftResid);

    if(!rightResid.empty())
        this->_set.insert(rightResid);
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Equality on discrete interval sets. Discrete interval sets are equal, if
    their elements are equal and their fragmentation is equal.
    NOTE: This is not inline with the mathematical view.
    We have a distinction between 'element equality' and 'lexicographical 
    equality'.    */
template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator == (const split_interval_set<DomainT,Compare,Interval,Alloc>& lhs,
                         const split_interval_set<DomainT,Compare,Interval,Alloc>& rhs)
{
    //MEMO PORT: This implementation worked with stlport, sgi and gnu 
    // implementations of the stl. But using MSVC-implementation
    // results in runtime error! So I had to provide an independent
    // safe implementation.
    //return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return Set::lexicographical_equal(lhs, rhs);
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "sp_itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};


}} // namespace itl boost

#endif



