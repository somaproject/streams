/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_SET_COPY_CONFORMITY_DRIVER_HPP_JOFA_091013
#define BOOST_VALIDATE_DRIVER_SET_COPY_CONFORMITY_DRIVER_HPP_JOFA_091013

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/itl_xt/list.hpp>
#include <boost/validate/validater/function_equality_validater.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
    class set_copy_conformity_driver : public itl_driver
    {
    public:
        set_copy_conformity_driver() { setProfile(); }


        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::itl_set]               = 25;
            _rootChoice[RootType::interval_set]          = 25;
            _rootChoice[RootType::separate_interval_set] = 25;
            _rootChoice[RootType::split_interval_set]    = 25;
            _rootChoice[RootType::itl_map]               = 0;
            _rootChoice[RootType::interval_map]          = 0;
            _rootChoice[RootType::split_interval_map]    = 0;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 100;
            _domainChoice[DomainType::Double]            = 0;  
            setDomainTypeNames();                              
            _domainChoice.init();                              

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Int]           = 100;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 0;
            setCodomainTypeNames();
            _codomainChoice.init();

            _neutronizerChoice.setSize(NeutronHandlerType::NeutronHandlerTypes_size);
            _neutronizerChoice.setMaxWeights(100);
            _neutronizerChoice[NeutronHandlerType::partial_absorber]      = 25;
            _neutronizerChoice[NeutronHandlerType::partial_enricher]      = 25;
            _neutronizerChoice[NeutronHandlerType::total_absorber]        = 25;
            _neutronizerChoice[NeutronHandlerType::total_enricher]        = 25;
            setNeutronHandlerTypeNames();
            _neutronizerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("set_copy_conformity_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("set_copy_conformity_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("set_copy_conformity_driver::setProfile()") << std::endl;
            }

            if(!_neutronizerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _neutronizerChoice.inconsitencyMessage("set_copy_conformity_driver::setProfile()") << std::endl;
            }

        }

        algebra_validater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            int neutronizerChoice  = _neutronizerChoice.some();

            switch(rootChoice)
            {
            //-----------------------------------------------------------------
            case RootType::itl_set:               return new function_equality_validater<itl::list<int>,                 itl::set<int> >;
            case RootType::interval_set:          return new function_equality_validater<itl::list<itl::interval<int> >, interval_set<int> >;
            case RootType::separate_interval_set: return new function_equality_validater<itl::list<itl::interval<int> >, separate_interval_set<int> >;
            case RootType::split_interval_set:    return new function_equality_validater<itl::list<itl::interval<int> >, split_interval_set<int> >;
            //-----------------------------------------------------------------
            default: return choiceError(ITL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

            return NULL; //just to please the compiler ;)
        }

    };


}} // namespace itl boost

#endif // BOOST_VALIDATE_DRIVER_SET_COPY_CONFORMITY_DRIVER_HPP_JOFA_091013
