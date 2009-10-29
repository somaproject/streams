/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_VALIDATION_COUNTS_HPP_JOFA_090713
#define BOOST_ITL_VALIDATION_COUNTS_HPP_JOFA_090713


namespace boost{namespace itl
{

class validation_counts
{
public:
    validation_counts():_time(0.0), _count(0){}
    validation_counts(double sum): _time(sum),_count(1){}

    double time()const     {return _time;}
    long   count()const    {return _count;}
    double average()const{ return _count==0 ? 0.0 : _time/_count; }

    validation_counts& operator += (const validation_counts& right)
    { 
        _time += right.time(); 
        _count += right.count(); 
        return *this; 
    }

private:
    double _time;
    long   _count;
};

bool operator == (const validation_counts& left, const validation_counts& right)
{ return left.time()==right.time() && left.count()==right.count(); } 

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, const validation_counts& object)
{
    return stream << "(time=" << object.time() << "count=" << object.count() << ")";
}



}} // namespace itl boost

#endif // BOOST_ITL_VALIDATION_COUNTS_HPP_JOFA_090713
