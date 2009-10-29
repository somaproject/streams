/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------
class point_gentor
--------------------------------------------------------------------*/
#ifndef __POINT_GENTOR_H_JOFA_000714__
#define __POINT_GENTOR_H_JOFA_000714__

#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/seqgentor.hpp>

namespace boost{namespace itl
{

//toy point.
template<class DomainT>
struct point
{
    std::string as_string()const
    {
        return std::string(
        "(" + to_string<DomainT>::apply(x) + "," + 
              to_string<DomainT>::apply(y) + ")"
        );
    }

    DomainT x;
    DomainT y;
};

template<class DomainT>
bool operator == (const point<DomainT>& left, const point<DomainT>& right)
{
    return left.x == right.x && left.y == right.y;
}

template<class CharType, class CharTraits, class DomainT>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, const point<DomainT>& object)
{
    return stream << object.as_string();
}

template <class Type>
struct type_to_string<itl::point<Type> >
{
    static std::string apply()
    { return "point<"+ type_to_string<Type>::apply() +">"; }
};

template <class DomainT, class PointT = point<DomainT> > 
class point_gentor: public RandomGentorAT<PointT>
{
public:
    virtual void some(PointT& x);

    void setRange(const itl::interval<DomainT>& range)
    { m_valueRange = range; }

    void setValueRange(DomainT low, DomainT up)
    { m_valueRange.set(low,up, itl::right_open); }

    void setMaxIntervalLength(DomainT len) { m_maxIntervalLength=len; }
    void setProbDerivation();

private:
    NumberGentorT<DomainT> m_DomainTGentor;

    interval<DomainT>    m_valueRange;
    DomainT              m_maxIntervalLength;
};


template <class DomainT, class PointT>
void point_gentor<DomainT, PointT>::some(PointT& value)
{
    value.x = m_DomainTGentor(m_valueRange);
    value.y = m_DomainTGentor(m_valueRange);
};

}} // namespace itl boost

#endif


