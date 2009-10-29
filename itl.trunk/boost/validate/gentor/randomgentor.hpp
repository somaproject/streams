/*-----------------------------------------------------------------------------+    
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once


#include <limits>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/bits_gentor.hpp>
#include <boost/itl_xt/setgentor.hpp>
#include <boost/itl_xt/mapgentor.hpp>
#include <boost/itl_xt/seqgentor.hpp>
#include <boost/itl_xt/itvgentor.hpp>
#include <boost/itl_xt/std/pair_gentor.hpp>
#include <boost/itl_xt/map_segment_gentor.hpp>

#ifdef LAW_BASED_TEST_BOOST_POLYGON
#include <libs/validate/example/labat_polygon_/point_gentor.hpp>
#include <libs/validate/example/labat_polygon_/polygon_gentor.hpp>
#endif //LAW_BASED_TEST_BOOST_POLYGON

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#ifdef USE_ITL_TREE
#include <boost/itl/tree/tree.hpp>
#endif
#include <boost/validate/gentor/gentorprofile.hpp>


namespace boost{namespace itl
{

    // -------------------------------------------------------------------------
    template <class ValueT> class RandomGentor;
    template <> class RandomGentor<int> : public NumberGentorT<int> {};
    template <> class RandomGentor<nat> : public NumberGentorT<nat> {};
    template <> class RandomGentor<double> : public NumberGentorT<double> {};

    // -------------------------------------------------------------------------
    template <class NaturalT> 
    class RandomGentor<itl::bits<NaturalT> > : 
        public bits_gentor<itl::bits<NaturalT> > {};

    // -------------------------------------------------------------------------
    template <class DomainT>
    class RandomGentor<itl::interval<DomainT> > :
        public ItvGentorT<DomainT> {};

    // -------------------------------------------------------------------------
    template <class DomainT, class CodomainT>
    class RandomGentor< std::pair<DomainT,CodomainT> > :
        public std_pair_gentor<DomainT,CodomainT> {};

#ifdef LAW_BASED_TEST_BOOST_POLYGON
    // -------------------------------------------------------------------------
    template <class DomainT> 
    class RandomGentor<itl::point<DomainT> > :
        public point_gentor<DomainT> {};

    template <class PointT> 
    class RandomGentor<itl::list<PointT> > :
        public polygon_gentor<itl::list<PointT> > {};

    template <class PointT> 
    class RandomGentor<itl::list<itl::list<PointT> > > :
        public polygon_set_gentor<itl::list<itl::list<PointT> > > {};
#endif //LAW_BASED_TEST_BOOST_POLYGON

    // ----- lists -------------------------------------------------------------
    template <class DomainT, class CodomainT> 
    class RandomGentor<std::pair<itl::interval<DomainT>, CodomainT> > :
        public map_segment_gentor<DomainT,CodomainT> {};

    template <class DomainT, class CodomainT> 
    class RandomGentor<itl::list<std::pair<itl::interval<DomainT>, CodomainT> > > :
        public SeqGentorT<itl::list<std::pair<itl::interval<DomainT>, CodomainT> > > {};

    template <class DomainT, class CodomainT> 
    class RandomGentor<itl::list<std::pair<DomainT,CodomainT> > > :
        public SeqGentorT<itl::list<std::pair<DomainT,CodomainT> > > {};

    template <class DomainT> 
    class RandomGentor<itl::list<itl::interval<DomainT> > > :
        public SeqGentorT<itl::list<itl::interval<DomainT> > > {};

    template <class DomainT> 
    class RandomGentor<itl::list<DomainT> > :
        public SeqGentorT<itl::list<DomainT> > {};

    // ----- sets --------------------------------------------------------------
    //template <class DomainT, template<class>class Set> 
    //class RandomGentor<Set<DomainT> > :
    //    public SetGentorT<Set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::set<DomainT> > :
        public SetGentorT<itl::set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::interval_set<DomainT> > :
        public SetGentorT<itl::interval_set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::separate_interval_set<DomainT> > :
        public SetGentorT<itl::separate_interval_set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::split_interval_set<DomainT> > :
        public SetGentorT<itl::split_interval_set<DomainT> > {};

    // ----- tree --------------------------------------------------------------
#ifdef USE_ITL_TREE
    template <class DomainT> 
    class RandomGentor<itl::tree<DomainT> > :
        public SetGentorT<itl::tree<DomainT> > {};
#endif

    // ----- maps --------------------------------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<itl::map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<itl::map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    class RandomGentor<itl::map<DomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > : 
        public MapGentorT<itl::map<DomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<itl::map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<itl::map<DomainT,CodomainT,Neutronizer> > {};


    // ----- interval_map<D,C,N> -----------------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<interval_map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<interval_map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<interval_map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<interval_map<DomainT,CodomainT,Neutronizer> > {};

    // ----- split_interval_map<D,C,N> ----------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<split_interval_map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<split_interval_map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<split_interval_map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<split_interval_map<DomainT,CodomainT,Neutronizer> > {};

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    template <class NumericDomainT, class BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    class RandomGentor<split_interval_map<NumericDomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > : 
        public MapGentorT<split_interval_map<NumericDomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > {};

    template <class NumericDomainT, class BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    class RandomGentor<interval_map<NumericDomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > : 
        public MapGentorT<interval_map<NumericDomainT,itl::bits<BitsT>,Neutronizer,Compare,Combine,Section> > {};

    //NOTE: All trials to reduce code replication for RandomGentor
    // and Calibrater by introducing a fancy 
    // template template IntervalMap shipwrecked due to compilers 
    // disability to resolve the resulting instantiations. Compilers
    // always see ambiguities where there seems to be a resolution.


    // ------------------------------------------------------------------------
    // class template SomeValue:
    // TargetT is a value tuple that fits to the value generator gentor
    // of type GentorT<TargetT>. The special kind of generator can be
    // passed by the user. The only requirement is that the generator
    // implements a function 'some' which is called to generate a tuple
    // of type TargetT
    template <class TargetT, template<class>class GentorT> struct SomeValue
    {
        static void apply(TargetT& value, GentorT<TargetT>& gentor); 
    };

    template <class TargetT> 
    struct SomeValue<TargetT, RandomGentor>
    {
        static void apply(TargetT& value, RandomGentor<TargetT>& gentor) 
        {
            gentor.some(value);
        }
    };
    // ------------------------------------------------------------------------


    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    template <class TargetT, template<class>class GentorT> struct Calibrater
    {
        static void apply(GentorT<TargetT>& gentor); 
    };

    template <> 
    struct Calibrater<int, RandomGentor>
    {
        static void apply(RandomGentor<int>& gentor)
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_int());
        }
    };

    template <> 
    struct Calibrater<nat, RandomGentor>
    {
        static void apply(RandomGentor<nat>& gentor)
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_nat());
        }
    };

    template <> 
    struct Calibrater<double, RandomGentor>
    {
        static void apply(RandomGentor<double>& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_double());
        }
    };

    template <class BitsT> 
    struct Calibrater<itl::bits<BitsT>, RandomGentor>
    {
        static void apply(RandomGentor<itl::bits<BitsT> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.set_range(itl::interval<BitsT>::rightopen(0, sizeof(BitsT)));
        }
    };


    template <> 
    struct Calibrater<itl::interval<int>, RandomGentor>
    {
        static void apply(RandomGentor<itl::interval<int> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_int());
        }
    };


#ifdef LAW_BASED_TEST_BOOST_POLYGON
    //--------------------------------------------------------------------------
    // boost::polygon
    //--------------------------------------------------------------------------
    template <> 
    struct Calibrater<itl::point<int>, RandomGentor>
    {
        static void apply(RandomGentor<itl::point<int> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_int());
        }
    };

    template <> 
    struct Calibrater<itl::list<point<int> >, RandomGentor>
    {
        static void apply(RandomGentor<itl::list<point<int> > >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_codomain_ContainerSize());
            point_gentor<int>* pointGentor = new point_gentor<int>;
            pointGentor->setRange(GentorProfileSgl::it()->range_int());
            gentor.setDomainGentor(pointGentor);
            gentor.setUnique(true);
        }
    };

    template <> 
    struct Calibrater<itl::list<list<point<int> > >, RandomGentor>
    {
        static void apply(RandomGentor<itl::list<list<point<int> > > >& gentor) 
        {
            point_gentor<int>* pointGentor = new point_gentor<int>;
            pointGentor->setRange(GentorProfileSgl::it()->range_int());

            polygon_gentor<list<point<int> > >* polyGentor = new polygon_gentor<list<point<int> > >;
            polyGentor->setDomainGentor(pointGentor);
            polyGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_codomain_ContainerSize());
            polyGentor->setUnique(true);

            gentor.setDomainGentor(polyGentor);
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
        }
    };

    //--------------------------------------------------------------------------
    // nogylop::tsoob
    //--------------------------------------------------------------------------
#endif // LAW_BASED_TEST_BOOST_POLYGON


    //--------------------------------------------------------------------------
    // lists
    //--------------------------------------------------------------------------
    template <> 
    struct Calibrater<std::pair<int,int>, RandomGentor>
    {
        static void apply(RandomGentor< std::pair<int,int> >& gentor){}
    };

    template <> 
    struct Calibrater<std::pair<itl::interval<int>, int>, RandomGentor>
    {
        static void apply(RandomGentor< std::pair<itl::interval<int>, int> >& gentor){}
    };

    template <> 
    struct Calibrater<std::pair<itl::interval<double>, int>, RandomGentor>
    {
        static void apply(RandomGentor< std::pair<itl::interval<double>, int> >& gentor){}
    };

    template <> 
    struct Calibrater<itl::list<std::pair<itl::interval<double>, int> >, RandomGentor>
    {
        static void apply(RandomGentor< itl::list<std::pair<itl::interval<double>, int> > >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            map_segment_gentor<double,int>* segment_gentor  = new map_segment_gentor<double,int>;
            NumberGentorT<int>* int_gentor = new NumberGentorT<int>;
            int_gentor->setRange(GentorProfileSgl::it()->range_int());
            segment_gentor->setCodomainGentor(int_gentor);
            segment_gentor->set_range(GentorProfileSgl::it()->range_interval_double());
            segment_gentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            segment_gentor->set_corange(GentorProfileSgl::it()->range_int()); // sets range of codomain_values
            gentor.setDomainGentor(segment_gentor);
        }
    };

    template <class NumericT> 
    struct Calibrater<itl::list< itl::interval<NumericT> >, RandomGentor>
    {
        static void apply(RandomGentor< itl::list< itl::interval<NumericT> > >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<NumericT>* itvGentor  = new ItvGentorT<NumericT>;
            itvGentor->setRange(GentorProfileSgl_numeric_range<NumericT>::get());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <class NumericT> 
    struct Calibrater<itl::list<NumericT>, RandomGentor>
    {
        static void apply(RandomGentor< itl::list<NumericT> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            NumberGentorT<NumericT>* domainGentor = new NumberGentorT<NumericT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericT>::get());
            gentor.setDomainGentor(domainGentor);
        }
    };

    template <> 
    struct Calibrater<itl::list<std::pair<int, int> >, RandomGentor>
    {
        static void apply(RandomGentor< itl::list< std::pair<int, int> > >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            std_pair_gentor<int,int>* pair_gentor  = new std_pair_gentor<int,int>;
            NumberGentorT<int>* int_gentor_1 = new NumberGentorT<int>;
            NumberGentorT<int>* int_gentor_2 = new NumberGentorT<int>;
            int_gentor_1->setRange(GentorProfileSgl::it()->range_int());
            int_gentor_2->setRange(GentorProfileSgl::it()->range_int());
            pair_gentor->set_first_gentor(int_gentor_1);
            pair_gentor->set_second_gentor(int_gentor_2);
            gentor.setDomainGentor(pair_gentor);
        }
    };

    template <> 
    struct Calibrater<itl::list<std::pair<itl::interval<int>, int> >, RandomGentor>
    {
        static void apply(RandomGentor< itl::list<std::pair<itl::interval<int>, int> > >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            map_segment_gentor<int,int>* segment_gentor  = new map_segment_gentor<int,int>;
            NumberGentorT<int>* int_gentor = new NumberGentorT<int>;
            int_gentor->setRange(GentorProfileSgl::it()->range_int());
            segment_gentor->setCodomainGentor(int_gentor);
            segment_gentor->set_range(GentorProfileSgl::it()->range_interval_int());
            segment_gentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            segment_gentor->set_corange(GentorProfileSgl::it()->range_int()); // sets range of codomain_values
            gentor.setDomainGentor(segment_gentor);
        }
    };

    //--------------------------------------------------------------------------
    // sets
    //--------------------------------------------------------------------------
    template <> 
    struct Calibrater<itl::set<int>, RandomGentor>
    {
        static void apply(RandomGentor<itl::set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            NumberGentorT<int>* intGentor = new NumberGentorT<int>;
            intGentor->setRange(GentorProfileSgl::it()->range_int());
            gentor.setDomainGentor(intGentor);
        }
    };

    template <> 
    struct Calibrater<itl::set<double>, RandomGentor>
    {
        static void apply(RandomGentor<itl::set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            NumberGentorT<double>* elemGentor = new NumberGentorT<double>;
            elemGentor->setRange(GentorProfileSgl::it()->range_double());
            gentor.setDomainGentor(elemGentor);
        }
    };


    template <> 
    struct Calibrater<interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<interval_set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<separate_interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<separate_interval_set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<separate_interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<separate_interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<split_interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_set<int> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<split_interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };


    //--------------------------------------------------------------------------
    // itl::tree
    //--------------------------------------------------------------------------
#ifdef USE_ITL_TREE
    template <> 
    struct Calibrater<itl::tree<int>, RandomGentor>
    {
        static void apply(RandomGentor<itl::tree<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());
            gentor.setDomainGentor(itvGentor);
        }
    };
#endif

    //----------------------------------------------------------------------------
    // itl::map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <typename NumericDomainT, class Neutronizer> 
    struct Calibrater<itl::map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<itl::map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            NumberGentorT<NumericDomainT>* domainGentor = new NumberGentorT<NumericDomainT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericDomainT>::get());

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl_numeric_range<int>::get());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            gentor.setDomainGentor(domainGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer> 
    struct Calibrater<itl::map<NumericDomainT,NumericCodomainT,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<itl::map<NumericDomainT,NumericCodomainT,Neutronizer> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            NumberGentorT<NumericDomainT>* domainGentor = new NumberGentorT<NumericDomainT>;
            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericDomainT>::get());
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());
            gentor.setDomainGentor(domainGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    struct Calibrater<itl::map<NumericDomainT,itl::bits<BitsT>,Neutronizer,
                               Compare,Combine,Section>, RandomGentor>
    {
        static void apply(RandomGentor<itl::map<NumericDomainT,itl::bits<BitsT>,Neutronizer,
                                                Compare,Combine,Section> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            NumberGentorT<NumericDomainT>* domainGentor   = new NumberGentorT<NumericDomainT>;
            bits_gentor<BitsT>*            codomainGentor = new bits_gentor<BitsT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericDomainT>::get());
            codomainGentor->set_range(interval<BitsT>::closed(0, (std::numeric_limits<BitsT>::max)()));
            gentor.setDomainGentor(domainGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };


    //----------------------------------------------------------------------------
    // itl::interval_map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <class NumericDomainT, class Neutronizer> 
    struct Calibrater<interval_map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<interval_map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl::it()->range_int());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_codomain_ContainerSize());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer> 
    struct Calibrater<interval_map<NumericDomainT,NumericCodomainT,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<interval_map<NumericDomainT,NumericCodomainT,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    //----------------------------------------------------------------------------
    // itl::split_interval_map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <class NumericDomainT, class Neutronizer> 
    struct Calibrater<split_interval_map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl::it()->range_int());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_codomain_ContainerSize());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    struct Calibrater<split_interval_map<NumericDomainT,NumericCodomainT,Neutronizer,Compare,Combine,Section>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_map<NumericDomainT,NumericCodomainT,Neutronizer,Compare,Combine,Section> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };


    template <typename NumericDomainT, typename BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    struct Calibrater<interval_map<NumericDomainT,itl::bits<BitsT>,
                                   Neutronizer,Compare,Combine,Section>, RandomGentor>
    {
        static void apply(RandomGentor<interval_map<NumericDomainT,itl::bits<BitsT>,
                                                          Neutronizer,Compare,Combine,Section> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            bits_gentor<BitsT>* codomainGentor = new bits_gentor<BitsT>;
            codomainGentor->set_range(interval<BitsT>::closed(0, (std::numeric_limits<BitsT>::max)()));

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename BitsT, class Neutronizer,
              ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section> 
    struct Calibrater<split_interval_map<NumericDomainT,itl::bits<BitsT>,
                                         Neutronizer,Compare,Combine,Section>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_map<NumericDomainT,itl::bits<BitsT>,
                                                          Neutronizer,Compare,Combine,Section> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            bits_gentor<BitsT>* codomainGentor = new bits_gentor<BitsT>;
            codomainGentor->set_range(interval<BitsT>::closed(0, (std::numeric_limits<BitsT>::max)()));

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };
    // ---------------------------------------------------------------------------


}} // namespace itl boost
