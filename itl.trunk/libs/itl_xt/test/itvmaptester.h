/* ------------------------------------------------------------------
class ItvMapTesterT
--------------------------------------------------------------------*/
#ifndef __ITVMAPTESTER_H_JOFA_990226__
#define __ITVMAPTESTER_H_JOFA_990226__

#include <boost/itl_xt/itvgentor.hpp>
#include <boost/itl_xt/mapgentor.hpp>

namespace boost{namespace itl
{

template <class ItvMapTV>
class ItvMapTesterT
{
public:
    typedef typename ItvMapTV::domain_type       domain_type;
    typedef typename ItvMapTV::codomain_type     codomain_type;
    typedef typename ItvMapTV::interval_set_type interval_set_type;
    typedef ItvGentorT<domain_type>              domain_generator_type;
    typedef NumberGentorT<codomain_type>         codomain_generator_type;

    void set_domain_generator(domain_generator_type* gentor)
    { m_ContainerGentor.setDomainGentor(gentor); }

    void set_codomain_generator(codomain_generator_type* gentor)
    { m_ContainerGentor.setCodomainGentor(gentor); }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_ContainerGentor.setRangeOfSampleSize(lwb,upb); }

    // The value of a Map is independent of the insertion sequence
    bool valueIsInsertionIndependent(ItvMapTV& y, ItvMapTV& y_perm);
    bool testInsertionIndependence(int nTries);

    // The value of a Map is invariant wrt. join-normalization
    bool valueIsJoinIndependent(ItvMapTV& y, ItvMapTV& y_join);
    bool testJoinIndependence(int nTries);

    // The value of a Map is invariant wrt. join-normalization after insert-permutation
    bool valueIsInsertAndJoinIndependent(ItvMapTV& y, ItvMapTV& y_perm, ItvMapTV& y_permJoin);
    bool testInsertAndJoinIndependence(int nTries);

    //LAW: x + y - join(perm(y)) == x;
    bool isInsertReversible1
        (ItvMapTV& x, ItvMapTV& y, ItvMapTV& y_perm, ItvMapTV& y_permJoin, 
         ItvMapTV& x_plus_y, ItvMapTV& x2); 
    bool testInsertReversibility1(int nTries, char* errFile);

    void debugInsertReversibility1(const ItvMapTV& x, const ItvMapTV& y, const ItvMapTV y_perm);

    //LAW: x + join(perm(y)) - y == x;
    bool isInsertReversible2
        (ItvMapTV& x, ItvMapTV& y, ItvMapTV& y_perm, ItvMapTV& y_permJoin, 
         ItvMapTV& x_plus_y_pj, ItvMapTV& x2); 
    bool testInsertReversibility2(int nTries, char* errFile);

    void debugInsertReversibility2(const ItvMapTV& x, const ItvMapTV& y, const ItvMapTV y_perm);

    //LAW: x -~ y == x * (dom(x) - y)
    // operator -~ steht fuer erase
    // x.erase(y) hat das gleiche Ergebnis wie der Schnitt von x mit x.domain() - y
    bool hasErasureAsIntersectionComputability(ItvMapTV& x, interval_set_type& y, 
        interval_set_type& xDom_minus_y, ItvMapTV& x_resid, ItvMapTV& x_sect);

    bool testErasureAsIntersectionComputability(int nTries, char* errFile);

private:
    MapGentorT<ItvMapTV>         m_ContainerGentor;
};


// ----------------------------------------------------------------------------
// LAW: InsertionIndependency<ItvMapTV>
// LAW: x == perm(x)

template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::valueIsInsertionIndependent(ItvMapTV& y, ItvMapTV& y_perm)
{
    ItvMapTV x, x_perm;
    m_ContainerGentor.some(x);
    m_ContainerGentor.last_permuted(x_perm);

    if(! x.isEqual(x_perm) ) {
        y = x; y_perm = x_perm;
        return false;
    } 
    else return true;
}



template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testInsertionIndependence(int nTries)
{
    bool correct=true;
    ItvMapTV y, y_perm;
    ItvMapTV min_y, min_y_perm;

    for(int i=0; i<nTries; i++)
        if(!valueIsInsertionIndependent(y, y_perm))
        {
            if(true==correct) { min_y = y; min_y_perm = y_perm; correct=false; }
            else if( y.interval_count() < min_y.interval_count() ) {
                min_y = y; min_y_perm = y_perm;
            }
        }

    if(!correct) {
        std::cout << "InsertionIndependence violated" << std::endl;
    }

    return correct;
}


// ----------------------------------------------------------------------------
// LAW: JoinIndependency<ItvMapTV>
// LAW: x == join(x)

template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::valueIsJoinIndependent(ItvMapTV& y, ItvMapTV& y_join)
{
    ItvMapTV x, x_perm, x_join;
    m_ContainerGentor.some(x);
    x_join = x;
    x_join.join();
    
    if(! x.isEqual(x_join) ) { y = x; y_join = x_join; return false; } 
    else return true;
}



template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testJoinIndependence(int nTries)
{
    bool correct=true;
    ItvMapTV y, y_join;
    ItvMapTV min_y, min_y_join;

    for(int i=0; i<nTries; i++)
        if(!valueIsJoinIndependent(y, y_join))
        {
            if(true==correct) { 
                min_y = y; min_y_join = y_join;
                correct=false;
                std::cout << "y.sz=" << y.interval_count() << "  try=" << i << std::endl;
            }
            else if( y.interval_count() < min_y.interval_count() ) {
                min_y = y; min_y_join = y_join;
                std::cout << "y.sz=" << y.interval_count() << "  try=" << i << std::endl;
            }
        }

    if(!correct) {
        std::cout << "JoinIndependence violated ---------------------------" << std::endl;
        std::cout << "y     :" << min_y.as_string().c_str() << std::endl;
        std::cout << "y_join:" << min_y_join.as_string().c_str() << std::endl;
    }

    return correct;
}

// ----------------------------------------------------------------------------
// LAW: InsertAndJoinIndependency<IntSetTV>
// LAW: x == join(perm(x))

template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::valueIsInsertAndJoinIndependent(ItvMapTV& y, ItvMapTV& y_perm, ItvMapTV& y_permJoin)
{
    ItvMapTV x, x_perm, x_permJoin;
    m_ContainerGentor.some(x);
    m_ContainerGentor.last_permuted(x_perm);
    x_permJoin = x_perm;
    x_permJoin.join();
    
    if(! x.isEqual(x_permJoin) ) {
        y = x; y_perm = x_perm; y_permJoin = x_permJoin;
        return false;
    } 
    else return true;
}



template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testInsertAndJoinIndependence(int nTries)
{
    bool correct=true;
    ItvMapTV y, y_perm, y_permJoin;
    ItvMapTV min_y, min_y_perm, min_y_permJoin;

    for(int i=0; i<nTries; i++) 
    {
        if(!valueIsInsertAndJoinIndependent(y, y_perm, y_permJoin))
        {
            if(true==correct) { 
                min_y = y; min_y_perm = y_perm; min_y_permJoin = y_permJoin;
                correct=false;
                std::cout << "y.sz=" << y.interval_count() << "  try=" << i << std::endl;
            }
            else if( y.interval_count() < min_y.interval_count() ) {
                min_y = y; min_y_perm = y_perm; min_y_permJoin = y_permJoin;
                std::cout << "y.sz=" << y.interval_count() << "  try=" << i << std::endl;
            }
        }
    }

    if(!correct) {
        std::cout << "InsertAndJoinIndependence violated ---------------------------" << std::endl;
        std::cout << "y      :" << min_y.as_string().c_str() << std::endl;
        std::cout << "y_perm :" << min_y_perm.as_string().c_str() << std::endl;
        std::cout << "y_pjoin:" << min_y_permJoin.as_string().c_str() << std::endl;
    }

    return correct;
}


// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::isInsertReversible1
(
    ItvMapTV& x, ItvMapTV& y, 
    ItvMapTV& y_perm, ItvMapTV& y_permJoin, 
    ItvMapTV& x_plus_y, ItvMapTV& x2
)
{
    // x + y - join(perm(y)) == x;
    
    ItvMapTV xx, yy, yy_perm, yy_permJoin, xx_plus_yy, xx2;

    m_ContainerGentor.some(xx);
    m_ContainerGentor.some(yy);
    m_ContainerGentor.last_permuted(yy_perm);
    yy_permJoin = yy_perm;
    yy_permJoin.join();

    xx2 = xx;
    xx2 += yy;
    xx_plus_yy = xx2;
    xx2 -= yy_permJoin;
    
    if(! xx.isEqual(xx2) ) {
        x = xx; y = yy; y_perm = yy_perm; y_permJoin = yy_permJoin;
        x_plus_y = xx_plus_yy; x2 = xx2 ;
        return false;
    } 
    else return true;
}    


template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testInsertReversibility1(int nTries, char* errFile)
{
    // x + join(perm(y)) - y == x;

    bool correct=true;
    ItvMapTV x, y, y_perm, y_permJoin, x_plus_y, x2;
    ItvMapTV min_x, min_y, min_y_perm, min_y_permJoin, min_x_plus_y, min_x2;
    int caseSize, min_caseSize;

    for(int i=0; i<nTries; i++) 
    {
        if(!isInsertReversible1(x, y, y_perm, y_permJoin, x_plus_y, x2))
        {
            caseSize = x.interval_count() + y.interval_count();
            if(true==correct) { 
                min_x = x; min_y = y; min_y_perm = y_perm; 
                min_y_permJoin = y_permJoin; min_x_plus_y = x_plus_y; min_x2 = x2;
                min_caseSize = caseSize;
                std::cout << "x.sz=" << x.interval_count() << " y.sz=" << y.interval_count() 
                     << "  try=" << i << std::endl;
                correct=false;
            }
            else if ( caseSize < min_caseSize )
            {
                min_x = x; min_y = y; min_y_perm = y_perm;
                min_y_permJoin = y_permJoin; min_x_plus_y = x_plus_y; min_x2 = x2;
                min_caseSize = caseSize;
                std::cout << "x.sz=" << x.interval_count() << " y.sz=" << y.interval_count() 
                     << "  try=" << i << std::endl;
            }
        }
    }

    if(!correct) {
        std::cout << "InsertReversibility1: x + y - join(perm(y)) == x =: x2  violated --------" << std::endl;
        std::cout << "x       :" << min_x.as_string().c_str() << std::endl;
        std::cout << "y       :" << min_y.as_string().c_str() << std::endl;
        std::cout << "y_perm  :" << min_y_perm.as_string().c_str() << std::endl;
        std::cout << "y_pJnt  :" << min_y_permJoin.as_string().c_str() << std::endl;
        std::cout << "x+y     :" << min_x_plus_y.as_string().c_str() << std::endl;
        std::cout << "x2      :" << min_x2.as_string().c_str() << std::endl;

        FILE* fp;
        fp = fopen(errFile,"w");
        fprintf(fp, "x: %s\n",      min_x.as_string().c_str());
        fprintf(fp, "y: %s\n",      min_y.as_string().c_str());
        fprintf(fp, "y_perm: %s\n", min_y_perm.as_string().c_str());
        fprintf(fp, "y_pJnt: %s\n", min_y_permJoin.as_string().c_str());
        fprintf(fp, "x+y: %s\n",    min_x_plus_y.as_string().c_str());
        fprintf(fp, "x2: %s\n",     min_x2.as_string().c_str());

        min_x.isEqual(min_x2);
        debugInsertReversibility1(min_x, min_y, min_y_perm);
    }
    else
    {
        std::cout << "InsertReversibility1("<<nTries<<") OK " 
             << " x + y - join(perm(y)) == x =: x2" << std::endl;
    }

    return correct;
}


template <class ItvMapTV>
void ItvMapTesterT<ItvMapTV>::debugInsertReversibility1
    (const ItvMapTV& x, const ItvMapTV& y, const ItvMapTV y_perm)
{
    ItvMapTV x2 = x, y_pj = y_perm;
    y_pj.join();

    x2 += y_pj;
    x2 -= y;
}



//-----------------------------------------------------------------------------

template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::isInsertReversible2
(
    ItvMapTV& x, ItvMapTV& y, 
    ItvMapTV& y_perm, ItvMapTV& y_permJoin, 
    ItvMapTV& x_plus_y_pj, ItvMapTV& x2
)
{
    // x + join(perm(y)) - y == x;
    
    ItvMapTV xx, yy, yy_perm, yy_permJoin, xx_plus_yy_pj, xx2;

    m_ContainerGentor.some(xx);
    m_ContainerGentor.some(yy);
    m_ContainerGentor.last_permuted(yy_perm);
    yy_permJoin = yy_perm;
    yy_permJoin.join();

    xx2 = xx;
    xx2 += yy_permJoin;
    xx_plus_yy_pj = xx2;
    xx2 -= yy;
    xx2.join();
    
    if(! is_element_equal(xx, xx2) ) {
        x = xx; y = yy; y_perm = yy_perm; y_permJoin = yy_permJoin;
        x_plus_y_pj = xx_plus_yy_pj; x2 = xx2 ;
        return false;
    } 
    else return true;
}    


template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testInsertReversibility2(int nTries, char* errFile)
{
    // x + join(perm(y)) - y == x;

    bool correct=true;
    ItvMapTV x, y, y_perm, y_permJoin, x_plus_y_pj, x2;
    ItvMapTV min_x, min_y, min_y_perm, min_y_permJoin, min_x_plus_y_pj, min_x2;
    size_t caseSize, min_caseSize;

    for(int i=0; i<nTries; i++) {
        if(!isInsertReversible2(x, y, y_perm, y_permJoin, x_plus_y_pj, x2))
        {
            caseSize = x.interval_count() + y.interval_count();
            if(true==correct) { 
                min_x = x; min_y = y; min_y_perm = y_perm; 
                min_y_permJoin = y_permJoin; min_x_plus_y_pj = x_plus_y_pj; 
                min_x2 = x2;
                min_caseSize = caseSize;
                std::cout << "x.sz="  << static_cast<unsigned int>(x.interval_count())
                     << " y.sz=" << static_cast<unsigned int>(y.interval_count())
                     << "  try=" << i << std::endl;
                correct=false;
            }
            else if ( caseSize < min_caseSize )
            {
                min_x = x; min_y = y; min_y_perm = y_perm;
                min_y_permJoin = y_permJoin; min_x_plus_y_pj = x_plus_y_pj; min_x2 = x2;
                min_caseSize = caseSize;
                std::cout << "x.sz="  << static_cast<unsigned int>(x.interval_count())
                     << " y.sz=" << static_cast<unsigned int>(y.interval_count()) 
                     << "  try=" << i << std::endl;
            }
        }
    }

    if(!correct) {
        std::cout << "InsertReversibility2: (x + join(perm(y)) - y).join() == x =: x2  violated --------" << std::endl;
        std::cout << "x       :" << min_x.as_string().c_str() << std::endl;
        std::cout << "y       :" << min_y.as_string().c_str() << std::endl;
        std::cout << "y_perm  :" << min_y_perm.as_string().c_str() << std::endl;
        std::cout << "y_pJnt  :" << min_y_permJoin.as_string().c_str() << std::endl;
        std::cout << "x+y_pJnt:" << min_x_plus_y_pj.as_string().c_str() << std::endl;
        std::cout << "x2      :" << min_x2.as_string().c_str() << std::endl;


        FILE* fp;
        fp = fopen(errFile,"w");
        fprintf(fp, "x: %s\n",      min_x.as_string().c_str());
        fprintf(fp, "y: %s\n",      min_y.as_string().c_str());
        fprintf(fp, "y_perm: %s\n", min_y_perm.as_string().c_str());
        fprintf(fp, "y_pJnt: %s\n", min_y_permJoin.as_string().c_str());
        fprintf(fp, "x+y: %s\n",    min_x_plus_y_pj.as_string().c_str());
        fprintf(fp, "x2: %s\n",     min_x2.as_string().c_str());

        debugInsertReversibility2(min_x, min_y, min_y_perm);
    }
    else
    {
        std::cout << "InsertReversibility2("<<nTries<<") OK " 
             << " x + join(perm(y)) - y == x =: x2" << std::endl;
    }

    return correct;
}


template <class ItvMapTV>
void ItvMapTesterT<ItvMapTV>::debugInsertReversibility2(const ItvMapTV& x, const ItvMapTV& y, const ItvMapTV y_perm)
{
    ItvMapTV x2 = x, y_pj = y_perm;
    y_pj.join();

    x2 += y_pj;
    x2 -= y;
}



//LAW: x -~ y == x * (dom(x) - y)
// operator -~ steht fuer erase
// x.erase(y) hat das gleiche Ergebnis wie der Schnitt von x mit x.domain() - y
template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::hasErasureAsIntersectionComputability(ItvMapTV& x, interval_set_type& y, 
    interval_set_type& xDom_minus_y, ItvMapTV& x_resid, ItvMapTV& x_sect)
{
    ItvMapTV xx, erasure, rhs;

    m_ContainerGentor.some(xx);
    m_ContainerGentor.some(erasure);

    interval_set_type erasureDomain;
    erasure.domain(erasureDomain);

    ItvMapTV xxResid = xx;
    xxResid -= erasureDomain;

    interval_set_type xxDomain;
    xx.domain(xxDomain);

    interval_set_type erasureComplement;
    erasureComplement = xxDomain;
    erasureComplement -= erasureDomain;

    ItvMapTV xxSect;
    xx.add_intersection(xxSect, erasureComplement);

    if(! is_element_equal(xxResid, xxSect) ) {
        x = xx; y = erasureDomain; xDom_minus_y = erasureComplement; 
        x_resid = xxResid; x_sect = xxSect;
        return false;
    } 
    else return true;
}



template <class ItvMapTV>
bool ItvMapTesterT<ItvMapTV>::testErasureAsIntersectionComputability(int nTries, char* errFile)
{
    bool correct=true;
    ItvMapTV x, x_resid, x_section;
    interval_set_type y, xDom_minus_y;

    ItvMapTV min_x, min_x_resid, min_x_section;
    interval_set_type min_y, min_xDom_minus_y;

    int caseSize, min_caseSize;

    for(int i=0; i<nTries; i++) {
        if(!hasErasureAsIntersectionComputability(x, y, xDom_minus_y, x_resid, x_section))
        {
            caseSize = static_cast<int>(x.interval_count() + y.interval_count());
            if(true==correct) { 
                min_x = x; min_y = y; 
                min_xDom_minus_y = xDom_minus_y;
                min_x_resid = x_resid; min_x_section = x_section;
                min_caseSize = caseSize;
                std::cout << "x.sz="  << static_cast<unsigned int>(x.interval_count())
                     << " y.sz=" << static_cast<unsigned int>(y.interval_count())
                     << "  try=" << i << std::endl;
                correct=false;
            }
            else if ( caseSize < min_caseSize )
            {
                min_x = x; min_y = y; 
                min_xDom_minus_y = xDom_minus_y;
                min_x_resid = x_resid; min_x_section = x_section;
                min_caseSize = caseSize;
                std::cout << "x.sz="  << static_cast<unsigned int>(x.interval_count())
                     << " y.sz=" << static_cast<unsigned int>(y.interval_count())
                     << "  try=" << i << std::endl;
            }
        }
    }

    if(!correct) {
        std::cout << "ErasureAsIntersectionComputability: x -~ y == x * (dom(x)-y) violated --------" << std::endl;
        std::cout << "x       :" << min_x.as_string().c_str() << std::endl;
        std::cout << "y       :" << min_y.as_string().c_str() << std::endl;


        FILE* fp;
        fp = fopen(errFile,"w");
        fprintf(fp, "x: %s\n",      min_x.as_string().c_str());
        fprintf(fp, "y: %s\n",      min_y.as_string().c_str());

        //debugInsertReversibility2(min_x, min_y, min_y_perm);
    }
    else
    {
        std::cout << "ErasureAsIntersectionComputability("<<nTries<<") OK " 
             << " x -~ y == x * (dom(x)-y)" << std::endl;
    }

    return correct;
}

}} // namespace itl boost

#endif


