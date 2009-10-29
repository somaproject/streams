/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_SET_HPP_JOFA_990223
#define BOOST_ITL_INTERVAL_SET_HPP_JOFA_990223

#include <boost/assert.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/functions.hpp>

namespace boost{namespace itl
{

/** Implements a set as a set of intervals - merging adjoining intervals */
template 
<
    typename    DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
class interval_set: 
    public interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                             DomainT,Compare,Interval,Alloc>
{
public:

    /// The base_type of this class
    typedef interval_base_set<itl::interval_set<DomainT,Compare,Interval,Alloc>,
                              DomainT,Compare,Interval,Alloc> base_type;

    typedef interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef type overloadable_type;

    typedef type joint_type;

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

    enum { fineness = 1 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    interval_set(): base_type() {}
    /// Copy constructor
    interval_set(const interval_set& src): base_type(src) {}

    /// Copy constructor for base_type
    template<class SubType>
    explicit interval_set
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
    }

    /// Constructor for a single element
    explicit interval_set(const domain_type& value): base_type() 
    { add(interval_type(value)); }
    /// Constructor for a single interval
    explicit interval_set(const interval_type& itv): base_type() 
    { 
        add(itv); 
    }

    /// Assignment operator
    template<class SubType>
    interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
        return *this; 
    }


    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> base_set_type;
        this->clear();
        // Has to be implemented via add. there might be touching borders to be joined
        iterator prior_ = this->_set.end();
        ITL_const_FORALL(typename base_set_type, it_, src) 
            prior_ = this->add(prior_, *it_);
    }

private:
    friend class 
        interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                                       DomainT,Compare,Interval,Alloc>;

    /// Does the set contain the interval  <tt>sub</tt>?
    bool contains_(const interval_type& sub)const;

    /// Insertion of an interval <tt>addend</tt>
    void add_(const value_type& addend);

    iterator add_(iterator prior_, const value_type& addend);

    /// Removal of an interval <tt>minuend</tt>
    void subtract_(const value_type& minuend);

private:
    /// Treatment of adjoint intervals on insertion
    iterator handle_neighbours(iterator it_);

    iterator join_on_left(iterator& left_, const iterator& right_);
} ;


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline typename interval_set<DomainT,Compare,Interval,Alloc>::iterator
    interval_set<DomainT,Compare,Interval,Alloc>::handle_neighbours(iterator it_)
{
    if(it_ == this->_set.begin())
    {
        iterator it_nxt=it_; it_nxt++;
        if(it_nxt!=this->_set.end() && (*it_).touches(*it_nxt)) 
            return join_on_left(it_, it_nxt);
    }
    else
    {
        // there is a predecessor
        iterator pred_ = it_; pred_-- ;

        if((*pred_).touches(*it_)) 
        {
            iterator it_extended = join_on_left(pred_, it_);

            iterator succ_=it_extended; succ_++;
            if(succ_!=this->_set.end())
            {
                // it's a non border element that might have two touching neighbours
                if((*it_extended).touches(*succ_)) 
                    return join_on_left(it_extended, succ_);
                else
                    return it_extended;
            }
            else
                return it_extended;
        }
        else
        {
            iterator succ_=it_; succ_++;
            if(succ_!=this->_set.end())
            {
                // it's a non border element that might have a right touching neighbours
                if((*it_).touches(*succ_)) 
                    return join_on_left(it_, succ_);
            }
        }
    }

    return it_;
}



template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline typename interval_set<DomainT,Compare,Interval,Alloc>::iterator 
    interval_set<DomainT,Compare,Interval,Alloc>
    ::join_on_left(iterator& left_, const iterator& right_)
{
    // both left and right are in the set and they are neighbours
    BOOST_ASSERT((*left_).exclusive_less(*right_));
    BOOST_ASSERT((*left_).touches(*right_));

    interval_type right_itv = (*right_);
    this->_set.erase(right_);
    const_cast<value_type&>(*left_).extend(right_itv);

    return left_;
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    if(addend.empty()) return;

    std::pair<iterator,bool> insertion = this->_set.insert(addend);

    if(insertion.second)
        handle_neighbours(insertion.first);
    else
    {
        iterator first_ = this->_set.lower_bound(addend),
                 last_  = insertion.first,
                 end_   = insertion.first; ++end_;
        //BOOST_ASSERT(end_ == this->_map.upper_bound(inter_val));
        iterator second_= first_; ++second_;

        interval_type leftResid  = right_subtract(*first_, addend);
        interval_type rightResid =  left_subtract(*last_ , addend);

        this->_set.erase(second_, end_  );

        interval_type extended = addend;
        extended.extend(leftResid).extend(rightResid);

        const_cast<value_type&>(*first_) = extended;
        handle_neighbours(first_);
    }
}

template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
typename interval_set<DomainT,Compare,Interval,Alloc>::iterator 
    interval_set<DomainT,Compare,Interval,Alloc>::add_(iterator prior_, const value_type& addend)
{
    if(addend.empty()) 
        return prior_;

    iterator insertion = this->_set.insert(prior_, addend);

    if(*insertion == addend)
        return handle_neighbours(insertion);
    else
    {
        std::pair<iterator,iterator> overlap = this->_set.equal_range(addend);
        iterator first_ = overlap.first,
                 end_   = overlap.second,
                 last_  = end_; --last_;

        iterator second_= first_; ++second_;

        interval_type leftResid  = right_subtract(*first_, addend);
        interval_type rightResid =  left_subtract(*last_ , addend);

        this->_set.erase(second_, end_);

        interval_type extended = addend;
        extended.extend(leftResid).extend(rightResid);

        const_cast<value_type&>(*first_) = extended;
        return handle_neighbours(first_);
    }
}

template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
{
    if(minuend.empty()) return;
    iterator first_ = this->_set.lower_bound(minuend);
    if(first_==this->_set.end()) return;
    iterator end_   = this->_set.upper_bound(minuend);
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


//-----------------------------------------------------------------------------
// equality of elements
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool is_element_equal(const interval_set<DomainT,Compare,Interval,Alloc>& lhs,
                             const interval_set<DomainT,Compare,Interval,Alloc>& rhs)
{
    // Since both are joining interval sets we can use the simpler Set::lexicographical_equal
    return &lhs == &rhs || Set::lexicographical_equal(lhs, rhs);
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};


//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};

}} // namespace itl boost

#endif


