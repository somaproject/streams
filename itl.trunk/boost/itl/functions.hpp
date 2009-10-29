/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_HPP_JOFA_090803
#define BOOST_ITL_FUNCTIONS_HPP_JOFA_090803

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>

namespace boost{namespace itl
{

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Returns true, if \c left and \c right contain the same elements. 
    Complexity: linear. */
template<class LeftT, class RightT>
typename boost::enable_if<is_intra_combinable<LeftT, RightT>, 
                          bool>::type
is_element_equal(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_equal(left, right);
}

/** Returns true, if \c left is lexicographically less than \c right. 
    Intervals are interpreted as sequence of elements.
    Complexity: linear. */
template<class LeftT, class RightT>
typename boost::enable_if<is_intra_combinable<LeftT, RightT>, 
                          bool>::type
is_element_less(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_less(left, right);
}

/** Returns true, if \c left is lexicographically greater than \c right. 
    Intervals are interpreted as sequence of elements.
    Complexity: linear. */
template<class LeftT, class RightT>
typename boost::enable_if<is_intra_combinable<LeftT, RightT>, 
                          bool>::type
is_element_greater(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_greater(left, right);
}

//------------------------------------------------------------------------------
template<class LeftT, class RightT>
typename boost::enable_if<is_inter_combinable<LeftT, RightT>, 
                          int>::type
inclusion_compare(const LeftT& left, const RightT& right)
{
    return Interval_Set::subset_compare(left, right, 
                                        left.begin(), left.end(),
                                        right.begin(), right.end());
}

template<class LeftT, class RightT>
typename boost::enable_if<is_concept_equivalent<is_element_container,LeftT, RightT>, 
                          int>::type
inclusion_compare(const LeftT& left, const RightT& right)
{
    return Set::subset_compare(left, right, 
                               left.begin(), left.end(),
                               right.begin(), right.end());
}
//------------------------------------------------------------------------------


//==============================================================================
//= Addition
//==============================================================================
/** \par \b Requires: \c OperandT is an interval container addable to \c ObjectT. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity: loglinear */
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{
    typename ObjectT::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        prior_ = object.add(prior_, *elem_); 

    return object; 
}

/* \par \b Requires: \c OperandT is an addable derivative type of \c ObjectT. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                    
                   \         element     segment 
ObjectT:
       interval container    O(log n)     O(n)   

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
\endcode

For the addition of \b elements or \b segments
complexity is \b logarithmic or \b linear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{ 
    return object.add(operand); 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}


/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT>
ObjectT operator + (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//------------------------------------------------------------------------------
//- Addition |=, | 
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c ObjectT and \c OperandT are addable.
    \par \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element     segment   container
ObjectT:
       interval container    O(log n)     O(n)     O(m log(n+m))

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the addition of \b elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
operator |= (ObjectT& object, const OperandT& operand)
{ 
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT>
ObjectT operator | (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//==============================================================================
//= Subtraction
//==============================================================================
//------------------------------------------------------------------------------
//- Subtraction -=, -
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c ObjectT and \c OperandT are subtractable.
    \par \b Effects: \c operand is subtracted from \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element    segment    container
ObjectT:
       interval container    O(log n)     O(n)     O(m log(n+m))

                                       amortized
            interval_sets               O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the subtraction of \em elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For interval sets subtraction of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_concept_equivalent<is_interval_map, ObjectT, OperandT>, 
                          ObjectT>::type& 
operator -=(ObjectT& object, const OperandT& operand)
{
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        object.subtract(*elem_); 

    return object; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
    return object.subtract(operand); 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_cross_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
    return object.erase(operand); 
}

template<class ObjectT, class IntervalSetT>
typename boost::enable_if<combines_right_to_interval_set<ObjectT, IntervalSetT>,
                          ObjectT>::type&
operator -= (ObjectT& object, const IntervalSetT& operand)
{
    return erase(object, operand);
}


template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator - (ObjectT object, const OperandT& operand)
{
    return object -= operand; 
}

//==============================================================================
//= Insertion
//==============================================================================
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
insert(ObjectT& object, const OperandT& operand)
{
    typename ObjectT::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        object.insert(*elem_); 

    return object; 
}


//==============================================================================
//= Erasure
//==============================================================================
template<class ObjectT, class OperandT>
typename boost::enable_if<combines_right_to_interval_container<ObjectT, OperandT>,
                          ObjectT>::type&
erase(ObjectT& object, const OperandT& operand)
{
    if(operand.empty())
        return object;

    typename OperandT::const_iterator common_lwb;
    typename OperandT::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object;

    typename OperandT::const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        object.erase(*it_++);

    return object; 
}


//==============================================================================
//= Intersection
//==============================================================================
//------------------------------------------------------------------------------
//- Intersection &=, &
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type&
operator &= (ObjectT& object, const OperandT& operand)
{
    ObjectT intersection;
    object.add_intersection(intersection, operand);
    object.swap(intersection);
    return object;
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (ObjectT object, const OperandT& operand)
{
    return object &= operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (const OperandT& operand, ObjectT object)
{
    return object &= operand; 
}

template<class ObjectT>
ObjectT operator & (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object &= operand; 
}

//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------
template<class LeftT, class RightT>
typename boost::enable_if<is_intra_combinable<LeftT, RightT>, 
                          bool>::type
intersects(const LeftT& left, const RightT& right)
{
    if(mpl::or_<is_total<LeftT>, is_total<RightT> >::value)
        return true;

    LeftT intersection;

    typename RightT::const_iterator right_common_lower_;
    typename RightT::const_iterator right_common_upper_;

    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename RightT::const_iterator it_ = right_common_lower_;
    while(it_ != right_common_upper_)
    {
        left.add_intersection(intersection, *it_++);
        if(!intersection.empty())
            return true;
    }

    return false; 
}

template<class LeftT, class RightT>
typename boost::enable_if<is_cross_combinable<LeftT, RightT>, 
                          bool>::type
intersects(const LeftT& left, const RightT& right)
{
    LeftT intersection;

    if(left.empty() || right.empty())
        return false;

    typename RightT::const_iterator right_common_lower_;
    typename RightT::const_iterator right_common_upper_;

    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename RightT::const_iterator it_ = right_common_lower_;
    while(it_ != right_common_upper_)
    {
        left.add_intersection(intersection, RightT::key_value(it_++));
        if(!intersection.empty())
            return true;
    }

    return false; 
}

template<class Type, class AssociateT>
typename boost::enable_if<is_inter_derivative<Type, AssociateT>, 
                          bool>::type
intersects(const Type& left, const AssociateT& right)
{
    return left.intersects(right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: loglinear, if \c left and \c right are interval containers. */
template<class LeftT, class RightT>
typename boost::enable_if<is_inter_combinable<LeftT, RightT>, 
                          bool>::type
is_disjoint(const LeftT& left, const RightT& right)
{
    return !intersects(left, right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: logarithmic, if \c AssociateT is an element type \c Type::element_type. 
    linear, if \c AssociateT is a segment type \c Type::segment_type. */
template<class Type, class AssociateT>
typename boost::enable_if<is_inter_derivative<Type, AssociateT>, 
                          bool>::type
is_disjoint(const Type& left, const AssociateT& right)
{
    return !left.intersects(right);
}


//==============================================================================
//= Symmetric difference
//==============================================================================
//------------------------------------------------------------------------------
//- Symmetric difference ^=, ^
//------------------------------------------------------------------------------


template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ 
    return object.flip(operand); 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ 
    return object.flip(operand); 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (ObjectT object, const OperandT& operand)
{
    return object ^= operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (const OperandT& operand, ObjectT object)
{
    return object ^= operand; 
}


template<class ObjectT>
ObjectT operator ^ (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object ^= operand; 
}


//-----------------------------------------------------------------------------
// hull
//-----------------------------------------------------------------------------
template<class ObjectT>
typename boost::enable_if<is_interval_container<ObjectT>, 
                          typename ObjectT::interval_type>::type
hull(const ObjectT& object)
{
    return 
        object.empty() ? neutron<typename ObjectT::interval_type>::value()
        : (ObjectT::key_value(object.begin()))
            .span(ObjectT::key_value(object.rbegin()));
}


}} // namespace itl boost

#endif


