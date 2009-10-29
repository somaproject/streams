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

#include <limits>
#include <boost/itl_xt/statvardesct.hpp>
#include <boost/itl_xt/var_tuple_order.hpp>
#include <boost/itl_xt/tuple_computer.hpp>

#include <sstream>

using namespace std;
using namespace boost::itl;

/** Example amount_cube.cpp \file amount_cube.cpp

    The template class TupelComputerT is a general class for computing aggregations
    on values that are associated to elements of a cartesian product. Stated more 
    simply TupelComputerT allows to count values associated to tupels.

    Basically a TupelComputer is a map associating a tupel (t1, ..., tn) to a 
    value v:
    (t1, ..., tn) -> v

    The values v can be amounts like integers, but also sets, an more complex 
    classes, provided an operator += for aggregation is defined on them.

    A TupelComputerT can be conceived as an n-dimensional cube.
    Instances of TupelComputers can be found in many fields. General crosstable
    prcedures in statistical systems, pivot tables in excel, online analytic
    processing tools (OLAP-cubes) and functionalities of data warehouse systems
    mirror an apply the concepts that TupelConputerT tries to offer as a c++
    template class.

    The most basic example of a TupelComputerT is the one that associates integers.
    We call it AmountTupelComputer or AmountCube.

    A TupelComputer counts on insert. That is, it applies the += operation on 
    associated values whenever the member function 'insert' is called and an 
    appropriate tupel is in the cube.

    An AmountTupelComputer counts frequencies of tupels if the inserted associated 
    value is always 1.

    This is the most basic instance of a TupelComputerT. It is demonstrated in this 
    sampe file.
    
    \include amount_cube/amount_cube.cpp
*/

/*    Consider a hospital administration that want's to count their patients 
    according to the attributes sex, age, and diagnosis ... */
enum sex            { female=0,      male=1 };
enum diagnosis        { anorexia=0, stroke=1 };

// These are the components of the tupel (indendent variables) that are to 
// be counted ...
enum PatientVarsET { sex, age, diagnosis, PatientVarsET_size };

// ... which makes up tupels of 3 elements
typedef var_tuple<PatientVarsET_size> PatientInfoTD;
typedef    amount_tuple_computer<PatientVarsET_size, int>    AmountCubeTD;

// This macro helps the notation a little
#define TRIPEL(tp,x0,x1,x2) tp[0]=x0; tp[1]=x1; tp[2]=x2 


void evaluate_by_tupel_insertion(AmountCubeTD& cube)
{
    /*    So, counting of cases is done just by inserting tupels of attributes with
        an associated 1. Equal tupel are summed up. So we expect a total of
        2 for 17 year old annorectic females. All other tupels are obviously unique. */
    PatientInfoTD marys_data;  TRIPEL(marys_data,  female, 17, anorexia); cube.insert(marys_data, 1);
    PatientInfoTD sallys_data; TRIPEL(sallys_data, female, 17, anorexia); cube.insert(sallys_data, 1);
    PatientInfoTD peters_data; TRIPEL(peters_data, male,   36, anorexia); cube.insert(peters_data, 1);
    PatientInfoTD anns_data;   TRIPEL(anns_data,   female, 34, stroke);   cube.insert(anns_data, 1);
    PatientInfoTD pauls_data;  TRIPEL(pauls_data,  male,   72, stroke);   cube.insert(pauls_data, 1);
    PatientInfoTD franks_data; TRIPEL(franks_data, male,   82, stroke);   cube.insert(franks_data, 1);
}

/*    Shows frequencies of attribute tupels for patients. Sums up frequencies for
    equal attribute combinations (tupels) */
void accumulate_identical_tupel()
{
    /*    Every TupelComputer need an order for their tupel. In the most simple case
        we take the default order provided by the default constructor */
    var_tuple_order<PatientInfoTD> defaultOrder;
    AmountCubeTD amountCube(defaultOrder);

    // Inserting an thus computing frquencies
    evaluate_by_tupel_insertion(amountCube);
    // Show result
    cout << "accumulate_identical_tupel(): (female, 17, annorexia)->2 all other tupels->1" << endl;
    cout << amountCube.as_string() << endl;
    cout << "----------------------------------------------------------------------------" << endl;
}


/*    We do not want to count each age separately but to evaluate frequencies
    for groups of ages. So we define: */
enum AgeGroupsET    { young=0,      old=1 };

/*    Modifying the tupel order we can indeed change the computation. Class
    GroupByAge shows how a grouping on age can be introduced to the computing
    of a TupelComputer. */
class GroupByAge : public group_order<PatientVarsET_size> 
{
public:
    GroupByAge() : m_var(age) {}

    /// Strict weak standard ordering for the grouped component.
    bool operator() (const PatientInfoTD& lhs, const PatientInfoTD& rhs)const
    { return ageGroup(lhs) < ageGroup(rhs); }

    /// Get number of tupel component which is grouped
    VarEnumTD getVarIndex()const { return m_var; }

    /// Equivalence introduced by the grouping
    bool areEquivalent(const PatientInfoTD& lhs, const PatientInfoTD& rhs)const
    { return ageGroup(lhs) == ageGroup(rhs); }

    /// That is the way we group ages (quite radical)
    AgeGroupsET ageGroup(const PatientInfoTD& info)const {return info[age] <= 35 ? young : old; }

private:
    PatientVarsET m_var; // Tupel component which is grouped
};



void accumulate_for_grouped_age()
{
    //    Now we can modify the tupel order
    var_tuple_order<PatientInfoTD> groupedAgeOrder;

    //    Grouping for age is introduced that way
    GroupByAge ageGrouper;
    groupedAgeOrder.setGroupOrder(&ageGrouper);
    AmountCubeTD amountCube(groupedAgeOrder);

    /*    The same data collection now yield a different result. As desired countig
        of ages is now grouped for two rough age groups */
    evaluate_by_tupel_insertion(amountCube);

    cout << "accumulate_for_grouped_age():" << endl;
    cout << "(fem,yng,anrx)->2 (fem,yng,strk)->1 (mle,old,anrx)->1 (mle,old,strk)->2" << endl;
    cout << amountCube.as_string() << endl;
    cout << "----------------------------------------------------------------------------" << endl;
}

/*    We can compute partial sums by switching components off the tupel.
    Components to be counted are defined by a PermutationT Object.
    A PermutationT is a selection that permutes the iteration order as well. */
void accumulate_for_sex()
{
    var_tuple_order<PatientInfoTD> groupedAgeOrder;

    GroupByAge ageGrouper;
    groupedAgeOrder.setGroupOrder(&ageGrouper);
    var_permutation<PatientVarsET_size> perm;
    // Count component 'sex' only
    perm.add(sex);
    groupedAgeOrder.setPermutation(perm);

    AmountCubeTD amountCube(groupedAgeOrder);

    evaluate_by_tupel_insertion(amountCube);

    cout << "accumulate_for_sex(): (female,-,-)->3 (male,-,-)->3 " << endl;
    cout << amountCube.as_string() << endl;
    cout << "----------------------------------------------------------------------------" << endl;
}

void accumulate_except_for_sex()
{
    var_tuple_order<PatientInfoTD> groupedAgeOrder;

    GroupByAge ageGrouper;
    groupedAgeOrder.setGroupOrder(&ageGrouper);
    var_permutation<PatientVarsET_size> perm;
    // Count component age(grouped) and diagnosis but not sex
    perm.add(age).add(diagnosis);
    groupedAgeOrder.setPermutation(perm);

    AmountCubeTD amountCube(groupedAgeOrder);

    evaluate_by_tupel_insertion(amountCube);

    cout << "accumulate_except_for_sex():" << endl;
    cout << "(-,yng,anrx)->2 (-,yng,strk)->1 (-,old,anrx)->1 (-,old,strk)->2" << endl;
    cout << amountCube.as_string() << endl;
    cout << "----------------------------------------------------------------------------" << endl;
}

void accumulate_the_total()
{
    var_tuple_order<PatientInfoTD> groupedAgeOrder;

    GroupByAge ageGrouper;
    groupedAgeOrder.setGroupOrder(&ageGrouper);
    var_permutation<PatientVarsET_size> perm;
    // The empty PermutationT yields the grand total
    groupedAgeOrder.setPermutation(perm);

    AmountCubeTD amountCube(groupedAgeOrder);

    evaluate_by_tupel_insertion(amountCube);

    cout << "accumulate_the_total():(-,-,-)->6" << endl;
    cout << amountCube.as_string() << endl;
    cout << "----------------------------------------------------------------------------" << endl;
}



int main()
{
    cout << ">> Interval Template Library: Sample amount_cube.cpp <<\n";
    cout << "-------------------------------------------------------\n";
    cout << "Note that there is always a representing value for a tupel component\n";
    cout << "even if the component is grouped or switched off.\n";
    cout << "-------------------------------------------------------\n";
    accumulate_identical_tupel();
    accumulate_for_grouped_age();
    accumulate_for_sex();
    accumulate_except_for_sex();
    accumulate_the_total();
    return 0;
}

// Program output

// >> Interval Template Library: Sample amount_cube.cpp <<
// -------------------------------------------------------
// Note that there is always a representing value for a tupel component
// even if the component is grouped or switched off.
// -------------------------------------------------------
// accumulate_identical_tupel(): (female, 17, annorexia)->2 all other tupels->1
// ((0,17,0)->2)((0,34,1)->1)((1,36,0)->1)((1,72,1)->1)((1,82,1)->1)
// ----------------------------------------------------------------------------
// accumulate_for_grouped_age():
// (fem,yng,anrx)->2 (fem,yng,strk)->1 (mle,old,anrx)->1 (mle,old,strk)->2
// ((0,17,0)->2)((0,34,1)->1)((1,36,0)->1)((1,72,1)->2)
// ----------------------------------------------------------------------------
// accumulate_for_sex(): (female,-,-)->3 (male,-,-)->3
// ((0,17,0)->3)((1,36,0)->3)
// ----------------------------------------------------------------------------
// accumulate_except_for_sex():
// (-,yng,anrx)->2 (-,yng,strk)->1 (-,old,anrx)->1 (-,old,strk)->2
// ((0,17,0)->2)((0,34,1)->1)((1,36,0)->1)((1,72,1)->2)
// ----------------------------------------------------------------------------
// accumulate_the_total():(-,-,-)->6
// ((0,17,0)->6)
// ----------------------------------------------------------------------------


