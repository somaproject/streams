/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_LAW_VIOLATIONS_HPP_JOFA_070411
#define BOOST_ITL_LAW_VIOLATIONS_HPP_JOFA_070411

#include <string>
#include <boost/itl/set.hpp>
#include <boost/validate/loki_xt/Tuple.h>

namespace boost{namespace itl
{

    class LawViolationsI
    {
    public:
        virtual ~LawViolationsI(){}

        virtual size_t size()const=0;
        virtual size_t getViolationsCount()const=0;
        virtual LawViolationsI& operator += (const LawViolationsI& rhs)=0;

        virtual const LawViolationsI* getLawViolations()const=0;

        virtual bool operator == (const LawViolationsI& rhs)const=0;
        virtual bool operator <  (const LawViolationsI& rhs)const=0;

        virtual void reportFirst()const=0;

    };

    class PolyLawViolations
    {
    public:
        PolyLawViolations(): p_violations(NULL){}
        PolyLawViolations(LawViolationsI* vios): p_violations(vios) {}
        ~PolyLawViolations() {} 

        void destroy() { delete p_violations; p_violations = NULL; }

        size_t size()const { return p_violations ? p_violations->size() : 0; }
        size_t getViolationsCount()const { return p_violations ? p_violations->getViolationsCount() : 0; }

        PolyLawViolations& operator += (const PolyLawViolations& rhs) 
        {
            if(p_violations == NULL)
                p_violations = rhs.p_violations;
            else if(rhs.p_violations != NULL)
                (*p_violations) += *(rhs.p_violations); 
            
            return *this; 
        } 

        bool operator == (const PolyLawViolations& rhs)const
        {
            return     (p_violations == NULL && rhs.p_violations == NULL)
                    || (
                           ( p_violations != NULL && rhs.p_violations != NULL)
                         &&( p_violations == rhs.p_violations
                             || *p_violations == (*rhs.p_violations))
                       );
        }

        bool operator < (const PolyLawViolations& rhs)const
        {
            return    (p_violations != NULL && rhs.p_violations != NULL)
                   && (*p_violations < (*rhs.p_violations));
        }

        void reportFirst()const
        {
            if(p_violations == NULL)
                std::cout << "Empty law violation." << std::endl;
            else
                p_violations->reportFirst();
        }

    private:
        LawViolationsI* p_violations;
    };


    /** class LawViolations.
        LawViolations is a set of instances of a law that are violated sorted by operator <.
        Operator < will usually be implemented via the size of the laws instance.
        We are always only interested in small instances of law violations for efficient 
        debugging. */
    template <class LawT>
    class LawViolations : public LawViolationsI
    {
    public:
        typedef LawT LawType;
        typedef typename itl::set<LawType> ViolationSet;
        typedef typename ViolationSet::size_type size_type;

        typedef typename ViolationSet::iterator iterator;
        typedef typename ViolationSet::const_iterator const_iterator;

        enum { MaxSize = 20 };

        LawViolations():_maxSize(MaxSize), _violationsCount(0){}

        const LawViolationsI* getLawViolations()const { return this; }

        size_t getViolationsCount()const { return _violationsCount; }

        LawViolationsI& operator += (const LawViolationsI& rhs) 
        { 
            ITL_const_FORALL(typename ViolationSet, vio_, 
                dynamic_cast<const LawViolations<LawT>*>(rhs.getLawViolations())->_violations)
                insert(*vio_);

            return *this; 
        } 

        bool operator == (const LawViolationsI& rhs)const
        {
            return _violations == dynamic_cast<const LawViolations<LawT>*>(rhs.getLawViolations())->_violations;
        }

        bool operator < (const LawViolationsI& rhs)const
        {
            return _violations < dynamic_cast<const LawViolations<LawT>*>(rhs.getLawViolations())->_violations;
        }

        void insert(const LawType& lawInstance) 
        {
            _violations.insert(lawInstance);
            _violationsCount++;
            if(0 < _violations.size() && _maxSize < static_cast<int>(_violations.size()))
            {
                typename ViolationSet::iterator doomed_ = _violations.end();
                doomed_--;
                _violations.erase(doomed_);
            }
        }

        iterator       begin()      { return _violations.begin(); }
        const_iterator begin()const { return _violations.begin(); }
        iterator       end()        { return _violations.end(); }
        const_iterator end()const   { return _violations.begin(); }

        void clear()       { _violations.clear(); }
        bool empty()const  { return _violations.empty(); }
        size_type size()const { return _violations.size(); }

        void reportFirst()const
        {
            typename ViolationSet::const_iterator fst = _violations.begin();
            LawT violation = *(_violations.begin());

            typename LawT::input_tuple  inVars;
            typename LawT::output_tuple outVars;
            violation.getInstance(inVars, outVars);
            std::cout << "Violation of: " << violation.typeString() << std::endl;
            std::cout <<  inVars.as_string() << std::endl;
            std::cout << outVars.as_string() << std::endl;
        }


    private:
        ViolationSet _violations;
        int          _maxSize;
        size_t       _violationsCount;
    };


template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, const PolyLawViolations& object)
{
    return stream << "operator<<: not implemented for itl::PolyLawViolations!";
}


}} // namespace itl boost

#endif //BOOST_ITL_LAW_VIOLATIONS_HPP_JOFA_070411

