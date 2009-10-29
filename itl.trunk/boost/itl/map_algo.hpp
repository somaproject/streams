/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_MAPALGO_HPP_JOFA_080225
#define BOOST_ITL_MAPALGO_HPP_JOFA_080225

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/set_algo.hpp>

namespace boost{namespace itl
{
namespace Map 
{

template<class MapType>
bool contained_in(const MapType& sub, const MapType& super)
{
    if(&super == &sub)                   return true;
    if(sub.empty())                      return true;
    if(super.empty())                    return false;
    if(super.size()    < sub.size()    ) return false;
    if(*sub.begin()    < *super.begin()) return false;
    if(*super.rbegin() < *sub.rbegin() ) return false;

    typename MapType::const_iterator common_lwb_;
    typename MapType::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sub, super))
        return false;

    typename MapType::const_iterator sub_ = sub.begin(), super_;
    while(sub_ != sub.end())
    {
        super_ = super.find((*sub_).first);
        if(super_ == super.end()) 
            return false;
        else if(!(sub_->second == super_->second))
            return false;
        sub_++;
    }
    return true;
}

template<class MapType>
bool contained_in(const typename MapType::set_type& sub, const MapType& super)
{
    typedef typename MapType::set_type SetType;

    if(sub.empty())                      return true;
    if(super.empty())                    return false;
    if(super.size()    < sub.size()    ) return false;
    if(*sub.begin()    < *super.begin()) return false;
    if(*super.rbegin() < *sub.rbegin() ) return false;

    typename SetType::const_iterator common_lwb_;
    typename SetType::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sub, super))
        return false;

    typename SetType::const_iterator sub_ = sub.begin();
    typename MapType::const_iterator super_;
    while(sub_ != sub.end())
    {
        super_ = super.find(*sub_++);
        if(super_ == super.end()) 
            return false;
    }
    return true;
}


template <class MapType>
bool intersects(const MapType& left, const MapType& right)
{
    typename MapType::const_iterator right_common_lower_;
    typename MapType::const_iterator right_common_upper_;
    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename MapType::const_iterator right_ = right_common_lower_;
    while(right_ != right_common_upper_)
        if(left.intersects(*right_++))
            return true;

    return false;
}

template <class ObjectT, class CoObjectT>
bool key_intersects(const ObjectT& left, const CoObjectT& right)
{
    typename CoObjectT::const_iterator right_common_lower_;
    typename CoObjectT::const_iterator right_common_upper_;
    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename CoObjectT::const_iterator right_ = right_common_lower_;
    while(right_ != right_common_upper_)
        if(left.intersects(CoObjectT::key_value(right_++)))
            return true;

    return false;
}

//----------------------------------------------------------------------
// flip
//----------------------------------------------------------------------
template<class MapType>
void flip(MapType& result, const MapType& x2)
{
    if(mpl::and_<is_total<MapType>, absorbs_neutrons<MapType> >::value)
    {
        result.clear();
        return;
    }

    typename MapType::const_iterator x2_ = x2.begin(), cur_x2_, x1_;
    while(x2_ != x2.end()) 
    {
        cur_x2_ = x2_;
        std::pair<typename MapType::iterator,bool> insertion 
            = result.insert(*x2_++);
        if(!insertion.second)
        {
            //result.erase(insertion.first);
            if(is_set<typename MapType::codomain_type>::value)
            {
                typename MapType::iterator res_ = insertion.first;
                typename MapType::codomain_type common_value = res_->second;
                typename MapType::key_type key_value = res_->first;
                typename MapType::inverse_codomain_intersect()(common_value, cur_x2_->second);
                result.subtract(*res_);
                result.add(typename MapType::value_type(key_value, common_value));
            }
            else
                result.subtract(*insertion.first);
        }
    }

    if(mpl::and_<is_total<MapType>, mpl::not_<absorbs_neutrons<MapType> > >::value)
        ITL_FORALL(typename MapType, it_, result)
            it_->second = neutron<typename MapType::codomain_type>::value();
}



template<class MapType>
typename MapType::const_iterator next_proton(typename MapType::const_iterator& iter_, const MapType& object)
{
    while(   iter_ != object.end() 
          && iter_->second == neutron<typename MapType::codomain_type>::value())
        ++iter_;

    return iter_;
}

/** Function template <tt>lexicographical_equal</tt> implements 
lexicographical equality except for neutronic content values. */
template<class MapType>
bool lexicographical_protonic_equal(const MapType& left, const MapType& right)
{
    if(&left == &right)        
        return true;

    typename MapType::const_iterator left_  = left.begin();
    typename MapType::const_iterator right_ = right.begin();

    left_  = next_proton(left_,  left);
    right_ = next_proton(right_, right);

    while(left_ != left.end() && right_ != right.end())
    {
        if(!(left_->first == right_->first && left_->second == right_->second))
            return false;

        ++left_;
        ++right_;
        left_  = next_proton(left_,  left);
        right_ = next_proton(right_, right);
    }

    return left_ == left.end() && right_ == right.end();
}

} // namespace Map
}} // namespace boost itl

#endif

