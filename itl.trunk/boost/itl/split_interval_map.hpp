/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SPLIT_INTERVAL_MAP_HPP_JOFA_000706
#define BOOST_ITL_SPLIT_INTERVAL_MAP_HPP_JOFA_000706

#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/interval_base_map.hpp>
#include <boost/itl/split_interval_set.hpp>

namespace boost{namespace itl
{

/** implements a map as a map of intervals - on insertion overlapping intervals are split and associated values are combined. */
template
<
    typename DomainT,
    typename CodomainT,
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section  = ITL_SECTION_INSTANCE(itl::inplace_et, CodomainT), 
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
>
class split_interval_map:
    public interval_base_map<split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                             DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
{
public:
    typedef Traits traits;
    typedef split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> type;
    typedef       interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> joint_type;
    typedef type overloadable_type;

    typedef interval_base_map <split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                               DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> base_type;

    typedef DomainT domain_type;
    typedef CodomainT codomain_type;
    typedef Interval<DomainT,Compare> interval_type;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::element_type element_type;
    typedef typename base_type::segment_type segment_type;
    typedef typename base_type::domain_mapping_type    domain_mapping_type;
    typedef typename base_type::interval_mapping_type  interval_mapping_type;
    typedef typename base_type::ImplMapT ImplMapT;

    typedef typename base_type::codomain_combine codomain_combine;

    typedef interval_set<DomainT,Compare,Interval,Alloc> interval_set_type;
    typedef interval_set_type set_type;

    enum { fineness = 3 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    split_interval_map(): base_type() {}
    /// Copy constructor
    split_interval_map(const split_interval_map& src): base_type(src) {}

    explicit split_interval_map(domain_mapping_type& base_pair): base_type()
    { add(base_pair); }

    explicit split_interval_map(const value_type& value_pair): base_type()
    { add(value_pair); }

    /// Assignment operator
    template<class SubType>
    split_interval_map& operator =
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Compare,Combine,Section,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_map.
    template<class SubType>
    void assign(const interval_base_map<SubType,DomainT,CodomainT,
                                        Traits,Compare,Combine,Section,Interval,Alloc>& src)
    {
        this->clear();
        this->_map.insert(src.begin(), src.end());
    }

    //==========================================================================
    //= Selection
    //==========================================================================

    //MEMO DESIGN DECISION: split_interval_map::operator[](interval): 
    // If used for selection will deliver a set of associated
    // values. It could only implemented for a single key value. But this 
    // would mislead the unexperienced user to hash a split_interval_map into 
    // singular intervals by inapt usage of op[]. So op[] will not be implemented
    // codomain_type& operator[](const interval_type& interval_of_keys)

private:
    // The following _suffixed function templates funx_ are implementations
    // correspoding unsuffixed function templates funx of the base class.
    // For funx_ want to be used in the base class via derived SubType pointer:
    // that()->funx_. So they must be either public or made accessible by a
    // friend declaration:
    friend class  
        interval_base_map
        <split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                            DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc  >;

    bool contains_(const value_type& value)const;

    template<class Combiner>
    void add_(const value_type&);

    template<class Combiner>
    iterator add_(iterator, const value_type&);

    template<class Combiner>
    void subtract_(const value_type&);

    void insert_(const value_type& value);
    iterator insert_(iterator, const value_type& value);
    void erase_(const value_type& value);

private:
    void handle_neighbours(const iterator& it_){}
    
    template<class Combiner>
    void add_main(interval_type& inter_val, const CodomainT& co_val, 
                 iterator& it_, const iterator& end_  );
    template<class Combiner>
    void add_segment(const interval_type& inter_val, const CodomainT& co_val, 
                     iterator& it_);

    void add_front(const interval_type& inter_val, iterator& it_);

    template<class Combiner>
    void add_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_);

    template<class Combiner>
    void subtract_main(const interval_type& inter_val, const CodomainT& co_val, 
                       iterator& it_, iterator& end_  );

    void subtract_front(const interval_type& inter_val, const CodomainT& co_val, iterator& it_);

    template<class Combiner>
    void subtract_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_);

    void insert_range(const interval_type& inter_val, const CodomainT& co_val, iterator& it_, iterator& end_  );
    void erase_rest(const interval_type& inter_val, const CodomainT& co_val, iterator& it_, iterator& last_);
} ;

//-----------------------------------------------------------------------------
// add<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_(const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    std::pair<iterator,bool> insertion 
        = this->template map_insert<Combiner>(inter_val, co_val);

    if(!insertion.second)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = this->_map.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert(end_ == this->_map.upper_bound(inter_val));

        iterator it_ = first_;
        interval_type rest_interval = inter_val;

        add_front         (rest_interval, it_);
        add_main<Combiner>(rest_interval, co_val, it_, last_);
        add_rear<Combiner>(rest_interval, co_val, it_);
    }    
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline typename split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::iterator
    split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_(iterator prior_, const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(inter_val.empty()) 
        return prior_;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return prior_;

    std::pair<iterator,bool> insertion 
        = this->template map_add<Combiner>(prior_, inter_val, co_val);

    if(insertion.second)
        return insertion.first;
    else
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = this->_map.equal_range(inter_val);
        iterator it_   = overlap.first,
                 last_ = overlap.second;
                 --last_;
        interval_type rest_interval = inter_val;

        add_front         (rest_interval, it_);
        add_main<Combiner>(rest_interval, co_val, it_, last_);
        add_rear<Combiner>(rest_interval, co_val, it_);

        return it_;
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_front(const interval_type& inter_val, iterator& first_)
{
    // If the collision sequence has a left residual 'left_resid' it will
    // be split, to provide a standardized start of algorithms:
    // The addend interval 'inter_val' covers the beginning of the collision sequence.

    // only for the first there can be a left_resid: a part of *first_ left of inter_val
    interval_type left_resid = right_subtract(first_->first, inter_val);

    if(!left_resid.empty())
    {   //            [------------ . . .
        // [left_resid---first_ --- . . .
        iterator prior_ = this->prior(first_);
        const_cast<interval_type&>(first_->first).left_subtract(left_resid);
        //NOTE: Only splitting
        iterator insertion_ = this->_map.insert(prior_, value_type(left_resid, first_->second));
    }

    //POST:
    // [----- inter_val ---- . . .
    // ...[-- first_ --...
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_main(interval_type& x_rest, const CodomainT& co_val, iterator& it_, const iterator& last_)
{
    interval_type cur_interval;
    while(it_!=last_)
    {
        cur_interval = it_->first ;
        add_segment<Combiner>(x_rest, co_val, it_);
        // shrink interval
        x_rest.left_subtract(cur_interval);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_segment(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type lead_gap = right_subtract(inter_val, it_->first);
    if(!lead_gap.empty())
        //           [lead_gap--- . . .
        // [prior_)           [-- it_ ...
        this->template gap_insert<Combiner>(prior(it_), lead_gap, co_val);

    // . . . --------- . . . addend interval
    //      [-- it_ --)      has a common part with the first overval
    Combiner()(it_->second, co_val);
    if(Traits::absorbs_neutrons && it_->second == Combiner::neutron())
        this->_map.erase(it_++);
    else 
        ++it_;
}



template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    iterator prior_ = this->prior(it_);
    interval_type cur_itv = (*it_).first ;

    interval_type lead_gap = right_subtract(inter_val, cur_itv);
    if(!lead_gap.empty())
        //          [lead_gap--- . . .
        // [prior_)          [-- it_ ...
        this->template gap_insert<Combiner>(prior_, lead_gap, co_val);
    

    interval_type end_gap = left_subtract(inter_val, cur_itv);
    if(!end_gap.empty())
    {
        // [---------------end_gap)
        //      [-- it_ --)
        Combiner()(it_->second, co_val);

        if(Traits::absorbs_neutrons && it_->second == Combiner::neutron())
        {
            this->_map.erase(it_);
            it_ = this->template gap_insert<Combiner>(prior_, end_gap, co_val);
        }
        else
            it_ = this->template gap_insert<Combiner>(it_, end_gap, co_val);
    }
    else
    {
        // only for the last there can be a right_resid: a part of *it_ right of addend
        interval_type right_resid = left_subtract(cur_itv, inter_val);

        if(right_resid.empty())
        {
            // [---------------)
            //      [-- it_ ---)
            Combiner()(it_->second, co_val);

            if(Traits::absorbs_neutrons && it_->second == Combiner::neutron())
            {
                this->_map.erase(it_);
                it_ = prior_;
            }
        }
        else
        {
            // [--------------)
            //      [-- it_ --right_resid)
            const_cast<interval_type&>(it_->first).right_subtract(right_resid);

            //NOTE: This is NOT an insertion that has to take care for correct application of
            // the Combiner functor. It only reestablished that state after splitting the
            // 'it_' interval value pair. Using map_insert<Combiner> does not work here.
            iterator insertion_ = this->_map.insert(it_, value_type(right_resid, it_->second));

            Combiner()(it_->second, co_val);

            if(Traits::absorbs_neutrons && it_->second == Combiner::neutron())
                this->_map.erase(it_);

            it_ = insertion_;
        }
    }
}




//-----------------------------------------------------------------------------
// subtract<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_(const value_type& minuend)
{
    interval_type inter_val = minuend.first;

    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = minuend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    iterator first_ = this->_map.lower_bound(inter_val);
    if(first_==this->_map.end()) 
        return;
    iterator end_   = this->_map.upper_bound(inter_val);
    if(first_==end_  ) 
        return;

    iterator last_  = end_; --last_;
    iterator it_    = first_;
    subtract_front         (inter_val, co_val, it_);
    subtract_main<Combiner>(inter_val, co_val, it_, last_);
    subtract_rear<Combiner>(inter_val, co_val, it_);
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_front(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type left_resid = right_subtract(it_->first, inter_val);

    if(!left_resid.empty())
    {
        iterator prior_ = it_;
        if(prior_ != this->_map.begin())
            --prior_;
        const_cast<interval_type&>(it_->first).left_subtract(left_resid);
        this->_map.insert(prior_, value_type(left_resid, it_->second));
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_main(const interval_type& inter_val, const CodomainT& co_val, 
                    iterator& it_, iterator& last_)
{
    while(it_ != last_)
    {
        Combiner()(it_->second, co_val);
        if(Traits::absorbs_neutrons && it_->second==Combiner::neutron())
            this->_map.erase(it_++); 
        else ++it_;
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type right_resid = left_subtract(it_->first, inter_val);

    if(right_resid.empty())
    {
        CodomainT& cur_val = it_->second ;
        Combiner()(cur_val, co_val);
        if(Traits::absorbs_neutrons && cur_val==Combiner::neutron())
            this->_map.erase(it_);
    }
    else
    {   // . . . ---)
        // . . . ---right_resid) : split it_
        const_cast<interval_type&>(it_->first).right_subtract(right_resid);
        iterator next_ = this->_map.insert(it_, value_type(right_resid, it_->second));
        Combiner()(it_->second, co_val);
        if(Traits::absorbs_neutrons && it_->second==Combiner::neutron())
            this->_map.erase(it_);
    }
}


//-----------------------------------------------------------------------------
// insert(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_(const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    std::pair<iterator,bool> insertion = this->_map.insert(addend);

    if(!insertion.second)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = this->_map.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert((++last_) == this->_map.upper_bound(inter_val));
        iterator it_ = first_;
        insert_range(inter_val, co_val, it_, last_);
    }
}

template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline typename split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::iterator
    split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_(iterator prior_, const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(inter_val.empty()) 
        return prior_;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return prior_;

    std::pair<iterator,bool> insertion 
        = this->template map_insert<codomain_combine>(prior_, inter_val, co_val);

    if(insertion.second)
        return insertion.first;
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = this->_map.equal_range(inter_val);
        iterator it_    = overlap.first,
                 last_  = overlap.second;
                 --last_;
        insert_range(inter_val, co_val, it_, last_);
        return it_;
    }
}


template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_range(const interval_type& inter_val, const CodomainT& co_val, iterator& it_, iterator& last_)
{
    iterator end_   = last_ ; ++end_;
    iterator prior_ = it_, inserted_;
    if(prior_ != this->_map.end())
        --prior_;
    interval_type rest_interval = inter_val, left_gap, cur_itv;
    interval_type last_interval = last_ ->first;

    while(it_ != end_  )
    {
        cur_itv = it_->first ;            
        left_gap = right_subtract(rest_interval, cur_itv);

        if(!left_gap.empty())
            inserted_ = this->_map.insert(prior_, value_type(left_gap, co_val));

        // shrink interval
        rest_interval.left_subtract(cur_itv);
        prior_ = it_;
        ++it_;
    }

    //insert_rear(rest_interval, co_val, last_):
    interval_type end_gap = left_subtract(rest_interval, last_interval);
    if(!end_gap.empty())
        it_ = this->_map.insert(prior_, value_type(end_gap, co_val));
    else
        it_ = prior_;

}


//-----------------------------------------------------------------------------
// erase(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_(const value_type& minuend)
{
    interval_type inter_val = minuend.first;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = minuend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    iterator first_ = this->_map.lower_bound(inter_val);
    if(first_==this->_map.end()) 
        return;
    iterator end_   = this->_map.upper_bound(inter_val);
    if(first_==end_  ) 
        return;

    iterator last_  = end_; --last_;

    iterator second_= first_; ++second_;
    if(first_ == last_) 
    {
        // only for the last there can be a right_resid: a part of *it_ right of minuend
        interval_type right_resid = left_subtract(first_->first, inter_val);

        if(first_->second == co_val)
        {
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(!left_resid.empty())
            {
                const_cast<interval_type&>(first_->first) = left_resid;
                if(!right_resid.empty())
                    this->_map.insert(first_, value_type(right_resid, co_val));
            }
            else if(!right_resid.empty())
                const_cast<interval_type&>(first_->first) = right_resid;
            else
                this->_map.erase(first_);
        }
    }
    else
    {
        // first AND NOT last
        if(first_->second == co_val)
        {
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(left_resid.empty())
                this->_map.erase(first_);
            else
                const_cast<interval_type&>(first_->first) = left_resid;
        }

        erase_rest(inter_val, co_val, second_, last_);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_rest(const interval_type& inter_val, const CodomainT& co_val, 
                 iterator& it_, iterator& last_)
{
    // For all intervals within loop: it_->first are contained_in inter_val
    while(it_ != last_)
        if((*it_).second == co_val)
            this->_map.erase(it_++); 
        else it_++;

    //erase_rear:
    if(it_->second == co_val)
    {
        interval_type right_resid = left_subtract(it_->first, inter_val);
        if(right_resid.empty())
            this->_map.erase(it_);
        else
            const_cast<interval_type&>(it_->first) = right_resid;
    }
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_set<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::absorbs_neutrons)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::is_total)); 
};


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    static std::string apply()
    { 
        return "sp_itv_map<"+ type_to_string<DomainT>::apply()   + ","
                            + type_to_string<CodomainT>::apply() + ","
                            + type_to_string<Traits>::apply()    +">"; 
    }
};


}} // namespace itl boost

#endif


