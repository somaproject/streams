/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_COLLECTOR_DRIVER_HPP_JOFA_091009
#define BOOST_VALIDATE_DRIVER_COLLECTOR_DRIVER_HPP_JOFA_091009

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/validate/validater/collector_validater.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
    class collector_driver : public itl_driver
    {
    public:
        collector_driver() { setProfile(); }

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
            _domainChoice[DomainType::Int]               = 100;
            _domainChoice[DomainType::Double]            = 0;
            setDomainTypeNames();
            _domainChoice.init();

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Int]           = 0;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 100;
            setCodomainTypeNames();
            _codomainChoice.init();

            _neutronizerChoice.setSize(NeutronHandlerType::NeutronHandlerTypes_size);
            _neutronizerChoice.setMaxWeights(100);
            _neutronizerChoice[NeutronHandlerType::partial_absorber] = 50;
            _neutronizerChoice[NeutronHandlerType::partial_enricher] = 50;
            _neutronizerChoice[NeutronHandlerType::total_absorber]   = 0;
            _neutronizerChoice[NeutronHandlerType::total_enricher]   = 0;
            setNeutronHandlerTypeNames();
            _neutronizerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("collector_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("collector_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("collector_driver::setProfile()") << std::endl;
            }

            if(!_neutronizerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _neutronizerChoice.inconsitencyMessage("collector_driver::setProfile()") << std::endl;
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
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new collector_validater<itl::map<int,itl::set<int> > >;
                case NeutronHandlerType::partial_enricher: return new collector_validater<itl::map<int,itl::set<int>,partial_enricher> >;
                //case NeutronHandlerType::total_absorber : return new collector_validater<itl::map<int,itl::set<int>,total_absorber > >;
                default: return choiceError(ITL_LOCATION("\nRootType::itl_map: neutronizerChoice:\n"), neutronizerChoice, _neutronizerChoice);
                }//switch neutronizerChoice
            }//case itl_map 
            //-----------------------------------------------------------------
            case RootType::interval_map: {
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new collector_validater<interval_map<int,itl::set<int> > >;
                case NeutronHandlerType::partial_enricher: return new collector_validater<interval_map<int,itl::set<int>,partial_enricher> >;
                //case NeutronHandlerType::total_absorber : return new collector_validater<interval_map<int,itl::set<int>,total_absorber > >;
                default: return choiceError(ITL_LOCATION("\nRootType::interval_map: neutronizerChoice:\n"), neutronizerChoice, _neutronizerChoice);
                }//switch neutronizerChoice
            }//case interval_map 
            //-----------------------------------------------------------------
            case RootType::split_interval_map: {
                switch(neutronizerChoice) {
                case NeutronHandlerType::partial_absorber: return new collector_validater<split_interval_map<int,itl::set<int> > >;
                case NeutronHandlerType::partial_enricher: return new collector_validater<split_interval_map<int,itl::set<int>,partial_enricher> >;
                //case NeutronHandlerType::total_absorber : return new collector_validater<split_interval_map<int,itl::set<int>,total_absorber > >;
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

#endif // BOOST_VALIDATE_DRIVER_COLLECTOR_DRIVER_HPP_JOFA_091009
