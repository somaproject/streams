/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_MORPHISM_HPP_JOFA_080315
#define BOOST_ITL_INTERVAL_MORPHISM_HPP_JOFA_080315

#include <boost/itl/detail/notate.hpp>
namespace boost{namespace itl
{
    namespace Interval
    {
        template <typename ElementContainerT, typename IntervalContainerT>
        void atomize(ElementContainerT& result, const IntervalContainerT& src)
        {
            ITL_const_FORALL(typename IntervalContainerT, itv_, src)
            {
                const typename IntervalContainerT::key_type& itv   = IntervalContainerT::key_value(itv_);
                typename IntervalContainerT::codomain_type   coval = IntervalContainerT::codomain_value(itv_);

                for(typename IntervalContainerT::domain_type element = itv.first(); element <= itv.last(); ++element)
                {
                    result.insert(ElementContainerT::make_element(element, coval));
                }
            }
        }

        template <typename IntervalContainerT, typename ElementContainerT>
        void cluster(IntervalContainerT& result, const ElementContainerT& src)
        {
            ITL_const_FORALL(typename ElementContainerT, element_, src)
            {
                const typename ElementContainerT::key_type&  key  = ElementContainerT::key_value(element_);
                const typename ElementContainerT::data_type& data = ElementContainerT::data_value(element_);

                result += IntervalContainerT::make_domain_element(key, data);
            }
        }

        template <typename AtomizedType, typename ClusteredType>
        struct Atomize
        {
            void operator()(AtomizedType& atomized, const ClusteredType& clustered)
            {
                Interval::atomize(atomized, clustered);
            }
        };

        template <typename ClusteredType, typename AtomizedType>
        struct Cluster
        {
            void operator()(ClusteredType& clustered, const AtomizedType& atomized)
            {
                Interval::cluster(clustered, atomized);
            }
        };

        template <typename JointType, typename SplitType>
        struct Join
        {
            void operator()(JointType& joint, SplitType& split)
            {
                split.join();
                ITL_FORALL(typename SplitType, split_, split)
                    joint.insert(*split_);
            }
        };

        template <typename AbsorberType, typename EnricherType>
        struct AbsorbNeutrons
        {
            void operator()(AbsorberType& absorber, EnricherType& enricher)
            {
                enricher.absorb_neutrons();
                ITL_FORALL(typename EnricherType, enricher_, enricher)
                    absorber.insert(*enricher_);
            }
        };

    } // namespace Interval


    template<> 
    inline std::string binary_template_to_string<Interval::Atomize>::apply() { return "@"; }
    template<> 
    inline std::string binary_template_to_string<Interval::Cluster>::apply() { return "&"; }
    template<> 
    inline std::string binary_template_to_string<Interval::Join>::apply() { return "j"; }
    template<> 
    inline std::string binary_template_to_string<Interval::AbsorbNeutrons>::apply() { return "a0"; }
}} // namespace boost itl

#endif



