/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

namespace boost{namespace itl
{

/** Time is a toy-class to demonstrate a class that conforms the requirements of
    a template parameter for class IntervalT. 

    In real world applications you may want to use the integer representation of a
    time variable. That way intervals and their containers are working most efficiently. 
*/

enum {sunday=0, monday, tuesday, wednesday, thursday, friday, saturday};
static const char* daynames[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

class Time
{
public:
    Time(): m_time(0) {}
    Time(int hours, int minutes): m_time(60*hours+minutes) {}
    Time(int day, int hours, int minutes): m_time((24*60)*day+60*hours+minutes) {}
    int getDay()const { return m_time/(24*60); }
    int getHours()const { return (m_time%(24*60))/60; }
    int getMinutes()const { return (m_time%(24*60))%60; }
    int asInt()const { return m_time; }
    std::string getDayString()const { return daynames[getDay()]; }

    std::string as_string()const
    {
        const int MAX_TIMESTING_LEN = 256;
        char repr[MAX_TIMESTING_LEN];
        sprintf(repr, "%3s:%02d:%02d", getDayString().c_str(), getHours(), getMinutes());
        return std::string(repr);
    }

    Time& operator ++ () { m_time++; return *this; }
    Time& operator -- () { m_time--; return *this; }

private:
    int m_time;
};


bool operator < (const Time& x1, const Time& x2) { return x1.asInt() < x2.asInt(); } 
bool operator == (const Time& x1, const Time& x2) { return x1.asInt() == x2.asInt(); } 
bool operator <= (const Time& x1, const Time& x2) { return x1.asInt() <= x2.asInt(); } 

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits> &stream, Time const& time)
{
    return stream << time.as_string();
}

}} // namespace itl boost

