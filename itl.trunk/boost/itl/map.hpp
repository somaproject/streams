/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_MAP_HPP_JOFA_070519
#define BOOST_ITL_MAP_HPP_JOFA_070519

#include <string>
#include <boost/type_traits/ice.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/detail/concept_check.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/map_algo.hpp>
#include <map>


namespace boost{namespace itl
{

struct partial_absorber
{
    enum { absorbs_neutrons = true };
    enum { is_total = false };
};

template<> 
inline std::string type_to_string<partial_absorber>::apply() { return "@0"; }


struct partial_enricher
{
    enum { absorbs_neutrons = false };
    enum { is_total = false };

};

template<> 
inline std::string type_to_string<partial_enricher>::apply() { return "e0"; }

struct total_absorber
{
    enum { absorbs_neutrons = true };
    enum { is_total = true };
};

template<> 
inline std::string type_to_string<total_absorber>::apply() { return "^0"; }

struct total_enricher
{
    enum { absorbs_neutrons = false };
    enum { is_total = true };
};

template<> 
inline std::string type_to_string<total_enricher>::apply() { return "e^0"; }


/** adds addition, subtraction and intersection to std::maps */
template 
<
    typename DomainT, 
    typename CodomainT, 
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, CodomainT), 
    ITL_ALLOC   Alloc   = std::allocator 
>
class map: private std::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), 
                            Alloc<std::pair<const DomainT, CodomainT> > >
{
public:
    typedef Alloc<typename std::pair<const DomainT, CodomainT> >  allocator_type;

    typedef typename itl::map<DomainT,CodomainT,Traits, Compare,Combine,Section,Alloc> type;
    typedef typename std::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT),
                              allocator_type>              base_type;
    typedef typename itl::set<DomainT, Compare, Alloc >    set_type;

    typedef Traits traits;

public:
    typedef DomainT                                     domain_type;
    typedef DomainT                                     key_type;
    typedef CodomainT                                   codomain_type;
    typedef CodomainT                                   mapped_type;
    typedef CodomainT                                   data_type;
    typedef std::pair<const DomainT, CodomainT>         element_type;
    typedef std::pair<const DomainT, CodomainT>         value_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT)         domain_compare;
    typedef ITL_COMBINE_CODOMAIN(Combine,CodomainT)     codomain_combine;
    typedef domain_compare                              key_compare;
    typedef typename inverse<codomain_combine >::type   inverse_codomain_combine;
    typedef ITL_SECTION_CODOMAIN(Section,CodomainT)     codomain_intersect;
    typedef typename inverse<codomain_intersect>::type  inverse_codomain_intersect;
    typedef typename base_type::value_compare           value_compare;

public:
    typedef typename base_type::pointer                 pointer;
    typedef typename base_type::const_pointer           const_pointer;
    typedef typename base_type::reference               reference;
    typedef typename base_type::const_reference         const_reference;
    typedef typename base_type::iterator                iterator;
    typedef typename base_type::const_iterator          const_iterator;
    typedef typename base_type::size_type               size_type;
    typedef typename base_type::difference_type         difference_type;
    typedef typename base_type::reverse_iterator        reverse_iterator;
    typedef typename base_type::const_reverse_iterator  const_reverse_iterator;
    
    enum { fineness = 4 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    map()
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    map(const key_compare& comp): base_type(comp){}

    template <class InputIterator>
    map(InputIterator first, InputIterator past): base_type(first,past){} 

    template <class InputIterator>
    map(InputIterator first, InputIterator past, const key_compare& comp): base_type(first,past,comp) 
    {}

    map(const map& src): base_type::map(src)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    explicit map(const element_type& key_value_pair): base_type::map()
    { 
        insert(key_value_pair); 
    }

    map& operator=(const map& src) { base_type::operator=(src); return *this; } 
    void swap(map& src) { base_type::swap(src); }

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

    using base_type::erase;
    using base_type::find;
    using base_type::count;

    using base_type::lower_bound;
    using base_type::upper_bound;
    using base_type::equal_range;

    using base_type::operator[];

public:
    //==========================================================================
    //= Containedness
    //==========================================================================

    /** Checks if a key is in the map */
    bool contains(const domain_type& key)const { return !(find(key) == end()); }

    /** Checks if a key-value pair is in the map */
    bool contains(const element_type& key_value_pair)const 
    { 
        const_iterator found_ = find(key_value_pair.first);
        return found_ != end() && found_->second == key_value_pair.second;
    }

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool contained_in(const map& super)const 
    { return Map::contained_in(*this, super); }

    /** Does <tt>*this</tt> contain <tt>sub</tt>? */
    bool contains(const map& sub)const 
    { return Map::contained_in(sub, *this); }

    //==========================================================================
    //= Size
    //==========================================================================
    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    size_t iterative_size()const { return size(); }

    size_t cardinality()const { return size(); }

    //==========================================================================
    //= Selection
    //==========================================================================

    /** Total select function. */
    codomain_type operator()(const domain_type& key)const
    {
        const_iterator it = find(key); 
        return it==end() ? neutron<codomain_type>::value()
                         : it->second;
    }

    //==========================================================================
    //= Addition, subtraction
    //==========================================================================
    /** \c add inserts \c value_pair into the map if it's key does 
        not exist in the map.    
        If \c value_pairs's key value exists in the map, it's data
        value is added to the data value already found in the map. */
    map& add(const value_type& value_pair) 
    { 
        return _add<codomain_combine>(value_pair); 
    }

    /** \c add add \c value_pair into the map using \c prior as a hint to
        insert \c value_pair after the position \c prior is pointing to. */
    iterator add(iterator prior, const value_type& value_pair) 
    { 
        return _add<codomain_combine>(prior, value_pair); 
    }

    /** If the \c value_pair's key value is in the map, it's data value is
        subtraced from the data value stored in the map. */
    map& subtract(const value_type& value_pair)
    {
        if(Traits::is_total && has_inverse<codomain_type>::value)
            this->template _add<inverse_codomain_combine>(value_pair); 
        else 
            this->template _subtract<inverse_codomain_combine>(value_pair); 
    
        return *this;
    }

    //==========================================================================
    //= Insertion, erasure
    //==========================================================================

    std::pair<iterator,bool> insert(const value_type& value_pair)
    {
        if(Traits::absorbs_neutrons && value_pair.second == codomain_combine::neutron()) 
            return std::pair<iterator,bool>(end(),true);
        else
            return base_type::insert(value_pair);
    }
    
    iterator insert(iterator prior, const value_type& value_pair)
    {
        if(Traits::absorbs_neutrons && value_pair.second == codomain_combine::neutron()) 
            return end();
        else
            return base_type::insert(prior, value_pair);
    }

    /** With <tt>key_value_pair = (k,v)</tt> set value \c v for key \c k */
    map& set(const element_type& key_value_pair)
    { 
        (*this)[key_value_pair.first] = key_value_pair.second; 
        return *this; 
    }

    /** erase \c key_value_pair from the map.
        Erase only if, the exact value content \c val is stored for the given key. */
    size_type erase(const element_type& key_value_pair);

    //==========================================================================
    //= Intersection
    //==========================================================================

    /** The intersection of \c key in \c *this map is added to \c section. */
    void add_intersection(map& section, const domain_type& key_value)const;

    /** The intersection of \c key_value_pair and \c *this map is added to \c section. */
    void add_intersection(map& section, const element_type& key_value_pair)const;

    /** The intersection of set \c sectant with \c *this map is added to \c section. */
    void add_intersection(map& section, const set_type& sectant)const;

    /** The intersection of map \c sectant with \c *this map is added to \c section. */
    void add_intersection(map& section, const map& sectant)const;

    /** Returns true, if there is an intersection of \c key and \c *this map.
        Functions \c intersects and \c contains are identical on key value arguments
        of type \c domain_type. Complexity: Logarithmic in container size. */
    bool intersects(const domain_type& key)const;

    /** Returns true, if there is an intersection of \key_value_pair and \c *this map.
        If the key is found, the content of \c key_value_pair has to have an intersection 
        with the content of the data value in the map. 
        Complexity: Logarithmic in container size.     */
    bool intersects(const element_type& key_value_pair)const;

    //==========================================================================
    //= Symmetric difference
    //==========================================================================

    map& flip(const element_type& key_value_pair)
    { Map::flip(*this, map(key_value_pair)); return *this; }


    //==========================================================================
    //= Morphisms
    //==========================================================================
    
    void absorb_neutrons()
    {
        //content_is_neutron<key_type, data_type> neutron_dropper;
        if(!Traits::absorbs_neutrons)
            erase_if(content_is_neutron<value_type>());
    }

    //==========================================================================
    //= Representation
    //==========================================================================
    
    /** Represent this map as string */
    std::string as_string()const;

    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    /** \c key_value allows for a uniform access to \c key_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const key_type& key_value(IteratorT value_)
    { return (*value_).first; }

    /** \c data_value allows for a uniform access to \c data_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const data_type& data_value(IteratorT value_){ return (*value_).second; }

    /** \c codomain_value allows for a uniform access to \c codomain_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const codomain_type& codomain_value(IteratorT value_){ return (*value_).second; }

    /** \c key_less allows for a uniform notation of key comparison which
        is used for common algorithms on sets and maps. */
    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return key_compare()((*lhs_).first,(*rhs_).first); }

    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    static value_type make_element(const key_type& key_val, const data_type& data_val)
    { return value_type(key_val, data_val); }

    /** Erase the elements in *this map to which property \c hasProperty applies. 
    Keep all the rest. */
    template<class Predicate>
    map& erase_if(const Predicate&);

    /** Copy the elements in map \c src to which property \c hasProperty applies 
    into \c *this map. */
    template<class Predicate>
    map& assign_if(const map& src, const Predicate&);

    /** Copy the key values of the map to \c domain_set. Complexity: Linear. */
    void domain(set_type& domain_set)const
    {
        typename set_type::iterator prior_ = domain_set.end();
        ITL_const_FORALL_THIS(it_)
            prior_ = domain_set.insert(prior_, it_->first);
    }

private:
    template<class Combiner>
    map& _add(const value_type& value_pair);

    template<class Combiner>
    iterator _add(iterator prior, const value_type& value_pair);

    template<class Combiner>
    map& _subtract(const value_type& value_pair);
};


//==============================================================================
//= Addition
//==============================================================================

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template <class Combiner>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::_add(const value_type& val)
{
    using namespace type_traits;
    if(Traits::absorbs_neutrons && val.second == Combiner::neutron())
        return *this;

    std::pair<iterator, bool> insertion;
    if(ice_and<Traits::is_total, has_inverse<codomain_type>::value, is_negative<Combiner>::value>::value)
        insertion = insert(value_type(val.first, version<Combiner>()(val.second)));
    else // Existential case
        insertion = insert(val);

    if( insertion.second )
        return *this;
    else
    {
        iterator it = insertion.first;
        Combiner()((*it).second, val.second);

        if(Traits::absorbs_neutrons && (*it).second == Combiner::neutron())
            erase(it);

        return *this;
    }
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template <class Combiner>
typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::iterator
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::_add(iterator prior_, const value_type& val)
{
    if(Traits::absorbs_neutrons && val.second == Combiner::neutron())
        return end();

    iterator inserted_ = base_type::insert(prior_, value_type(val.first, Combiner::neutron()));
    Combiner()(inserted_->second, val.second);

    if(Traits::absorbs_neutrons && inserted_->second == Combiner::neutron())
    {
        erase(inserted_);
        return end();
    }
    else
        return inserted_;
}


//==============================================================================
//= Subtraction
//==============================================================================
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template <class Combiner>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::_subtract(const value_type& val)
{
    iterator it_ = find(val.first);
    if(it_ != end())
    {
        Combiner()((*it_).second, val.second);
        if(Traits::absorbs_neutrons && (*it_).second == codomain_combine::neutron())
            erase(it_);
    }
    return *this;
}



//==============================================================================
//= Erasure
//==============================================================================
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::size_type 
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::erase(const value_type& value_pair)
{
    if(Traits::absorbs_neutrons && value_pair.second == codomain_combine::neutron())
        return 0; // neutrons are never contained 'substantially' 
                  // only 'virually'.

    iterator it_ = find(value_pair.first);
    if(it_ != end() && value_pair.second == it_->second)
    {
        erase(it_);
        return 1;
    }

    return 0;
}


//==============================================================================
//= Intersection
//==============================================================================

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::add_intersection(map& section, const value_type& sectant)const
{
    if(Traits::is_total)
    {
        section = *this;
        section.add(sectant);
    }
    else
    {
        const_iterator it_ = find(sectant.first);
        if(it_ != end())
        {
            section.add(*it_);
            if(is_set<codomain_type>::value)
                section.template _add<codomain_intersect>(sectant); 
            else
                section.template _add<codomain_combine>(sectant);
        }
    }
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::add_intersection(map& section, const domain_type& sectant)const
{
    const_iterator it_ = find(sectant);
    if(it_ != end()) 
        section.add(*it_);
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::add_intersection(map& section, const typename map::set_type& sectant)const
{
    typename set_type::const_iterator common_lwb_;
    typename set_type::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sectant, *this))
        return;

    typename set_type::const_iterator sec_ = common_lwb_;
    while(sec_ != common_upb_)
        add_intersection(section, *sec_++);
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::add_intersection(map& section, const map& sectant)const
{
    const_iterator common_lwb_;
    const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sectant, *this))
        return;

    const_iterator sec_ = common_lwb_;
    while(sec_ != common_upb_)
    {
        const_iterator it_ = find(sec_->first);
        if(it_ != end())
        {
            section.add(*it_);
            if(is_set<CodomainT>::value)
                section.template _add<codomain_intersect>(*sec_); 
            else
                section.template _add<codomain_combine>(*sec_);
        }
        ++sec_;
    }
}

//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::intersects(const domain_type& key)const
{
    return traits::is_total || contains(key);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::intersects(const element_type& key_value_pair)const
{
    if(traits::is_total)
        return true;

    type intersection;
    add_intersection(intersection, key_value_pair);
    return !intersection.empty(); 
}


//==============================================================================
//= Conversion
//==============================================================================
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
std::string map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::as_string()const
{ 
    std::string repr;
    ITL_const_FORALL_THIS(it) {
        std::string elem("(");
        elem += to_string<DomainT>::apply((*it).first);
        elem += "->";
        elem += to_string<CodomainT>::apply((*it).second);
        elem += ")";

        repr += elem;
    }
    return repr;
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template<class Predicate>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::erase_if(const Predicate& pred)
{
    iterator it = begin();
    while(it != end())
        if(Predicate()(*it))
            erase(it++); 
        else ++it;
    return *this;
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template<class Predicate>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::assign_if(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& src, const Predicate& pred)
{
    clear();
    const_iterator it = src.begin();
    while(it != src.end()) {
        if(Predicate()(*it)) 
            add(*it++); 
    }
    return *this;
}

//-------------------------------------------------------------------------
template <class DomainT, class CodomainT, class Traits, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& 
    insert(map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object, 
           const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& addend) 
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> map_type;

    ITL_const_FORALL(typename map_type, elem_, addend) 
        object.insert(*elem_); 

    return object; 
}

template <class DomainT, class CodomainT, class Traits, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& 
    erase(map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object, 
          const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& erasure) 
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> map_type;

    ITL_const_FORALL(typename map_type, elem_, erasure) 
        object.erase(*elem_); 

    return object; 
}

template <class DomainT, class CodomainT, class Traits, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& 
    erase(map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object, 
          const set<DomainT,Compare,Alloc>& erasure) 
{
    typedef set<DomainT,Compare,Alloc> operand_type;

    ITL_const_FORALL(typename operand_type, elem_, erasure) 
        object.erase(*elem_); 

    return object; 
}


//-----------------------------------------------------------------------------
// non member functions
//-----------------------------------------------------------------------------

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Standard equality, which is lexicographical equality of the sets
    as sequences, that are given by their Compare order. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator == (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                         const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator != (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                         const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs == rhs); }

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool is_element_equal(const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

/** Protonic equality is equality on all elements that do not carry a neutron as content. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool is_protonic_equal (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                               const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    return Map::lexicographical_protonic_equal(lhs, rhs);
}

/** Strict weak less ordering which is given by the Compare order */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator < (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator<((const base_type&)lhs, (const base_type&)rhs);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator > (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return rhs < lhs; }

/** Partial ordering which is induced by Compare */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator <= (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs > rhs); }

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator >= (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs < rhs); }

//------------------------------------------------------------------------------
//==============================================================================
//= Addition
//==============================================================================

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator += (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& operand)
{ return object.add(operand); }

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator + (       itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>                object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& operand)
{
    return object += operand; 
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator + (const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& operand,
                           itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>                object  )
{
    return object += operand; 
}

/** Add a map \c operand to map \c object. If an element of \c operand already exists
    in \c object, add up the contents. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator += (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    return Set::add(object, operand); 
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator +  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>  object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
    return object += operand; 
}


//--------------------------------------------------------------------------

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator |= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{ return object.add(operand); }

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator | (       itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>              object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{
    return object |= operand; 
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator | (const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& operand,
                           itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>                object  )
{
    return object |= operand; 
}

/** Add a map \c operand to map \c object. If an element of \c operand already exists
    in \c object, add up the contents. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator |= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    return Set::add(object, operand); 
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator |  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>  object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
    return object |= operand; 
}


//==============================================================================
//= Subtraction
//==============================================================================

/** Subtract a map \c x2 from this map. If an element of \c x2 already exists
    in \c *this, subtract the contents using <tt>operator -=</tt>. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator -= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
    ITL_const_FORALL(typename ObjectT, it_, operand)
        object.subtract(*it_);

    return object; 
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator -  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>  object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    return object -= operand; 
}

/** Subtract a set \c x2 from this map. Every element of \c this map that
    has a key that is element of \c x2 is deleted from the map. */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator -= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
    return Set::erase(object, operand); 
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator -  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&           object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
    return object -= operand; 
}


//==============================================================================
//= Intersection
//==============================================================================

/** Intersect map \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator &= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
    typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> object_type;
    if(Traits::is_total) 
        return object += operand;
    else
    {
        object_type section;
        object.add_intersection(section, operand);
        object.swap(section);
        return object;
    }
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator &  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    return object &= operand; 
}

/** Intersect set \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator &= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
    typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> object_type;
    object_type section;
    object.add_intersection(section, operand);
    object.swap(section);
    return object;
}


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator &  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>            object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
    return object &= operand; 
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator & (const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand,
                           itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>            object  )
{
    return object &= operand; 
}

//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&              object, 
       const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::domain_type& key)
{
    return object.intersects(key);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(
      const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::domain_type& key,
               const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&              object)
{
    return object.intersects(key);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&               object, 
       const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& key_value_pair)
{
    return object.intersects(key_value_pair);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(
      const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::element_type& key_value_pair,
               const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&               object)
{
    return object.intersects(key_value_pair);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&           left, 
       const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& right)
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> MapT;
    if(is_total<MapT>::value)
        return true;

    if(left.iterative_size() < right.iterative_size())
        return Map::key_intersects(right, left);
    else
        return Map::key_intersects(left, right);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(
      const typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& left,
               const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&           right)
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> MapT;
    if(is_total<MapT>::value)
        return true;

    if(left.iterative_size() < right.iterative_size())
        return Map::key_intersects(right, left);
    else
        return Map::key_intersects(left, right);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
bool intersects(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& left, 
                const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& right)
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> MapT;
    if(is_total<MapT>::value)
        return true;

    if(left.iterative_size() < right.iterative_size())
        return Map::intersects(right, left);
    else
        return Map::intersects(left, right);
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc,
          class LeftT, class RightT>
enable_if<mpl::or_<is_same< typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>, LeftT >,
                   is_same< typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>, RightT>
                  >, bool>
is_disjoint(const LeftT& left, const RightT& right)
{
    return !intersects(left, right);
}



//==============================================================================
//= Symmetric difference
//==============================================================================

/** Symmetric subtract map \c x2 and \c *this.
    So \c *this becomes the symmetric difference of \c *this and \c x2 */
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator ^= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
    Map::flip(object, operand); 
    return object; 
}

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator ^  (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>  object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
    return object ^= operand; 
}


//---------------------------------------------------------------------------------
template<class CharType, class CharTraits, 
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
  const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object)
{
    typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
    stream << "{";
    ITL_const_FORALL(typename ObjectT, it, object)
        stream << "(" << it->first << "->" << it->second << ")";

    return stream << "}";
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_interval_container<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_interval_container<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{
    typedef is_interval_splitter type;
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef absorbs_neutrons type;
    BOOST_STATIC_CONSTANT(int, value = Traits::absorbs_neutrons); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_total<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_total type;
    BOOST_STATIC_CONSTANT(int, value = Traits::is_total); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct type_to_string<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{
    static std::string apply()
    {
        return "map<"+ type_to_string<DomainT>::apply()  + ","
                     + type_to_string<CodomainT>::apply() + ","
                     + type_to_string<Traits>::apply() +">"; 
    }
};


}} // namespace itl boost

#endif // BOOST_ITL_MAP_HPP_JOFA_070519

