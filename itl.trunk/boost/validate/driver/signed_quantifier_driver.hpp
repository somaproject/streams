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
#include <boost/validate/validater/signed_quantifier_validater.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
    class signed_quantifier_driver : public itl_driver
    {
    public:
        signed_quantifier_driver() { setProfile(); }


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
            _domainChoice[DomainType::Int]               = 0;
            _domainChoice[DomainType::Double]            = 100;
            setDomainTypeNames();
            _domainChoice.init();

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Nat]           = 0;
            _codomainChoice[CodomainType::Int]           = 100;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 0;
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
                std::cout << _rootChoice.inconsitencyMessage("signed_quantifier_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("signed_quantifier_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("signed_quantifier_driver::setProfile()") << std::endl;
            }

            if(!_neutronizerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _neutronizerChoice.inconsitencyMessage("signed_quantifier_driver::setProfile()") << std::endl;
            }

        }


        algebra_validater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            //int domainChoice       = _domainChoice.some(); // not used
            int codomainChoice     = _codomainChoice.some();
            int neutronizerChoice  = _neutronizerChoice.some();

            switch(rootChoice)
            {
            //-----------------------------------------------------------------
            case RootType::itl_map: {
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new signed_quantifier_validater<itl::map<int,   int,partial_absorber> >;
                case NeutronHandlerType::partial_enricher: return new signed_quantifier_validater<itl::map<int,   int,partial_enricher> >;
                case NeutronHandlerType::total_absorber:   return new signed_quantifier_validater<itl::map<double,int,total_absorber  > >;
                case NeutronHandlerType::total_enricher:   return new signed_quantifier_validater<itl::map<double,int,total_enricher  > >;
                default: return choiceError(ITL_LOCATION("\nRootType::itl_map: neutronizerChoice:\n"), neutronizerChoice, _neutronizerChoice);
                }//switch neutronizerChoice
            }//case itl_map 
            //-----------------------------------------------------------------
            case RootType::interval_map: {
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new signed_quantifier_validater<interval_map<double,int,partial_absorber> >;
                case NeutronHandlerType::partial_enricher: return new signed_quantifier_validater<interval_map<double,int,partial_enricher> >;
                case NeutronHandlerType::total_absorber:   return new signed_quantifier_validater<interval_map<int,   int,total_absorber  > >;
                case NeutronHandlerType::total_enricher:   return new signed_quantifier_validater<interval_map<int,   int,total_enricher  > >;
                default: return choiceError(ITL_LOCATION("\nRootType::interval_map: neutronizerChoice:\n"), neutronizerChoice, _neutronizerChoice);
                }//switch neutronizerChoice
            }//case interval_map 
            //-----------------------------------------------------------------
            case RootType::split_interval_map: {
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new signed_quantifier_validater<split_interval_map<double,int,partial_absorber> >;
                case NeutronHandlerType::partial_enricher: return new signed_quantifier_validater<split_interval_map<int,   int,partial_enricher> >;
                case NeutronHandlerType::total_absorber:   return new signed_quantifier_validater<split_interval_map<double,int,total_absorber  > >;
                case NeutronHandlerType::total_enricher:   return new signed_quantifier_validater<split_interval_map<int,   int,total_enricher  > >;
                default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: neutronizerChoice:\n"), neutronizerChoice, _neutronizerChoice);
                }//switch neutronizerChoice
            }//case split_interval_map 
            //-----------------------------------------------------------------

            default: return choiceError(ITL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

            return NULL; //just to please the compiler ;)
        }

    };


}} // namespace itl boost

