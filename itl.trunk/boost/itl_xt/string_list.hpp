/*-----------------------------------------------------------------------------+    
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+-----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------
class itl::string_list
    A List of Strings.
    Can be useful for string split and join.
    Also simple text-formating can be done. 
--------------------------------------------------------------------*/
#ifndef BOOST_ITL_STRING_LIST_HPP_JOFA_990603
#define BOOST_ITL_STRING_LIST_HPP_JOFA_990603

#include <string>    
#include <list>    
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/itl_list.hpp>

namespace boost{namespace itl
{    
    
/// providing split, join and basic text processing
/** class string_list implements a list of strings that provide split and 
    join functions and basic text formatting.

    @author Joachim Faulhaber
*/
class string_list:    public itl::list<std::string> //JODO URG no inhertiance from base container
{
public:
    /** Empty string_list (default-Ctor) */
    string_list(): list<std::string>() {}

    /** Construction from an array of strings and an int 'nOfDeno' which must be the 
     number of array elements */
    string_list(const char* const deno[], int nOfDeno);

    /** Split stringT arg at spearators 'separator' to buid string_list *this.
        Returns the number of empty strings within the List
    */
    int split(const std::string& arg, const char* separator = " ");

    /** Works like split but sort out empty strings.
        Returns the number of empty strings that have been sorted out.
    */
    int separate(const std::string& arg, const char* separator = " ");

    /** Separates for ANY character, which is found in the separator string. */
    int separateForAny(const std::string& arg, const char* separator = " ");

    //The resulting list contains the sequence of null-terminated strings
    //out of the buffer txtbuf, limited by the maximal length maxlen, which
    //can be omitted by specifying -1
    //buffer layout: txt1\0txt2\0\txt3\0\0 <-- double null byte marks end
    //return value: number of strings found
    int separateForNull(const char * txtbuf, int maxlen = -1);
    
    /* concatenate all strings of this list inserting 'separator' between elements */
    std::string join(const char* separator = " ")const;
    /* concatenate all strings like join but sort out empty elements */
    std::string merge(const char* separator = " ")const;
    
    /* split all blank-separated words in stringT text into this list of words.
       Words exceeding maxLen are seperated and a hyphen (-) is inserted         */
    void wordify(const std::string& text, int maxLen);

    /* break down a list of words into a text paragraph of width maxLen */ 
    void lineify(string_list& words, int maxLen);

    /* break down a flat list of words into this text paragraph of width maxLen */
    void format(const std::string& flatText, int maxLen);

    /** get the 0 based index of that element, compare ignoring case */
    int index_IgnoreCase(const std::string& text)const;    

    /** pop n elements from the front */
    string_list& popFront(int n);

    /** pop k elements from the front to keep at least n elements from the back*/
    string_list& keepBack(int n);

    /** Set 'count' tail-Elements to empty strings */
    string_list& emptyTail(int count);
} ;

}} // namespace boost itl

#endif


