/*-----------------------------------------------------------------------------+    
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
#ifndef __var_tuple_inscriptor_JOFA_041006_H__
#define __var_tuple_inscriptor_JOFA_041006_H__

#include <boost/itl/string_list.hpp>
#include <boost/itl/map.hpp>
#include <boost/itl/var_permutation.hpp>
#include <boost/itl/var_tuple.hpp>

namespace boost{namespace itl
{


    template <int varCountV>
    class var_tuple_inscriptor
    {
    private:
        typedef var_tuple<varCountV> var_tupleT;
        typedef var_permutation<varCountV> var_permutationT;
        typedef MapT<int,int> OmissionMapTD;

    public:
        /** Die Permutation sorgt daf�r, dass die Auswahl und Reihenfolge der Beschriftung
            ge�ndert werden kann. */
        void setPermutation(const var_permutationT& perm) { m_Perm = perm; }
        var_permutationT getPermutation()const { return m_Perm; }

        /** Konvertiere einen Tupel von Werten unabh�ngiger Variablen in eine StringListe
            von Beschriftungen: Diese dient als Zeilenbeschriftung f�r Statistikbl�tter
            der Excelausgabe.

            'changeVar' ist der Index der Variablen, die sich bei einer Iteration aendert.
            So haben wir die Moeglichkeit nur VariablenLabels bei Aenderung auszugeben.
            changeVar=0 fuehrt dazu, dass alle Variablenabels fuer ein Tulel ausgegeben
            werden.
        */
        virtual void getValueLabels(StringListT& valueLabels, const var_tupleT& tup, 
                                    VarEnumTD changeVar, int grade)const=0;

        /** Die Liste der Variablentitel anf�gen */
        virtual void addVarTitles(StringListT& headers)const=0;

        /** Nach der Variable 'permVar' werden 'count' spalten leer gelassen */
        void addOmission(int permVar, int count)
        { m_Omissions[permVar] = count; }

        int omissionCount(int permVar)const 
        { 
            OmissionMapTD::const_iterator count_ = m_Omissions.find(permVar);
            if(count_ == m_Omissions.end())
                return 0;
            else
                return (*count_).second;
        }

    protected:
        var_permutationT m_Perm;
        OmissionMapTD     m_Omissions;
    };

}} // namespace itl boost

#endif // __var_tuple_inscriptor_JOFA_041006_H__


