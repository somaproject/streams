/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------
class itl::list
    a general list class that extends stl-lists
    for concepts InplaceAddable and InplaceSubtractable
--------------------------------------------------------------------*/
#ifndef BOOST_ITL_LIST_HPP_JOFA_070519
#define BOOST_ITL_LIST_HPP_JOFA_070519

#include <string>
#include <list>
#include <algorithm>
#include <boost/assert.hpp>
#include <boost/itl_xt/std/pair.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/set_algo.hpp>
#include <boost/itl/predicates.hpp>


namespace boost{namespace itl
{
    //JODO 1_0_1 documentation
    /// an stl based list implementing inplace addition operators += 
    /** 

        @author Joachim Faulhaber
    */
    template <typename DataT, ITL_ALLOC Alloc = std::allocator>
    class list: private std::list<DataT, Alloc<DataT> >
    {
    public:
        typedef DataT        value_type;
        typedef Alloc<DataT> allocator_type;

    private:
        typedef typename itl::list<DataT, Alloc>           type;
        typedef typename std::list<DataT, allocator_type>  base_type;

    public:        
        typedef typename base_type::pointer                pointer;
        typedef typename base_type::reference              reference;
        typedef typename base_type::const_reference        const_reference;
        typedef typename base_type::iterator               iterator;
        typedef typename base_type::const_iterator         const_iterator;
        typedef typename base_type::size_type              size_type;
        typedef typename base_type::difference_type        difference_type;
        typedef typename base_type::reverse_iterator       reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        
    public:
        list(){}
        list(size_type size): base_type(size){}
        list(size_type size, const DataT value): base_type(size, value){}

        template <class InputIterator>
        list(InputIterator f, InputIterator l): base_type(f,l) {}

        list(const list& src): base_type::list(src){}

        list& operator=(const list& src) { base_type::operator=(src); return *this; } 
        void swap(list& src) { base_type::swap(src); }

        using base_type::begin;
        using base_type::end;
        using base_type::rbegin;
        using base_type::rend;

        using base_type::size;
        using base_type::max_size;
        using base_type::empty;

        using base_type::front;
        using base_type::back;
        using base_type::push_front;
        using base_type::push_back;
        using base_type::pop_front;
        using base_type::pop_back;

        using base_type::insert;
        using base_type::erase;
        using base_type::clear;

        using base_type::resize;
        using base_type::splice;

        using base_type::remove;
        using base_type::unique;
        using base_type::merge;
        using base_type::sort;

    public:
        // --------------------------------------------------------------------
        // itl specific extensions
        // --------------------------------------------------------------------

        /** Find the element \c x and return a \c const_iterator to that element, 
            if it exists. If \c is not found in the list return <tt>const_iterator 
            end()</tt> */
        const_iterator find(const DataT& x)const { return std::find(begin(), end(), x); }

        /** Find the element \c x and return a \c iterator to that element, 
            if it exists. If \c is not found in the list return <tt>iterator 
            end()</tt> */
        iterator       find(const DataT& x)      { return std::find(begin(), end(), x); }

        /// Checks if the element \c x is in the list
        bool contains(const DataT& x)const { return !(find(x) == end()); }

        /** <tt>this->contained_in(super)</tt>, if \c *this is a list that is contained as 
            sublist in the list \c super */
        bool contained_in(const list& super)const { return is_subsequence(super); }

        size_t iterative_size()const{ return size(); }

        //JODO concept InplaceAddable
        /// Add a list \c tail to this list.
        list& operator += (const list& tail) { splice(end(), list(tail)); return *this; }

        /** yields true if any of the elements in \c *this has the property \c prop */
        bool any(const property<DataT>& prop)const;

        //JODO concept StringRepresentable
        /** Represent this list as a string */
        std::string as_string(const char* sep = " ")const;

        /** Function add is mainly a convenience function allowing for a compact
            denotation of lists like in:
            <code>
            list<std::string> note;
            note.add("A").add("compact").add("denotation").add("for").add("lists");
            </code>
        */
        list& add(const DataT& val) { push_back(val); return *this; }


        // --------------------------------------------------------------------
        //JODO: sequence specific algorithms should be moved to a file seq_algo
        // just as this has been done for sets. seq_algos can then be applied
        // to other containers, that share sequence properties, like e.g. vectors

        /** Check, if every element of the list occurs only once */
        bool is_unique()const;

        /** Insert sequence \c seq as a \e consequent subsequence into \c *this.

            Whereby \c sub is a <em>consequent subsequence</em> of \c super, if
            \c sub is contained as a (possibly non contingent) subsequence of
            \c super without permuting elements:

            <code>
            super: my big new ball is light and blue
            sub1 : my     new ball is           blue
            sub2 : my big     ball is light
            nosub: my         ball is big            // NO consequent subsequence
            </code>
        */
        void interlace(const list& seq);


        /** \c *this is supersequence of \c subSeq, if \c subSeq is 
            contained in \c *this without permutations of elements. */
        bool is_supersequence(const list& subSeq)const;

        /** \c *this is subsequence of \c superSeq, if \c *this is 
            contained in \c superSeq without permutations of elements. */
        bool is_subsequence(const list& superSeq)const
        { return superSeq.is_supersequence(*this); }

        /** \c *this is consequent to \c seq, if there are no elements in \c *this that
            occur in different order in \c seq */
        bool is_consequent(const list& seq)const;

        /** \c perm is a permutation of \c seq that is consequent to \c *this */
        void consequent_permutation(list& perm, list& seq);

        iterator first_common_element(iterator& beg1_, iterator& end1_, 
                                    iterator& beg2_, iterator& end2_)const;

    private:
        void interlace(list& seq, typename list::iterator& inPos);
        bool is_supersequence(list& seq, typename list::const_iterator& pos)const;
        bool is_consequent(list& seq, typename list::const_iterator& pos)const;
        void consequent_permutation(list& perm, iterator& perm_, list& seq, iterator& seq_);

    };


    template <typename DataT, ITL_ALLOC Alloc>
    inline bool operator == (const itl::list<DataT,Alloc>& lhs,
                             const itl::list<DataT,Alloc>& rhs)
    {
        typedef std::list<DataT,Alloc<DataT> > base_type;
        return operator==((const base_type&)lhs, (const base_type&)rhs);
    }
    
    template <typename DataT, ITL_ALLOC Alloc>
    inline bool operator < (const itl::list<DataT,Alloc>& lhs,
        const itl::list<DataT,Alloc>& rhs)
    {
        typedef std::list<DataT,Alloc<DataT> > base_type;
        return operator<((const base_type&)lhs, (const base_type&)rhs);
    }

    template <typename DataT, ITL_ALLOC Alloc>
    inline bool operator <= (const itl::list<DataT,Alloc>& lhs,
        const itl::list<DataT,Alloc>& rhs)
    {
        typedef std::list<DataT,Alloc<DataT> > base_type;
        return operator<=((const base_type&)lhs, (const base_type&)rhs);
    }

    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::is_unique()const 
    {
        //JODO implement via std-algorithm of directly
        list self = *this;
        self.unique();
        return (*this) == self;
    }

    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::is_supersequence(const list& subSeq)const
    {
        list sub = subSeq;
        typename list::const_iterator pos = begin();
        return is_supersequence(sub, pos);
    }

    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::is_supersequence(list& subtee, const_iterator& pos)const
    {
        if(subtee.empty())
            return true;
        else
        {
            DataT fst = *subtee.begin();
            const_iterator val_ = std::find(pos, end(), fst);
            if(val_==end())
                return false;
            else
            {
                subtee.pop_front();
                return is_supersequence(subtee, ++val_);
            }
        }
    }


    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::is_consequent(const list& conSeq)const
    {
        list seq = conSeq;
        typename list::const_iterator pos = begin();
        return is_consequent(seq, pos);
    }


    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::is_consequent(list& seq, const_iterator& pos)const
    {
        if(seq.empty())
            return true;
        else
        {
            DataT fst = *seq.begin();
            const_iterator val_ = std::find(begin(), pos, fst);
            if(val_==end())
            {
                seq.pop_front();
                return this->is_consequent(seq, ++val_);
            }
            else
                return false;

        }
    }

    template <typename DataT, ITL_ALLOC Alloc>
    void list<DataT,Alloc>::consequent_permutation(list& perm, list& seq)
    {
        perm.clear();
        iterator this_ = begin();
        iterator seq_  = seq.begin();
        consequent_permutation(perm, this_, seq, seq_);
    }

    template <typename DataT, ITL_ALLOC Alloc>
    void list<DataT,Alloc>::consequent_permutation(list& perm, iterator& this_, list& seq, iterator& seq_)
    {
        if(seq_ == seq.end())
            return;
        else
        {
            DataT cand = *seq_;
            iterator cur_ = std::find(begin(), this_, cand);
            if(cur_ == this_)
            {
                // Next cand is not found in 'front' of *this. Oh happy day.
                // No element swapping is necessary. We can collect 'cand'
                perm.push_back(cand); seq_++;

                // See if 'cand' is found in rear
                cur_ = std::find(this_, end(), cand);

                if(cur_ == end())
                    consequent_permutation(perm, this_, seq, seq_);
                else
                { 
                    this_ = cur_; 
                    consequent_permutation(perm, this_, seq, seq_); 
                }
            }
            else
            {
                iterator fromThis_ = cur_; fromThis_++;
                iterator perm_ = first_common_element(fromThis_, end(), perm.begin(), perm.end());
                if(perm_ == perm.end())
                    perm.push_front(cand);
                else
                    perm.insert(perm_, cand);
                seq_++;
                consequent_permutation(perm, this_, seq, seq_); 
            }
        }
    }


    template <typename DataT, ITL_ALLOC Alloc>
    typename list<DataT,Alloc>::iterator list<DataT,Alloc>::first_common_element
        (iterator& beg1_, iterator& end1_, iterator& beg2_, iterator& end2_)const
    {
        iterator it1_, it2_ = beg2_;
        for(it1_=beg1_; it1_ != end1_; it1_++)
            for(it2_=beg2_; it2_ != end2_; it2_++)
                if(*it1_ == *it2_)
                    return it2_;

        return it2_;
    }


    template <typename DataT, ITL_ALLOC Alloc>
    void list<DataT,Alloc>::interlace(const list& inList)
    {
        BOOST_ASSERT(is_unique());
        BOOST_ASSERT(inList.is_unique());
        list seq = inList;
        iterator inPos = begin();
        interlace(seq, inPos);
    }

    template <typename DataT, ITL_ALLOC Alloc>
    void list<DataT,Alloc>::interlace(list& seq, iterator& inPos)
    {
        if(seq.empty())
            return;
        else
        {
            const DataT& cand = *seq.begin();
            iterator cur_ = std::find(inPos, end(), cand);

            if(cur_ == end())
            {
                // cand war nirgends in *this Liste
                insert(inPos, cand);
                seq.pop_front();
                interlace(seq, inPos);
            }
            else
            {
                // cand ist schon in rear also auch in master. Die Einfuegeposition
                // wird hinter das eingefuegte Element cand hochgezogen. Dabei ueberspringen
                // wir allerdings Elemente, die spaeter in front gesucht werden muessen
                seq.pop_front();
                interlace(seq, ++cur_);
            }
        }
    }


    // THINK: here codereplication occurs at the next level of abstraction (cf. SetT)
    template <typename DataT, ITL_ALLOC Alloc>
    bool list<DataT,Alloc>::any(const property<DataT>& pred)const
    {
        const_iterator it = begin();
        while(it != end() && ! pred(*it)) ++it;
        if(it==end()) return false; else return true;
    }


    template <typename DataT, ITL_ALLOC Alloc>
    std::string list<DataT,Alloc>::as_string(const char* sep /* =" " */)const
    {
        const_iterator it=begin();
        if(it == end()) return std::string("");
        else
        {
            std::string y = "{"+to_string<DataT>::apply(*it);
            it++;
            while(it != end()) 
            { 
                y+=sep; 
                y += to_string<DataT>::apply(*it); 
                it++; 
            }
            y += "}";
            return y;
        }
    }

//==============================================================================
//= Streaming
//==============================================================================
template<class CharType, class CharTraits, 
    class DataT, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
  const itl::list<DataT,Alloc>& object)
{
    typedef itl::list<DataT,Alloc> ObjectT;
    stream << "[";
    typename ObjectT::const_iterator it = object.begin();
    if(it != object.end())
        stream << *it++;
    while(it != object.end())
        stream << " " << *it++;
    return stream << "]";
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class Type>
struct type_to_string<itl::list<Type> >
{
    static std::string apply()
    { return "list<"+ type_to_string<Type>::apply() +">"; }
};

}} // namespace itl boost

#endif // BOOST_ITL_LIST_HPP_JOFA_070519

