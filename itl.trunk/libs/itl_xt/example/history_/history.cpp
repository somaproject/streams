/*-----------------------------------------------------------------------------+    
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/split_interval_map.hpp>
#include "../toytime.h"

/** Example history.cpp \file history.cpp

History demonstrates further possibilities of an interval map
(interval_map or split_interval_map). For a more basic example see party.cpp.

Here we show the mechanics of a product_history which is split_interval_map that
maps intervals to products of attributes that change over time.

Using product_histories, we can monitor a set of attributes in a history:

Monday 22:00: A person is taken to a hospital with a head injury. So a suspicion
of cranial fraction is filed.

Monday 22:05: In a first examination at the emergency ward an alcoholic 
intoxication is diagnosed.

Monday 22:15: The person is moved to the radiology to examine the headinjury.
There he has to wait for 15 minutes in the hall.

Monday 22:30: Using x-ray, cranial fraction can be excluded.

Monday 22:50: The person is moved to ward A for treatment. The head injury is
finally diagnosed as laceration.

Tuesday 07:00: The person falls into an acute delirium due to deprivation of 
alcohol.

This history of events could be formalized like that:
\code
[mon:22:00, mon:22:05) -> { DIAG->{susp.cran.frac.}, WARD->{emergency ward} }
[mon:22:05, mon:22:15) -> { DIAG->{susp.cran.frac., alc.intox.}, WARD->{emergency ward} }
[mon:22:15, mon:22:30) -> { DIAG->{susp.cran.frac., alc.intox.} }
[mon:22:30, mon:22:50) -> { DIAG->{alc.intox.}, WARD->{radiology} }
[mon:22:50, tue:07:00) -> { DIAG->{laceration, alc.intox.}, WARD->{ward A} }
[tue:07:00, tue:07:20) -> { DIAG->{laceration, acute delirium}, WARD->{ward A} }
\endcode

Now suppose the episodes of that history are recorded separately for diagnoses and
ward. A situation that is often found if data are stored in a well designed database.

\code
Diagnosis:
[mon:22:00, mon:22:30) -> susp.cran.frac.
[mon:22:05, mon:07:00) -> alc.intox.
[mon:22:50, mon:07:20) -> laceration
[mon:07:00, mon:07:20) -> acute delirium

Stays:
[mon:22:00, mon:22:15) -> emergency ward
(unregistered gap)
[mon:22:30, mon:22:50) -> radiology
[mon:22:50, mon:07:20) -> ward A
\endcode

A product_history allows to compute the history of events by simply adding all
separate episode data to a history object.

\include history/history.cpp
*/



#include <boost/itl_xt/typed_episode.hpp>
#include <boost/itl_xt/episode_product.hpp>
#include <boost/itl_xt/product_history.hpp>

using namespace std;
using namespace boost::itl;


/* To use a product_history object, we need an TypeDomain-class, specifying
    an enumeration type and a common value type for all episodes of a given
    problem domain to work on.
*/
class HospitalTypeDomain
{
public:
    /*  All episode classes for the hospital sample share a common value type ValueBaseTD
        which is std::string in this toy example. Use any class of your problem domain
        for real word instances. */
    typedef std::string ValueBaseTD ;

    // For derived episode classes, we define an enumeration 
    enum domainET { diagnosis, ward };
    typedef enum domainET DomainET;

    // string representation of the episode types
    static std::string as_string(DomainET val)
    {
        switch(val)
        {
        case diagnosis: return std::string("DIAG");
        case ward:      return std::string("WARD");
        default:        return std::string("Unknown");
        }
    }
};


/*  Base class for all episodes of the sample hospital problem domain. 
    An episode is an object that has an interval and a value.          */
class HospitalEpisodes : public typed_episode<Time, HospitalTypeDomain>
{
public:
    // The domain type of intervals used by HospitalEpisodes is (toy)Time
    typedef Time ItvDomTD;
    // Type of the intervals used by HospitalEpisodes
    typedef boost::itl::interval<Time> IntervalTD;

public:
    // Construct an episode from interval and value
    HospitalEpisodes(const IntervalTD& itv, const std::string& val):
        m_itv(itv), m_value(val) 
    {}

    // Every episode has an interval
    virtual IntervalTD interval()const { return m_itv; }

    void setValue(const HospitalTypeDomain::ValueBaseTD& val) { m_value = val; }

    virtual const HospitalTypeDomain::ValueBaseTD* value()const { return &m_value; }

    // Equality of values
    virtual bool isValueEqual(const ordered_type<HospitalTypeDomain>* x2)const
    { return m_value==dynamic_cast<const HospitalEpisodes*>(x2)->m_value; }

    // String representation
    virtual std::string as_string()const { return m_value; }

private:
    // For sake of simplicity the value of the example episodes is text
    HospitalTypeDomain::ValueBaseTD m_value;
    IntervalTD m_itv;
};

// ----------------------------------------------------------------------------
// Now specific episode-classes for diagnoses and wards are implemented
// ----------------------------------------------------------------------------

// Diagnoses: Begin, end and kind of desease
class DiagnosisEpisode : public HospitalEpisodes
{
public:
    DiagnosisEpisode(Time begin, Time end, const std::string& val)
        : HospitalEpisodes(boost::itl::interval<Time>::rightopen(begin,end),val){}

    HospitalTypeDomain::DomainET type()const { return HospitalTypeDomain::diagnosis; }
};

// Wards: Begin, end and Number of Ward a patient stayed at
class WardEpisode : public HospitalEpisodes
{
public:
    WardEpisode(Time begin, Time end, const std::string& val)
        : HospitalEpisodes(boost::itl::interval<Time>::rightopen(begin,end),val){}

    HospitalTypeDomain::DomainET type()const { return HospitalTypeDomain::ward; }
};

// ----------------------------------------------------------------------------
/* Having defined hospital episodes, we can build a history class for
   by instatiation of the product_history template class. */
class HospitalProductHistory : public product_history<Time, HospitalTypeDomain>
{    
public:
    typedef product_history<Time, HospitalTypeDomain> BaseTypeTD;
    typedef episode_product<Time, HospitalTypeDomain> ValueTypeTD;
    typedef BaseTypeTD::IntervalTD IntervalTD;
    typedef BaseTypeTD::DomainTD DomainTD;
};

// We may call a single entry in that history a hospital event.
typedef HospitalProductHistory::ValueTypeTD HospitalEventTD;

void medical_file()
{
    DiagnosisEpisode susp_cran_frac(Time(monday, 22,00), Time(monday, 22,30), "susp.cran.frac.");
    DiagnosisEpisode alc_intox     (Time(monday, 22,05), Time(tuesday, 7,00), "alc.intox.");
    DiagnosisEpisode laceration    (Time(monday, 22,50), Time(tuesday, 7,20), "laceration");
    DiagnosisEpisode acute_delirium(Time(tuesday, 7,00), Time(tuesday, 7,20), "acute_delirium");

    WardEpisode      emergency_ward(Time(monday, 22,00), Time(monday, 22,15), "emergency_ward");
    WardEpisode      radiology     (Time(monday, 22,30), Time(monday, 22,50), "radiology");
    WardEpisode      ward_A        (Time(monday, 22,50), Time(tuesday, 7,20), "ward_A");

    HospitalProductHistory history;

    history += &susp_cran_frac;
    history += &alc_intox;
    history += &laceration;
    history += &acute_delirium;

    history += &emergency_ward;
    history += &radiology;
    history += &ward_A;

    HospitalProductHistory::iterator it = history.begin();
    while(it != history.end())
    {
        interval<Time> when = (*it).first;
        HospitalEventTD what = (*it).second;

        cout << when.as_string() << ": " << what.as_string() << endl;
        ++it;
    }
}

int main()
{
    cout << ">> Interval Template Library: Sample history.cpp <<\n";
    cout << "---------------------------------------------------\n";
    medical_file();
    return 0;
}

// Program output:

// >> Interval Template Library: Sample history.cpp <<
// ---------------------------------------------------
// [mon:22:00,mon:22:05): DIAG{susp.cran.frac.}, WARD{emergency_ward}
// [mon:22:05,mon:22:15): DIAG{alc.intox. susp.cran.frac.}, WARD{emergency_ward}
// [mon:22:15,mon:22:30): DIAG{alc.intox. susp.cran.frac.}
// [mon:22:30,mon:22:50): DIAG{alc.intox.}, WARD{radiology}
// [mon:22:50,tue:07:00): DIAG{laceration alc.intox.}, WARD{ward_A}
// [tue:07:00,tue:07:20): DIAG{acute_delirium laceration}, WARD{ward_A}
