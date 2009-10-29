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
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/validater/interval_morphic_validater.hpp>

namespace boost{namespace itl
{
    
class itl_morphic_driver : public itl_driver
{
public:
    itl_morphic_driver() { setProfile(); }

    void setProfile()
    {
        setValid(true);
        _rootChoice.setSize(RootType::Types_size);
        _rootChoice.setMaxWeights(100);
        _rootChoice[RootType::itl_set]               = 0;
        _rootChoice[RootType::interval_set]          = 20;
        _rootChoice[RootType::separate_interval_set] = 20;
        _rootChoice[RootType::split_interval_set]    = 20;
        _rootChoice[RootType::itl_map]               = 0;
        _rootChoice[RootType::interval_map]          = 20;
        _rootChoice[RootType::split_interval_map]    = 20;
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
        _codomainChoice[CodomainType::Nat]           = 33;
        _codomainChoice[CodomainType::Int]           = 33;
        _codomainChoice[CodomainType::Double]        = 0;
        _codomainChoice[CodomainType::set_int]       = 34;
        setCodomainTypeNames();
        _codomainChoice.init();

        _neutronizerChoice.setSize(NeutronHandlerType::NeutronHandlerTypes_size);
        _neutronizerChoice.setMaxWeights(100);
        _neutronizerChoice[NeutronHandlerType::partial_absorber]    = 25;
        _neutronizerChoice[NeutronHandlerType::partial_enricher]    = 25;
        _neutronizerChoice[NeutronHandlerType::total_absorber]      = 25;
        _neutronizerChoice[NeutronHandlerType::total_enricher]      = 25;
        setNeutronHandlerTypeNames();
        _neutronizerChoice.init();

        if(!_rootChoice.is_consistent())
        {
            setValid(false);
            std::cout << _rootChoice.inconsitencyMessage("itl_morphic_driver::setProfile()") << std::endl;
        }

        if(!_domainChoice.is_consistent())
        {
            setValid(false);
            std::cout << _domainChoice.inconsitencyMessage("itl_morphic_driver::setProfile()") << std::endl;
        }

        if(!_codomainChoice.is_consistent())
        {
            setValid(false);
            std::cout << _codomainChoice.inconsitencyMessage("itl_morphic_driver::setProfile()") << std::endl;
        }

        if(!_neutronizerChoice.is_consistent())
        {
            setValid(false);
            std::cout << _neutronizerChoice.inconsitencyMessage("itl_morphic_driver::setProfile()") << std::endl;
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
        // Sets
        //-----------------------------------------------------------------
        case RootType::interval_set:          return new interval_morphic_validater<interval_set<int> >;
        case RootType::separate_interval_set: return new interval_morphic_validater<separate_interval_set<int> >;
        case RootType::split_interval_set:    return new interval_morphic_validater<split_interval_set<int> >;
        //-----------------------------------------------------------------
        // Maps
        //-----------------------------------------------------------------
        case RootType::split_interval_map: {
            switch(domainChoice) {
            case DomainType::Int: 
                switch(neutronizerChoice) {
                NEURONIZER_CASES(interval_morphic_validater, split_interval_map, int, int)
                default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: neutronizerChoice:\n"), 
                                            neutronizerChoice, _neutronizerChoice);
                }
            default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"), 
                                         domainChoice, _domainChoice);
            }
        }
        case RootType::interval_map: {
            switch(domainChoice) {
            case DomainType::Int: 
                switch(neutronizerChoice) {
                NEURONIZER_CASES(interval_morphic_validater, interval_map, int, int)
                default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"), 
                                            domainChoice, _domainChoice);
                }
            }
        }
        default: return choiceError(ITL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
        } //switch(rootChoice)

        return NULL; //just to please the compiler ;)
    }

};


}} // namespace itl boost

