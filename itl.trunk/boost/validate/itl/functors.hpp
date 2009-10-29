/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
Auxiliary functors that are used to validate the equality of
handcoded loops and calls of std::algorithms.
+-----------------------------------------------------------------------------*/

#ifndef BOOST_VALIDATE_ITL_FUNCTORS_HPP_JOFA_091004
#define BOOST_VALIDATE_ITL_FUNCTORS_HPP_JOFA_091004

#include <boost/itl/iterator.hpp>

namespace boost{namespace itl
{

template <typename SourceT, typename TargetT>
struct base_insertion
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        ITL_const_FORALL(typename SourceT, item_, items)
            collected.insert(*item_);
    }
};

template<> 
inline std::string binary_template_to_string<base_insertion>::apply() { return "_i"; }

template <typename SourceT, typename TargetT>
struct hint_insertion
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        typename TargetT::iterator prior_ = collected.end();
        ITL_const_FORALL(typename SourceT, item_, items)
            prior_ = collected.insert(prior_, *item_);
    }
};

template<> 
inline std::string binary_template_to_string<hint_insertion>::apply() { return "!i"; }

template <typename SourceT, typename TargetT>
struct copy_insertion
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        std::copy(items.begin(), items.end(), itl::inserter(collected, collected.end()));
    }
};

template<> 
inline std::string binary_template_to_string<copy_insertion>::apply() { return "ci"; }


template <typename SourceT, typename TargetT>
struct base_addition
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        ITL_const_FORALL(typename SourceT, item_, items)
            collected.add(*item_);
    }
};

template<> 
inline std::string binary_template_to_string<base_addition>::apply() { return "_+"; }

template <typename SourceT, typename TargetT>
struct hint_addition
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        typename TargetT::iterator prior_ = collected.end();
        ITL_const_FORALL(typename SourceT, item_, items)
            prior_ = collected.add(prior_, *item_);
    }
};

template<> 
inline std::string binary_template_to_string<hint_addition>::apply() { return "!+"; }

template <typename SourceT, typename TargetT>
struct copy_addition
{
    void operator()(TargetT& collected, const SourceT& items)
    {
        collected.clear();
        std::copy(items.begin(), items.end(), itl::adder(collected, collected.end()));
    }
};

template<> 
inline std::string binary_template_to_string<copy_addition>::apply() { return "c+"; }

}} // namespace itl boost

#endif // BOOST_VALIDATE_FUNCTORS_HPP_JOFA_091004