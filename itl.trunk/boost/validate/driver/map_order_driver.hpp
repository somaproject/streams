/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <stdio.h>
#include <boost/validate/utility.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/validater/itl_order_validater.hpp>

namespace boost{namespace itl
{
    
    class map_order_driver : public itl_driver
    {
    public:
        map_order_driver() { setProfile(); }

        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::itl_set]               = 0;
            _rootChoice[RootType::interval_set]          = 0;
            _rootChoice[RootType::separate_interval_set] = 0;
            _rootChoice[RootType::split_interval_set]    = 0;
            _rootChoice[RootType::itl_map]               = 33;
            _rootChoice[RootType::interval_map]          = 33;
            _rootChoice[RootType::split_interval_map]    = 34;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 50;
            _domainChoice[DomainType::Double]            = 50;
            setDomainTypeNames();
            _domainChoice.init();

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Nat]           = 0;
            _codomainChoice[CodomainType::Int]           = 50;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 50;
            setCodomainTypeNames();
            _codomainChoice.init();

            _neutronizerChoice.setSize(NeutronHandlerType::NeutronHandlerTypes_size);
            _neutronizerChoice.setMaxWeights(100);
            _neutronizerChoice[NeutronHandlerType::partial_absorber]    = 50;
            _neutronizerChoice[NeutronHandlerType::partial_enricher]    = 50;
            _neutronizerChoice[NeutronHandlerType::total_absorber]      = 0;
            _neutronizerChoice[NeutronHandlerType::total_enricher]      = 0;
            setNeutronHandlerTypeNames();
            _neutronizerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("map_order_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("map_order_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("map_order_driver::setProfile()") << std::endl;
            }

            if(!_neutronizerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _neutronizerChoice.inconsitencyMessage("map_order_driver::setProfile()") << std::endl;
            }

        }

        algebra_validater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            int domainChoice       = _domainChoice.some();
            int codomainChoice     = _codomainChoice.some();
            int neutronizerChoice  = _neutronizerChoice.some();

            switch(rootChoice)
            {
            //-----------------------------------------------------------------
            case RootType::itl_map: {
                switch(domainChoice) {
                case DomainType::Int: 
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<itl::map<int,nat,total_enricher> >;
                    case CodomainType::Int:     return new itl_order_validater<itl::map<int,int,partial_absorber> >;
                    case CodomainType::set_int: return new itl_order_validater<itl::map<int,itl::set<int>,partial_enricher> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }//switch codomain

                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<itl::map<double,nat,partial_enricher> >; 
                    case CodomainType::Int:     return new itl_order_validater<itl::map<double,int,total_absorber> >; 
                    case CodomainType::set_int: return new itl_order_validater<itl::map<double,itl::set<int>,partial_absorber> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }//switch codomain

                default: return choiceError(ITL_LOCATION("\nRootType::itl_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case itl_map 
            //-----------------------------------------------------------------
            case RootType::interval_map: {
                switch(domainChoice) {
                case DomainType::Int:
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<interval_map<int,nat,partial_absorber> >; 
                    case CodomainType::Int:     return new itl_order_validater<interval_map<int,int,total_enricher> >; 
                    case CodomainType::set_int: return new itl_order_validater<interval_map<int,itl::set<int>,total_absorber> >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }// switch codomain
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<interval_map<double,nat,total_absorber> >; 
                    case CodomainType::Int:     return new itl_order_validater<interval_map<double,int,partial_enricher> >; 
                    case CodomainType::set_int: return new itl_order_validater<interval_map<double,itl::set<int>,total_enricher> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }// switch codomain
                default: return choiceError(ITL_LOCATION("\nRootType::interval_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case interval_map
            //-----------------------------------------------------------------
            case RootType::split_interval_map: {
                switch(domainChoice) {
                case DomainType::Int:
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<split_interval_map<int,nat,total_enricher> >; 
                    case CodomainType::Int:     return new itl_order_validater<split_interval_map<int,int,partial_absorber> >; 
                    case CodomainType::set_int: return new itl_order_validater<split_interval_map<int,itl::set<int>,partial_enricher> >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Nat:     return new itl_order_validater<split_interval_map<double,nat,partial_enricher> >; 
                    case CodomainType::Int:     return new itl_order_validater<split_interval_map<double,int,total_absorber> >; 
                    case CodomainType::set_int: return new itl_order_validater<split_interval_map<double,itl::set<int>,partial_absorber> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                }
                default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case split_interval_map
            //-----------------------------------------------------------------

            default: return choiceError(ITL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

            return NULL; //just to please the compiler ;)
        }

    };


}} // namespace itl boost

