/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_UTILITY_HPP_JOFA_090203
#define BOOST_VALIDATE_UTILITY_HPP_JOFA_090203

#include <iostream>
#include <boost/itl/type_traits/to_string.hpp>

#define ITL_LOCATION(message) location(__FILE__,__LINE__,message)

#define NEURONIZER_CASES(validater_Template, itl_Map, domain_Type, codomain_Type) \
case NeutronHandlerType::partial_absorber: return new validater_Template<itl_Map<domain_Type,codomain_Type,partial_absorber> >; \
case NeutronHandlerType::partial_enricher: return new validater_Template<itl_Map<domain_Type,codomain_Type,partial_enricher> >; \
case NeutronHandlerType::total_absorber: return new validater_Template<itl_Map<domain_Type,codomain_Type,total_absorber> >; \
case NeutronHandlerType::total_enricher: return new validater_Template<itl_Map<domain_Type,codomain_Type,total_enricher> >;


namespace boost{namespace itl
{
    std::string location(const std::string& file, int line, const std::string& message)
    {
        std::string result = file;
        result += "(" + to_string<int>::apply(line) + "): ";
        result += message;
        return result;
    }

}} // namespace itl boost

#endif // BOOST_VALIDATE_UTILITY_HPP_JOFA_090203

