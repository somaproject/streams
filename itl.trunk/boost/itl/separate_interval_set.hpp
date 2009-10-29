/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SEPARATE_INTERVAL_SET_HPP_JOFA_080608
#define BOOST_ITL_SEPARATE_INTERVAL_SET_HPP_JOFA_080608

#include <boost/assert.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set.hpp>

namespace boost{namespace itl
{

/** Implements a set as a set of intervals - leaving adjoining intervals separate */
template 
<
    typename                         DomainT, 
    ITL_COMPARE                      Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC                        Alloc    = std::allocator
> 
class separate_interval_set: 
    public interval_base_set<separate_interval_set<DomainT,Compare,Interval,Alloc>,
                             DomainT,Compare,Interval,Alloc>
{
public:
    // inherit all typedefs

    typedef interval_base_set<itl::separate_interval_set<DomainT,Compare,Interval,Alloc>,
                              DomainT,Compare,Interval,Alloc> base_type;

    typedef separate_interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef type overloadable_type;

    typedef interval_set<DomainT,Compare,Interval,Alloc> joint_type;

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


    enum { fineness = 2 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    separate_interval_set(): base_type() {}
    /// Copy constructor
    separate_interval_set(const separate_interval_set& src): base_type(src) {}

    /// Copy constructor for base_type
    template<class SubType>
    separate_interval_set
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
    }

    /// Constructor for a single element
    explicit separate_interval_set(const domain_type& elem): base_type() { add(elem); }
    /// Constructor for a single interval
    explicit separate_interval_set(const interval_type& itv): base_type() { add(itv); }

    /// Assignment operator
    template<class SubType>
    separate_interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
        return *this; 
    }

    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        this->clear();
        this->_set.insert(src.begin(), src.end());
    }

private:
    friend class 
        interval_base_set<separate_interval_set<DomainT,Compare,Interval,Alloc>,
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
    void handle_neighbours(const iterator& it_){}
} ;


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void separate_interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    if(addend.empty()) return;

    std::pair<iterator,bool> insertion = this->_set.insert(addend);

    if(insertion.second)
        handle_neighbours(insertion.first);
    else
    {
        iterator first_ = this->_set.lower_bound(addend),
                 last_  = insertion.first,
                 end_   = insertion.first; end_  ++;
        //BOOST_ASSERT(end_ == this->_map.upper_bound(inter_val));
        iterator second_= first_; ++second_;

        interval_type leftResid  = right_subtract(*first_, addend);
        interval_type rightResid =  left_subtract(*last_ , addend);

        this->_set.erase(second_, end_  );

        interval_type extended = addend;
        extended.extend(leftResid).extend(rightResid);

        const_cast<value_type&>(*first_) = extended;
    }
}

template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
typename separate_interval_set<DomainT,Compare,Interval,Alloc>::iterator 
    separate_interval_set<DomainT,Compare,Interval,Alloc>::add_(iterator prior_, const value_type& addend)
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

        iterator second_= first_; ++second_;

        interval_type leftResid  = right_subtract(*first_, addend);
        interval_type rightResid =  left_subtract(*last_ , addend);

        this->_set.erase(second_, end_);

        interval_type extended = addend;
        extended.extend(leftResid).extend(rightResid);

        const_cast<value_type&>(*first_) = extended;
        return first_;
    }
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void separate_interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
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

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_separator<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_separator<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::separate_interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "se_itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};

}} // namespace itl boost

#endif


