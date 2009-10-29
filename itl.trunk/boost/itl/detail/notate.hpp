/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
    Macro definitions for some useful notations e.g. iteration headers
-----------------------------------------------------------------------------*/
#ifndef  BOOST_ITL_NOTATE_HPP_JOFA_990119
#define  BOOST_ITL_NOTATE_HPP_JOFA_990119


// Iterations over stl or stl-compatible containers:
#define ITL_FORALL(type,iter,obj) for(type::iterator iter=(obj).begin(); (iter)!=(obj).end(); (iter)++)
#define ITL_const_FORALL(type,iter,obj) for(type::const_iterator iter=(obj).begin(); !((iter)==(obj).end()); (iter)++)

#define ITL_FORALL_THIS(iter) for(iterator iter=begin(); (iter)!=end(); (iter)++)
#define ITL_const_FORALL_THIS(iter) for(const_iterator iter=this->begin(); (iter)!=this->end(); (iter)++)

// Plain old array iteration (assuming member function VecT::size()!)
#define ITL_FORALL_VEC(idx, vec) for(int idx=0; idx<vec.size(); idx++)

namespace boost{namespace itl
{
    const int UNDEFINED_INDEX = -1;
}} // namespace itl boost


#endif // BOOST_ITL_NOTATE_HPP_JOFA_990119


