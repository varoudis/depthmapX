// Paf Template Library --- a set of useful C++ templates
//
// Copyright (c) 1996-2006 PAF Turner (a.turner@ucl.ac.uk)
//
//-----------------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  See the lgpl.txt file for details
//-----------------------------------------------------------------------------
//
// Paf's cross platform box of tricks
// Everything you need to write any C++.  All in one file!
//
// pstring     similar to STL string
// pmemvec     base clase for pvector and prefvec
// pvector     similar to STL vector
// prefvec     pvector with a different allocator (vector of references)
// pqvector    searchable prefvec
// pqmap       a simple map class, based on a binary tree
// plist       a simple list class
// ptree       a simple tree template
// pflipper    used for flipping between two vectors (or anythings...)
// pexception  exception class, base for various exception types
//
//
// A3 eliminates the double referencing used previously in the 
// vector classes

#ifndef __PAFTL_H__
#define __PAFTL_H__

#define PAFTL_DATE "19-MAR-2006"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <fstream.h>

#ifndef bool
//   #define bool int
#endif
#ifndef true
   #define true 1
#endif
#ifndef false
   #define false 0
#endif

///////////////////////////////////////////////////////////////////////////////

// namespace paftl {

class pexception;
class pstring;
template <class T> class pvector;
template <class T> class prefvec;
template <class T> class pqvector;
template <class T1, class T2> class pqmap;
template <class T> class plist;
template <class T> class ptree;
template <class T> class pflipper;

// a few basic types

typedef pvector<int>       pvecint;
typedef pvector<float>     pvecfloat;
typedef pvector<double>    pvecdouble;
typedef pqvector<pstring>  pvecstring;

///////////////////////////////////////////////////////////////////////////////

// miscellaneous enums (paftl used as namespace)

class paftl
{
public:
   enum add_t {ADD_UNIQUE, ADD_REPLACE, ADD_DUPLICATE};
};

///////////////////////////////////////////////////////////////////////////////

class pexception
{
public:
   enum exception_t { UNDEFINED         = 0x0000, 
                      MEMORY_ALLOCATION = 0x0001,
                      FILE_ERROR        = 0x0002 };

protected:
   int m_exception;

public:
   pexception(int n_exception = UNDEFINED)
      { m_exception = n_exception; }
   int error_code() 
      { return m_exception; }
};


///////////////////////////////////////////////////////////////////////////////

// pmemvec: base allocation for pvector and prefvec

template <class T> class pmemvec
{
public:
   class exception : public pexception
   {
   public:
      enum exception_t { PVECTOR_UNDEFINED   = 0x1000,
                         EMPTY_VECTOR        = 0x1001, 
                         UNASSIGNED_ITERATOR = 0x1002,
                         OUT_OF_RANGE        = 0x1003 };
   public:
      exception(int n_exception = PVECTOR_UNDEFINED) : pexception( n_exception ) {}
   };
   enum base_t { npos = -1 };
protected:
   T *m_data;
   short m_shift;
   int m_length;
public:
   // redefine
   pmemvec(int sz = 0);
   pmemvec(const pmemvec<T>& );
   virtual ~pmemvec();
   pmemvec<T>& operator = (const pmemvec<T>& );
   //
   virtual void push_back(const T& item);
   virtual void pop_back();
   virtual void remove_at(int pos = 0);
   virtual void remove_at(const pvecint& list);
   virtual void insert_at(int pos, const T& item);
   //
   virtual void set(int count);
   virtual void set(const T& item, int count);
   //
   virtual void clear();
   virtual void clearnofree();
protected:
   int storage_size() const
      { return m_shift ? (2 << m_shift) : 0; }
   void grow(int pos);
   void shrink();
public:
   int size() const
      { return m_length; }
   T& base_at(int pos) 
      { return m_data[pos]; }
   const T& base_at(int pos) const
      { return m_data[pos]; }
public:
   istream& read( istream& stream, streampos offset = -1 );
   ostream& write( ostream& stream );
};

template <class T>
pmemvec<T>::pmemvec(int sz)
{ 
   // note: uses same as grow / storage_size, but cannot rely on function existence when calling constructor
   if (sz == 0) {
      m_data = NULL; 
      m_shift = 0; 
   }
   else {
      do {
         m_shift++;
      } while ((2 << m_shift) < sz);
      m_data = new T [(2 << m_shift)];
   }
   m_length = 0;
}

template <class T>
pmemvec<T>::pmemvec(const pmemvec& v)
{
   m_shift  = v.m_shift;
   m_length = v.m_length;

   if (m_shift) {
      m_data = new T [storage_size()];
      if (!m_data)
         throw pexception( pexception::MEMORY_ALLOCATION );
      
      if (m_length) {
         for (int i = 0; i < m_length; i++)
            m_data[i] = v.m_data[i];
      }
   }
   else {
      m_data = NULL;
   }
}

template <class T>
pmemvec<T>::~pmemvec()
{
   if (m_data)
   {
      delete [] m_data;
      m_data = NULL;
   }
}

template <class T>
pmemvec<T>& pmemvec<T>::operator = (const pmemvec<T>& v)
{
   if (m_shift < v.m_shift) 
   {
      if (m_shift != 0) {
         delete [] m_data;
      }
      m_shift = v.m_shift;
      if (m_shift) {
         m_data = new T [storage_size()];
         if (!m_data)
            throw pexception( pexception::MEMORY_ALLOCATION );
      }
      else {
         m_data = NULL;
      }
   }
   m_length = v.m_length;
   if (m_length) {
      for (int i = 0; i < m_length; i++)
         m_data[i] = v.m_data[i];
   }
   return *this;
}

template <class T>
void pmemvec<T>::push_back(const T& item)
{
   if (m_length >= storage_size()) {
      grow( m_length );
   }
   m_data[m_length++] = item;
}

template <class T>
void pmemvec<T>::pop_back()
{
   if (m_length == 0)
      throw exception( exception::EMPTY_VECTOR );

   --m_length;

   // Preferably include shrink code here
}

template <class T>
void pmemvec<T>::insert_at(int pos, const T& item)
{
   if (pos == npos || pos > m_length) {
      throw exception( exception::OUT_OF_RANGE );
   }
   if (m_length >= storage_size()) {
      grow( pos );
   }
   else {
      for (int i = m_length; i > pos; i--) {
         m_data[i] = m_data[i - 1];
      }
   }
   m_data[pos] = item;
   m_length++;
}

template <class T>
void pmemvec<T>::remove_at(int pos)
{
   // This is a simple but reliable remove item from vector
   if (m_length == 0)
      throw exception( exception::EMPTY_VECTOR );
   else if (pos == npos || pos >= m_length) 
      throw exception( exception::OUT_OF_RANGE );
   
   for (int i = pos; i < m_length - 1; i++) {
      m_data[i] = m_data[i + 1];
   }
   --m_length;

   // Preferably include shrink code here
}

// this version for bulk deletes: copies over entries to new list
template <class T>
void pmemvec<T>::remove_at(const pvecint& list)
{
   if (m_length == 0)
      throw exception( exception::EMPTY_VECTOR );
   if (list.size() >= m_length) {
      // if the list does not contain duplicates, then this simply means delete all contents:
      clear();
      return;
   }
   // shrink new vector:
   while ((m_length - list.size()) * 2 < storage_size()) {
      m_shift--;
   }
   int new_length = 0;
   T *new_data = new T [storage_size()];
   bool *rem_flag = new bool [m_length];
   if (!new_data || !rem_flag)
      throw pexception( pexception::MEMORY_ALLOCATION );
   for (int i = 0; i < m_length; i++) {
      rem_flag[i] = false;
   }
   for (i = 0; i < list.size(); i++) {
      if (list[i] == npos || list[i] >= m_length)
         throw exception( exception::OUT_OF_RANGE );
      rem_flag[list[i]] = true;
   }
   for (i = 0; i < m_length; i++) {
      if (!rem_flag[i]) {
         new_data[new_length] = m_data[i];
         new_length++;
      }
   }
   m_length = new_length;
   delete [] m_data;
   delete [] rem_flag;
   m_data = new_data;
}

template <class T>
void pmemvec<T>::set(int count)
{
   clear();
   do {
      m_shift++;
   } while ((2 << m_shift) < count);
   m_data = new T [(2 << m_shift)];
   m_length = count;
}

template <class T>
void pmemvec<T>::set(const T& item, int count)
{
   clear();
   do {
      m_shift++;
   } while ((2 << m_shift) < count);
   m_data = new T [(2 << m_shift)];
   m_length = count;
   for (int i = 0; i < m_length; i++) {
      m_data[i] = item;
   }
}

template <class T>
void pmemvec<T>::clear()
{
   m_length = 0;
   m_shift = 0;
   if (m_data)
   {
      delete [] m_data;
      m_data = NULL;
   }
}

template <class T>
void pmemvec<T>::clearnofree()
{
   m_length = 0;
}

template <class T>
void pmemvec<T>::grow(int pos)
{        
   m_shift++;

   T *new_data = new T [storage_size()];
   if (!new_data)
      throw pexception( pexception::MEMORY_ALLOCATION );

   if (m_length) {
      for (int i = 0; i < m_length + 1; i++)
         new_data[i] = (i < pos) ? m_data[i] : m_data[i-1];
   }
   if (m_data) {
      delete [] m_data;
   }
   m_data = new_data;
}

template <class T>
void pmemvec<T>::shrink()
{
}

template <class T>
istream& pmemvec<T>::read( istream& stream, streampos offset )
{
   int length;
   if (offset != -1) {
      stream.seekg( offset );
   }
   stream.read( (char *) &length, sizeof(int) );
   if (length >= storage_size()) {
      if (m_data) {
         delete [] m_data;
         m_data = NULL;
      }
      while (length >= storage_size())
         m_shift++;
      m_data = new T [storage_size()];
      if (!m_data)
         throw pexception( pexception::MEMORY_ALLOCATION );
   }
   m_length = length;
   if (m_length > 0) {
      stream.read( (char *) m_data, sizeof(T) * m_length );
   }
   return stream;
}

template <class T>
ostream& pmemvec<T>::write( ostream& stream )
{
   stream.write( (char *) &m_length, sizeof(int) );
   if (m_length != 0) {
      stream.write( (char *) m_data, sizeof(T) * m_length );
   }
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

template <class T> class pvector : public pmemvec<T>
{
protected:
   mutable int m_current;
public:
   pvector(int sz = 0) : pmemvec<T>(sz)
      {m_current = npos;}
   pvector(const pvector<T>& v) : pmemvec<T>(v)
      {m_current = v.m_current;}
   pvector<T>& operator = (const pvector<T>& );
   virtual ~pvector()
      {;}
   //
   T& at(int pos)
      { return m_data[pos]; }
   const T& at(int pos) const
      { return m_data[pos]; }
   T& operator[](int pos)
      { return at(pos); }
   const T& operator[](int pos) const
      { return at(pos); }
   //
   T& head()
      { return at(0); }
   const T& head() const
      { return at(0); }
   T& tail()
      { return at(m_length-1); }
   const T& tail() const
      { return at(m_length-1); }
   // standard operations (unordered vector)
   T& find(const T& item);
   const T& find(const T& item) const;
   int findindex(const T& item) const;
   // binary operations (ordered vector)
   int add(const T& item, int type = paftl::ADD_UNIQUE); // ignored if already exists
   T& search(const T& item);
   const T& search(const T& item) const;
   int searchindex(const T& item) const;
   int searchfloorindex(const T& item) const;
   int searchceilindex(const T& item) const;
   void remove(const T& item) 
   { remove_at(searchindex(item)); }
   // set operations (ordered vector)
   void operator += (const pvector<T>& v);
   // qsort algo:
   void sort();
   void sort(int lo, int hi);
   //
   friend pvector<T> intersect(const pvector<T>& a, const pvector<T>& b);
};

template <class T>
pvector<T>& pvector<T>::operator = (const pvector<T>& v)
{
   if (&v != this)
   {
      pmemvec<T>::operator = (v);
   }
   return *this;
}

template <class T>
T& pvector<T>::find(const T& item)
{
   if (findindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE);
   }
   return at(m_current);
}
template <class T>
const T& pvector<T>::find(const T& item) const
{
   if (findindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE);
   }
   return at(m_current);
}

template <class T>
int pvector<T>::findindex(const T& item) const
{
   for (int i = 0; i < m_length; i++) {
      if (at(i) == item) {
         m_current = i;
         return i;
      }
   }
   return npos;
}

// oops... we need an iterator... add use a current position marker.

template <class T>
T& pvector<T>::search(const T& item)
{
   if (searchindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE); // Not found
   }
   return at(m_current);
}
template <class T>
const T& pvector<T>::search(const T& item) const
{
   if (searchindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE); // Not found
   }
   return at(m_current);
}

template <class T>
int pvector<T>::searchindex(const T& item) const
{
   if (m_length) {
      int ifloor = -1; int itop = m_length; int ihere = 0;
      while (itop - ifloor > 1) {
         ihere = (itop + ifloor) / 2;
         if (item < at(ihere))
            itop = ihere;
         else
            ifloor = ihere;
      }
      ihere = (itop + ifloor) / 2;
      m_current = ihere;
      if (at(m_current) == item) {
         return m_current;
      }
   }
   return npos;
}

template <class T>
int pvector<T>::searchfloorindex(const T& item) const
{
   searchindex(item);
   while (m_current > -1 && at(m_current) > item) {
      m_current--;
   }
   return m_current;
}

template <class T>
int pvector<T>::searchceilindex(const T& item) const
{
   searchindex(item);
   while (m_current < m_length && at(m_current) < item) {
      m_current++;
   }
   return m_current;
}


// Note: uses m_current set by searchindex

// Really need a list 'merge' function as well... will write this soon!

template <class T>
int pvector<T>::add(const T& item, int type) // UNIQUE by default
{
   int where = -1;
   if (m_length == 0) {
      this->pmemvec<T>::push_back( item );
      where = 0;
   }
   else {
      searchindex(item);
      if (item < at(m_current)) {
         this->pmemvec<T>::insert_at( m_current, item );
         where = m_current;
      }
      else if (item > at(m_current) || type == paftl::ADD_DUPLICATE) {
         this->pmemvec<T>::insert_at( m_current + 1, item );
         where = m_current + 1;
      }
      else if (type == paftl::ADD_REPLACE) {
         // relies on good assignment operator
         at(m_current) = item;
      }
      // n.b., type "UNIQUE" does not replace, returns -1
   }
   return where;
}

template <class T>
void pvector<T>::operator += (const pvector<T>& v)
{
   if (this != &v && m_length + v.m_length > 0) {

      while (m_length + v.m_length >= storage_size())
         m_shift++;

      T *new_data = new T [storage_size()];
      if (!new_data)
         throw pexception( pexception::MEMORY_ALLOCATION );

      int i = 0, j = 0, k = 0;
      while (i + j < m_length + v.m_length) {
         if ( i < m_length ) {
            if (j < v.m_length) {
               if (m_data[i] < v.m_data[j]) {
                  new_data[k++] = m_data[i++];
               }
               else if (the_data()[i] > v.the_data()[j]) {
                  new_data[k++] = v.m_data[j++];
               }
               else {
                  new_data[k++] = m_data[i++]; j++;                 
               }
            }
            else {
               while (i < m_length) {
                  new_data[k++] = m_data[i++];                  
               }
            }
         }
         else {
            while (j < v.m_length) {            
               new_data[k++] = v.m_data[j++];
            }
         }
      }
      if (m_data) {
         delete [] m_data;
      }
      m_length = k;
      m_data = new_data;
   }
}

template <class T>
void pvector<T>::sort()
{
   sort(0,m_length-1);
}

template <class T>
void pvector<T>::sort(int lo, int hi)
{
   int curlo = lo; int curhi = hi;
   if (curhi > curlo)
   {
      const T& val = at((curlo+curhi)/2);
      while (lo <= hi) {
         while (lo < curhi && at(lo) < val)
            lo++;
         while (hi > curlo && at(hi) > val)
             hi--;
         if (lo <= hi) 
         {
            // swap contents
            T temp = m_data[lo];
            m_data[hi] = m_data[lo];
            m_data[lo] = temp;
            //
            lo++; hi--;
         }
         if (curlo < hi)
            sort(curlo, hi);

         if(lo < curhi)
            sort(lo, curhi);
      }
   }
}

// requires two sorted lists
template <class T>
inline pvector<T> intersect(const pvector<T>& a, const pvector<T>& b)
{
   pvector<T> retvec;
   int i = 0;
   int j = 0;
   while (i < a.size() && j < b.size()) {
      if (a[i] == b[j]) {
         retvec.push_back(a[i]);
         i++; j++;
      }
      else {
         while (a[i] < b[j] && i < a.size()) {
            i++;
         }
         while (a[i] > b[j] && j < b.size()) {
            j++;
         }
      }
   }
   return retvec;
}

///////////////////////////////////////////////////////////////////////////////

// prefvec: a vector of references (useful for larger objects)
// should be able to use pqvector in most cases

template <class T> class prefvec : public pmemvec<T *>
{
public:
   prefvec(int sz = 0) : pmemvec<T *>(sz)
      {;}
   prefvec(const prefvec<T>& );
   virtual ~prefvec();
   prefvec<T>& operator = (const prefvec<T>& );
   //
   void push_back(const T& item);
   void pop_back();
   void remove_at(int pos = 0);
   void remove_at(const pvecint& list);
   void insert_at(int pos, const T& item);
   //
   void set(int count);
   void set(const T& item, int count);
   //
   void clear();
   //
   T& at(int pos)
      { return  *(m_data[pos]); }
   const T& at(int pos) const
      { return *(m_data[pos]); }
   T& operator[](int pos)
      { return at(pos); }
   const T& operator[](int pos) const
      { return at(pos); }
   //
   T& head()
      { return at(0); }
   const T& head() const
      { return at(0); }
   T& tail()
      { return at(m_length-1); }
   const T& tail() const
      { return at(m_length-1); }
   //
   // NOTE: no find (as often equivalence operator will not be defined)
   //
   // Override read and write
   istream& read( istream& stream );
   ostream& write( ostream& stream );   
};

template <class T>
prefvec<T>::prefvec(const prefvec& v) : pmemvec<T *>(v)
{
   for (int i = 0; i < m_length; i++) {
      m_data[i] = new T;
      *(m_data[i]) = v.at(i);
   }
}

template <class T>
prefvec<T>& prefvec<T>::operator = (const prefvec<T>& v)
{
   if (&v != this) {
      for (int i = 0; i < m_length; i++) {
         delete m_data[i];
      }
      this->pmemvec<T *>::operator = (v);
      for (int j = 0; j < m_length; j++) {
         m_data[j] = new T(v.at(j));
      }
   }
   return *this;
}

template <class T>
prefvec<T>::~prefvec()
{
   for (int i = 0; i < m_length; i++) {
      if (m_data[i])
         delete m_data[i];
   }
   // virtual destructor called for pmemvec
}

template <class T>
void prefvec<T>::push_back(const T& item)
{
   T *p = new T(item);
   pmemvec<T *>::push_back( p );
}

template <class T>
void prefvec<T>::pop_back()
{
   delete m_data[m_length - 1];
   pmemvec<T *>::pop_back();
}

template <class T>
void prefvec<T>::remove_at(int pos)
{
   if (m_length == 0)
      throw exception( exception::EMPTY_VECTOR );
   else if (pos == npos || pos >= m_length) 
      throw exception( exception::OUT_OF_RANGE );

   delete m_data[pos];
   pmemvec<T *>::remove_at( pos );
}

// this version for intended for bulk deletes (also retains previous ordering)
template <class T>
void prefvec<T>::remove_at(const pvecint& list)
{
   if (m_length == 0)
      throw exception( exception::EMPTY_VECTOR );

   for (int i = 0; i < list.size(); i++) {
      if (list[i] == npos || list[i] >= m_length) 
         throw exception( exception::OUT_OF_RANGE );
      delete m_data[list[i]];
   }
   pmemvec<T *>::remove_at( list );
}


template <class T>
void prefvec<T>::insert_at(int pos, const T& item)
{
   T *p = new T(item);
   pmemvec<T *>::insert_at(pos, p);
}

template <class T>
void prefvec<T>::set(int count)
{
   pmemvec<T *>::set(count);
   for (int i = 0; i < m_length; i++) {
      m_data[i] = new T;
   }
}

template <class T>
void prefvec<T>::set(const T& item, int count)
{
   pmemvec<T *>::set(count);
   for (int i = 0; i < m_length; i++) {
      m_data[i] = new T(item);
   }
}


template <class T>
void prefvec<T>::clear()
{
   for (int i = 0; i < m_length; i++) {
      delete m_data[i];
   }
   pmemvec<T *>::clear();
}

// Note: read and write only work for structures without pointers

template <class T>
istream& prefvec<T>::read( istream& stream )
{
   for (int i = 0; i < m_length; i++) {
      delete m_data[i];
   }
   int length;
   stream.read( (char *) &length, sizeof(int) );
   if (stream.fail()) {
      throw pexception(pexception::FILE_ERROR);
   }
   if (length >= storage_size()) {
      if (m_data) {
         delete [] m_data;
      }
      while (length >= storage_size())
         m_shift++;
      m_data = new T * [storage_size()];
   }
   m_length = length;
   for (int j = 0; j < m_length; j++) {
      T *p = new T;
      stream.read( (char *) p, sizeof(T) );
      if (stream.fail()) {
         throw pexception(pexception::FILE_ERROR);
      }
      m_data[j] = p;
   }
   return stream;
}

template <class T>
ostream& prefvec<T>::write( ostream& stream )
{
   stream.write( (char *) &m_length, sizeof(int) );
   for (int i = 0; i < m_length; i++) {
      stream.write( (char *) &at(i), sizeof(T) );
   }
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

// pqvector... prefvec with the binary addition routine...
// (i.e., almost the hash table...)

// (so... I think we might replace pqmap with an inherited form of this soon)
// (if MS would oblige...)

template <class T> class pqvector : public prefvec<T>
{
protected:
   mutable int m_current;
public:
   pqvector(int sz = 0) : prefvec<T>(sz) {;}
   pqvector(const pqvector<T>& v) : prefvec<T>( v ) {;}
   virtual ~pqvector() {;}
   pqvector<T>& operator = (const pqvector<T>& v)
   { this->prefvec<T>::operator = (v); return *this; }
   //
   // at, [] and so on as before
   //
   // standard operations (unordered vector)
   T& find(const T& item);
   const T& find(const T& item) const;
   int findindex(const T& item) const;
   //
   // binary operations (ordered vector)
   T& search(const T& item);
   const T& search(const T& item) const;
   int searchindex(const T& item) const;
   void remove(const T& item) 
   { remove_at(searchindex(item)); }
   int add(const T& item, int type = paftl::ADD_UNIQUE);
   T& current()
   { return at(m_current); }
   const T& current() const
   { return at(m_current); }
   // qsort algo:
   void sort();
   void sort(int lo, int hi);
};

template <class T>
T& pqvector<T>::find(const T& item)
{
   if (findindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE);
   }
   return at(m_current);
}
template <class T>
const T& pqvector<T>::find(const T& item) const
{
   if (findindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE);
   }
   return at(m_current);
}

template <class T>
int pqvector<T>::findindex(const T& item) const
{
   for (int i = 0; i < m_length; i++) {
      if (at(i) == item) {
         m_current = i;
         return i;
      }
   }
   return npos;
}

// oops... we need an iterator... add use a current position marker.

template <class T>
T& pqvector<T>::search(const T& item)
{
   if (searchindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE); // Not found
   }
   return at(m_current);
}
template <class T>
const T& pqvector<T>::search(const T& item) const
{
   if (searchindex(item) == npos) {
      throw exception(exception::OUT_OF_RANGE); // Not found
   }
   return at(m_current);
}

template <class T>
int pqvector<T>::searchindex(const T& item) const
{
   if (m_length) {
      int ifloor = -1; int itop = m_length; int ihere = 0;
      while (itop - ifloor > 1) {
         ihere = (itop + ifloor) / 2;
         if (item < at(ihere))
            itop = ihere;
         else
            ifloor = ihere;
      }
      ihere = (itop + ifloor) / 2;
      m_current = ihere;
      if (at(m_current) == item) {
         return m_current;
      }
   }
   return npos;
}

// Note: uses m_current set by searchindex

// Really need a list 'merge' function as well... will write this soon!

template <class T>
int pqvector<T>::add(const T& item, int type) // default type UNIQUE
{
   int where = -1;
   if (m_length == 0) {
      this->prefvec<T>::push_back( item );
      where = 0;
   }
   else {
      searchindex(item);
      if (item < at(m_current)) {
         this->prefvec<T>::insert_at( m_current, item );
         where = m_current;
      }
      else if (item > at(m_current) || type == paftl::ADD_DUPLICATE) {
         this->prefvec<T>::insert_at( m_current + 1, item );
         where = m_current + 1;
      }
      else if (type == paftl::ADD_REPLACE) {
         // relies on good assignment operator
         this->prefvec<T>::at(m_current) = item;
      }
      // n.b., type "UNIQUE" does not replace, returns -1
   }
   return where;
}

template <class T>
void pqvector<T>::sort()
{
   sort(0,m_length-1);
}

template <class T>
void pqvector<T>::sort(int lo, int hi)
{
   int curlo = lo; int curhi = hi;
   if (curhi > curlo)
   {
      const T& val = at((curlo+curhi)/2);
      while (lo <= hi) {
         while (lo < curhi && at(lo) < val)
            lo++;
         while (hi > curlo && at(hi) > val)
             hi--;
         if (lo <= hi) 
         {
            // swap contents (using pointer)
            T* temp = m_data[lo];
            m_data[hi] = m_data[lo];
            m_data[lo] = temp;
            //
            lo++; hi--;
         }
         if (curlo < hi)
            sort(curlo, hi);

         if(lo < curhi)
            sort(lo, curhi);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////

// psubvec is based on pvector, designed for arrays of chars or shorts, it subsumes itself 
// so can be stored as a single pointer: useful if you have a lot of empty arrays

template <class T> class psubvec
{
public:
   enum base_t { npos = -1 };
protected:
   T *m_data;
public:
   psubvec()
      { m_data = NULL; }
   psubvec(const psubvec<T>& );
   ~psubvec();
   psubvec<T>& operator = (const psubvec<T>& );
   //
   virtual void push_back(const T item);
   virtual void clear();
public:
   bool isEmpty() // isEmpty is provided in addition to size as is quicker to test
      { return m_data == NULL; }
   T size() const
      { return m_data ? m_data[0] : 0; }
   T& operator [] (T pos) 
      { return m_data[pos+1]; }
   const T& operator [] (T pos) const
      { return m_data[pos+1]; }
public:
   istream& read( istream& stream, streampos offset = -1 );
   ostream& write( ostream& stream );
};

template <class T>
psubvec<T>::psubvec(const psubvec& v)
{
   if (v.m_data) {
      T length = v.m_data[0];
      T count = 0;
      while (length >>= 1) // find bit length (note: cannot assume int)
         count++;
      m_data = new T [1 << count];
      length = v.m_data[0];
      for (T i = 0; i < length + 1; i++) {
         m_data[i] = v.m_data[i];
      }
   }
   else {
      m_data = NULL;
   }
}

template <class T>
psubvec<T>::~psubvec()
{
   if (m_data)
   {
      delete [] m_data;
      m_data = NULL;
   }
}

template <class T>
psubvec<T>& psubvec<T>::operator = (const psubvec<T>& v)
{
   if (this != &v) {
      if (v.m_data) {
         T length = v.m_data[0];
         T count = 0;
         while (length >>= 1) // find bit length (note: cannot assume int)
            count++;
         m_data = new T [1 << count];
         length = v.m_data[0];
         for (T i = 0; i < length + 1; i++) {
            m_data[i] = v.m_data[i];
         }
      }
      else {
         m_data = NULL;
      }
   }
   return *this;
}

template <class T>
void psubvec<T>::push_back(const T item)
{
   if (!m_data) {
      m_data = new T [2];
      m_data[0] = 1;
      m_data[1] = item;
   }
   else {
      int length = m_data[0] + 1;
      if ((length & (length - 1)) == 0) { // determine if next length would be power of 2
         T *new_data = new T [length << 1];
         for (T i = 0; i < length; i++)
            new_data[i] = m_data[i];
         delete [] m_data;
         m_data = new_data;
      }
      m_data[0] = length;
      m_data[length] = item;
   }
}

template <class T>
void psubvec<T>::clear()
{
   if (m_data)
   {
      delete [] m_data;
      m_data = NULL;
   }
}

template <class T>
istream& psubvec<T>::read( istream& stream, streampos offset )
{
   if (m_data) {
      delete [] m_data;
      m_data = NULL;
   }
   T length;
   stream.read( (char *) &length, sizeof(T) );
   if (length) {
      T copy = length;
      while (length >>= 1) // find bit length (note: cannot assume int)
        count++;
      m_data = new T [1 << count];
      stream.read((char *) &m_data, sizeof(T*(copy+1)) );
   }
}

template <class T>
ostream& psubvec<T>::write( ostream& stream )
{
   if (m_data) {
      stream.write((char *) &m_data, sizeof(T*(m_data[0]+1)));
   }
}



///////////////////////////////////////////////////////////////////////////////////////////////

// And now the quick mapping routine

// Helper class keyvaluepair...

template <class T1, class T2> class keyvaluepair {
protected:
   T1 m_key;
   T2 m_value;
public:
   keyvaluepair(const T1 key = T1(), const T2 value = T2())
   { m_key = key; m_value = value; }
   T1& key() { return m_key; }
   const T1 key() const { return m_key; }
   T2& value() { return m_value; }
   const T2& value() const { return m_value; }
   friend bool operator == (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b);
   friend bool operator <  (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b);
   friend bool operator >  (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b);
   istream& read( istream& stream );
   ostream& write( ostream& stream );
};
template <class T1, class T2>
inline bool operator == (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b)
{ return (a.m_key == b.m_key); }
template <class T1, class T2>
inline bool operator < (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b)
{ return (a.m_key < b.m_key); }
template <class T1, class T2>
inline bool operator > (const keyvaluepair<T1,T2>& a, const keyvaluepair<T1,T2>& b)
{ return (a.m_key > b.m_key); }
// Note: read and write only work for structures without pointers
template <class T1, class T2>
istream& keyvaluepair<T1,T2>::read( istream& stream )
{
   stream.read( (char *) &m_key, sizeof(T1) );
   stream.read( (char *) &m_value, sizeof(T2) );
   return stream;
}
template <class T1, class T2>
ostream& keyvaluepair<T1,T2>::write( ostream& stream )
{
   stream.write( (char *) &m_key, sizeof(T1) );
   stream.write( (char *) &m_value, sizeof(T2) );
   return stream;
}

template <class T1, class T2> class keyvaluepairref
{
protected:
   T1 m_key;
   T2 *m_value;
public:
   keyvaluepairref(const T1 key = T1())
   { m_key = key; m_value = NULL; }
   keyvaluepairref(const T1 key, const T2& value) 
   { m_key = key; m_value = new T2(value); }
   keyvaluepairref(const keyvaluepairref& k)
   { m_key = k.m_key; m_value = new T2(*(k.m_value)); }
   keyvaluepairref& operator = (const keyvaluepairref& k) 
   { 
      if (this != &k) 
      {
         m_key = k.m_key; 
         m_value = new T2(*(k.m_value));
      }
      return *this; 
   }
   ~keyvaluepairref()
   { if (m_value) {delete m_value; m_value = NULL;} } 
   T1& key() { return m_key; }
   const T1 key() const { return m_key; }
   T2& value() { return *m_value; }
   const T2& value() const { return *m_value; }
   friend bool operator == (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b);
   friend bool operator <  (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b);
   friend bool operator >  (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b);
   //
   virtual istream& read( istream& stream );
   virtual ostream& write( ostream& stream );
};
template <class T1, class T2>
inline bool operator == (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b)
{ return (a.m_key == b.m_key); }
template <class T1, class T2>
inline bool operator < (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b)
{ return (a.m_key < b.m_key); }
template <class T1, class T2>
inline bool operator > (const keyvaluepairref<T1,T2>& a, const keyvaluepairref<T1,T2>& b)
{ return (a.m_key > b.m_key); }
// Note: read and write only work for structures without pointers
template <class T1, class T2>
istream& keyvaluepairref<T1,T2>::read( istream& stream )
{
   stream.read( (char *) &m_key, sizeof(T1) );
   m_value = new T2;
   stream.read( (char *) m_value, sizeof(T2) );
   return stream;
}
template <class T1, class T2>
ostream& keyvaluepairref<T1,T2>::write( ostream& stream )
{
   stream.write( (char *) &m_key, sizeof(T1) );
   stream.write( (char *) m_value, sizeof(T2) );
   return stream;
}

// ...and yuk! it gets worse... now we have to define a whole new class:

template <class T1,class T2,class Pair> class pmemmap
{
// It looks very similar to a pqvector...
protected:
   pqvector<Pair> m_vector;
public:
   pmemmap() {;}
   pmemmap(const pmemmap<T1,T2,Pair>& map ) { m_vector = map.m_vector; }
   ~pmemmap() {;}
   pmemmap<T1,T2,Pair>& operator = (const pmemmap<T1,T2,Pair>& map ) 
   { if (this != &map) m_vector = map.m_vector; return *this; }
   //
   // at, [] and so on
   T2& at(int i)
   { return m_vector.at(i).value(); }
   const T2& at(int i) const
   { return m_vector.at(i).value(); }
   T2& operator [] (int i)
   { return m_vector.at(i).value(); }
   const T2& operator [] (int i) const
   { return m_vector.at(i).value(); }
   T2& head()
   { return m_vector.head().value(); }
   const T2& head() const
   { return m_vector.head().value(); }
   T2& tail()
   { return m_vector.tail().value(); }
   const T2& tail() const
   { return m_vector.tail().value(); }
   int size() const
   { return m_vector.size(); }
   void clear()
   { m_vector.clear(); }
   //
   // standard operations (unordered vector)
   T2& find(const T1& item) const
   { return m_vector.find(Pair(item)).value(); }
   int findindex(const T1& item) const
   { return m_vector.findindex(Pair(item)); }
   //
   // binary operations (ordered vector)
   T2& search(const T1& item)
   { return m_vector.search(Pair(item)).value(); }
   const T2& search(const T1& item) const
   { return m_vector.search(Pair(item)).value(); }
   const int searchindex(const T1& item) const
   { return m_vector.searchindex(Pair(item)); }
   void remove_at(int i)
   { m_vector.remove_at(i); }
   void remove_at(const pvecint& list)
   { m_vector.remove_at(list); }
   void remove(const T1& item)
   { remove_at(m_vector.searchindex(item)); }
   int add(const T1& k, const T2& v, int type = paftl::ADD_UNIQUE) // note: does not replace!
   { return m_vector.add( Pair(k,v), type ); }
   // extras
   T1& key(int i)
   { return m_vector.at(i).key(); }
   const T1 key(int i) const
   { return m_vector.at(i).key(); }
   T2& value(int i)
   { return m_vector.at(i).value(); }
   const T2& value(int i) const
   { return m_vector.at(i).value(); }
   T2& current() 
   { return m_vector.current().value(); }
   const T2& current() const
   { return m_vector.current().value(); }
   // read and write (structures without pointers *only*)
   istream& read( istream& stream );
   ostream& write( ostream& stream );
};

// Note: read and write only work for structures without pointers

template <class T1,class T2,class Pair>
istream& pmemmap<T1,T2,Pair>::read( istream& stream )
{
   for (int i = m_vector.size() - 1; i >= 0; i--) {
      m_vector.remove_at(i);
   }
   int length;
   stream.read( (char *) &length, sizeof(int) );
   for (int j = 0; j < length; j++) {
      // these should be in order, so just push them:
      Pair p;
      p.read(stream);
      m_vector.push_back(p);
   }
   return stream;
}

template <class T1,class T2,class Pair>
ostream& pmemmap<T1,T2,Pair>::write( ostream& stream )
{
   int length = m_vector.size();
   stream.write( (char *) &length, sizeof(int) );
   for (int i = 0; i < m_vector.size(); i++) {
      m_vector[i].write(stream);
   }
   return stream;
}

// ...to stop the MS compiler complaining...
#define kvp(T1,T2) keyvaluepair<T1,T2>

template <class T1, class T2> class pmap : public pmemmap<T1,T2,kvp(T1,T2)>
{
};

// ...to stop the MS compiler complaining...
#define kvpr(T1,T2) keyvaluepairref<T1,T2>

template <class T1, class T2> class pqmap : public pmemmap<T1,T2,kvpr(T1,T2)>
{
};

///////////////////////////////////////////////////////////////////////////////

// and a simple list class...
// (with integrated iterator)

template <class T> class plist
{
protected:
   class pitem
   {
   protected:
      T *m_data;
   public:
      pitem *m_previous;
      pitem *m_next;
      pitem(const T *d = NULL, pitem *p = NULL, pitem *n = NULL)
      {
         if (d)
            m_data = new T(*d);
         else 
            m_data = NULL;
         m_previous = p;
         m_next = n;
      }
      pitem(const pitem& p) 
      {
         throw 1;
      }
      pitem& operator = (const pitem& p)
      {
         throw 1;
      }
      ~pitem()
      {
         if (m_data)
            delete m_data;
         m_data = NULL;
         m_previous = NULL;
         m_next = NULL;
      }
      void predel()
      {
         if (m_previous != NULL) {
            pitem *temp = m_previous->m_previous;
            temp->m_next = this;      // should work without reallocation
            delete m_previous;
            m_previous = temp;        // should work without reallocation
         }
      }
      void postdel()
      {
         if (m_next != NULL) {
            pitem *temp = m_next->m_next;
            temp->m_previous = this;  // should work without reallocation
            delete m_next;
            m_next = temp;            // should work without reallocation
         }
      }
      pitem *preins(const T& v)
      {
         pitem *temp = m_previous;
         m_previous = new pitem(&v, temp, this);
         temp->m_next = m_previous;  // should work without reallocation
         return m_previous;
      }
      pitem *postins(const T& v)
      {
         pitem *temp = m_next;
         m_next = new pitem(&v, this, temp);
         temp->m_previous = m_next;  // should work without reallocation
         return m_next;
      }
      T& data() const
      {
         if (!m_data) {
            throw 1;
         }
         return *m_data;
      }
   };
protected:
   pitem m_head;
   pitem m_tail;
   mutable pitem *m_current;
public:
   bool empty() const
   {
      return (m_head.m_next == &m_tail); // or v.v.
   }
   void pop_back()
   {
      m_tail.predel();  // i.e., delete the one before the tail
   }
   void pop_front()
   {
      m_head.postdel();   // i.e., delete the one after the head
   }
   void push_back(const T& v) 
   {
      m_tail.preins(v); // i.e., add before the tail
   }
   void push_front(const T& v)
   {
      m_head.postins(v); // i.e., add after the head
   }
   void first() const
   {
      m_current = m_head.m_next;
   }
   void last() const
   {
      m_current = m_tail.m_previous;
   }
   bool is_head() const
   {
      return (m_current == &m_head);
   }
   bool is_tail() const
   {
      return (m_current == &m_tail);
   }
   const plist<T>& operator ++(int) const
   {
      m_current = m_current->m_next;
      return *this;
   }
   const plist<T>& operator --(int) const
   {
      m_current = m_current->m_previous;
      return *this;
   }
   T& operator *() const {
      return m_current->data();
   };
   void preins(const T& data) {
      m_current->preins(data);
   }
   void postins(const T& data) {
      m_current->postins(data);
   }
   // note pre and post del delete the current item:
   void predel() {
      m_current = m_current->m_previous;
      m_current->postdel();
   }
   void postdel() {
      m_current = m_current->m_next;
      m_current->predel();
   }
   void clear()
   {
      while (!empty()) {
         pop_back();
      }
   }
public:
   plist()
   {
      m_head.m_next = &m_tail;
      m_tail.m_previous = &m_head;
      m_current = &m_head; // just so it's somewhere...
   }
   plist(const plist& list) {
      m_head.m_next = &m_tail;
      m_tail.m_previous = &m_head;
      m_current = list.m_current;
      for (list.first(); !list.is_tail(); list++) {
         push_back(*list);
      }
      list.m_current = m_current; // set back original list current to what it was before
   }
   plist& operator = (const plist& list) {
      if (&list != this) {
         m_current = list.m_current;
         clear();
         for (list.first(); list.is_tail(); list++) {
            push_back(*list);
         }
         list.m_current = m_current; // set back original list current to what it was before
      }
      return *this;
   }
   ~plist()
   {
      // remove contents:
      clear();
   }
};

///////////////////////////////////////////////////////////////////////////////

// ptree: a simple tree class

// Allows template of a template (recursive) definition for tree
#define ptreeT ptree<T>

template <class T> class ptree
{
public:
   class exception : public pexception
   {
   public:
      enum exception_t { PTREE_UNDEFINED = 0x1000,
                         UNASSIGNED_DATA = 0x1001 };
   public:
      exception(int n_exception = PTREE_UNDEFINED) : pexception( n_exception ) {}
   };
   enum base_t { npos = -1 };
protected:
   T *m_data;
   ptree<T> *m_parent;
   pvector<ptreeT *> m_children;
public:
   ptree() {
      m_data = NULL;
      m_parent = NULL;
   }
   ptree(const T& data) {
      m_data = new T(data);
      m_parent = NULL;
   }
   ptree(const ptree<T>& tree ) {
      m_data = tree.m_data;
      for (int i = 0; i < tree.m_children.size(); i++) {
         ptree<T> *child = new ptree<T>( *(tree.m_children[i]) );
         m_children.push_back( child );
         m_children.tail()->m_parent = this;
      }
   }
   ptree<T>& operator = (const ptree<T>& tree) {
      if (this != &tree) {
         m_data = tree.m_data;
         for (int i = 0; i < tree.m_children.size(); i++) {
            ptree<T> *child = new ptree<T>( *(tree.m_children[i]) );
            m_children.push_back( child );
            m_children.tail()->m_parent = this;
         }      
      }
      return *this;
   }
   ~ptree() {
      delete m_data;
      m_data = NULL;
      while (m_children.size()) {
         delete m_children.tail();
         m_children.pop_back();
      }
   }
   void addChild(const T& data) {
      m_children.push_back( new ptree<T>(data) );
      m_children.tail()->m_parent = this;
   }
   void removeChild(int ref) {
      ptree<T> *child = m_children[ref];
      m_children.remove(ref);
      delete child;
   }
   int getChildCount() const {
      return m_children.size();
   }
   ptree<T>& getChild(int ref) const {
      return *(m_children[ref]);
   }
   ptree<T>& getLastChild() const {
      return *(m_children[m_children.size() - 1]);
   }
   int hasParent() const {
      return m_parent ? 1 : 0;
   }
   ptree<T>& getParent() const {
      return *m_parent;
   }
   void setParent(ptree<T>& parent) {
      if (m_parent) {
         for (int i = 0; i < m_parent->m_children.size(); i++) {
            if (m_parent->m_children[i] == this) {
               m_parent->m_children.remove(i);
            }
         }
      }
      m_parent = &parent;
   }
   void setValue(const T& data) {
      m_data = new T(data);
   }
   T& getValue() const {
      if (!m_data)
         throw exception( exception::UNASSIGNED_DATA );
      return *m_data;
   }
   pvector<T> getPath() {
      pvector<T> path;
      getPath(path);
      return path;
   }
   void getPath(pvector<T>& path) {
      path.push_back( getValue() );
      if (m_parent) {
         m_parent->getPath(path);
      }
   }
};

///////////////////////////////////////////////////////////////////////////////

template <class T> class pflipper
{
protected:
   T *m_a;
   T *m_b;
   short parity;
public:
   pflipper() {
      parity = 0;
      m_a = new T;
      m_b = new T;
   }
   pflipper( const T& a, const T& b ) {
      parity = 0; 
      m_a = new T(a);
      m_b = new T(b);
   }
   pflipper( const pflipper& f ) {
      parity = f.parity;
      m_a = new T(*(f.m_a));
      m_b = new T(*(f.m_b));
   }
   ~pflipper() {
      if (m_a) {
         delete m_a; 
         m_a = NULL;
      }
      if (m_b) {
         delete m_b;
         m_b = NULL;
      }
   }   
   pflipper& operator = (const pflipper& f ) {
      if (this != &f) {
         if (m_a)
            delete m_a; 
         if (m_b)
            delete m_b;
         parity = f.parity;
         m_a = new T(*(f.m_a));
         m_b = new T(*(f.m_b));
      }
      return *this;
   }
   void flip() {
      parity = ~parity;
   }
   T& a() const {
      if (parity) 
         return *m_a;
      else 
         return *m_b;
   }
   T& b() const {
      if (parity)
         return *m_b;
      else
         return *m_a;
   }
};

///////////////////////////////////////////////////////////////////////////////

// pstring: a simple string class

class pstring
{
public:
   class exception : public pexception
   {
   public:
      enum exception_t { PSTRING_UNDEFINED = 0x1000,
                         BAD_STRING        = 0x1001, 
                         OUT_OF_RANGE      = 0x1002,
                         UNABLE_TO_CONVERT = 0x1003 };
   public:
      exception(int n_exception = PSTRING_UNDEFINED) : pexception( n_exception ) {}
   };

   enum base_t { npos = -1 };

protected:
   int   m_alloc;
   int   m_start;
   int   m_end;
   char *m_data;

public:
   pstring(const char *data = NULL)
   { 
      if (!data || (m_end = strlen(data)) < 1) {
         m_data = NULL;
         m_start = 0;
         m_end = 0;
         m_alloc = 0;
      }
      else {
         m_start = 0;
         m_alloc = m_end + 1;
         m_data = new char [m_alloc];
         if (!m_data)
            throw pexception( pexception::MEMORY_ALLOCATION );
         for (int i = m_start; i < m_end; i++) m_data[i] = data[i];
         m_data[m_end] = '\0';
      }
   }
   pstring(const char c)
   {
      m_start = 0;
      m_end = 1;
      m_alloc = m_end + 1;
      m_data = new char [m_alloc];
      m_data[0] = c;
      m_data[m_end] = '\0';
   }
   pstring(const int length)
   {
      if (length < 1) {
         m_data = NULL;
         m_start = 0;
         m_end = 0;
         m_alloc = 0;
      }
     else {
         m_start = length;
         m_end   = length;
         m_alloc = length + 1;
         m_data = new char [m_alloc];
         if (!m_data) {
            throw pexception( pexception::MEMORY_ALLOCATION );
         }
         m_data[m_end] = '\0';
      }
   }
   // Added V1.6 to allow initialisation from long / double
   // (due to the phasing out of the pdata class)
   friend pstring pstringify(const char c);
   friend pstring pstringify(const int val, const pstring& format);
   friend pstring pstringify(const double& val, const pstring& format);
   //
   pstring(const pstring& s)
   {
      if (s.m_alloc == 0) {
         m_data = NULL;
         m_start = 0;
         m_end = 0;
         m_alloc = 0;
      }
      else if (!s.m_data)
         throw exception( exception::BAD_STRING );
      else {
         m_start = 0;
         m_end = s.m_end - s.m_start;
         m_alloc = m_end + 1;
         m_data = new char [m_alloc];
         if (!m_data)
            throw pexception( pexception::MEMORY_ALLOCATION );
         for (int i = 0; i < m_end; i++) {
            m_data[i] = s.m_data[i + s.m_start];
         }
         m_data[m_end] = '\0';
      }
   }
   virtual ~pstring()
   { 
      if (m_data) { 
         delete [] m_data; m_data = NULL; m_start = 0; m_end = 0; m_alloc = 0;
      } 
   }
   pstring& operator = (const pstring& s)
   {
      if (&s != this) {
         if (m_alloc < s.m_end - s.m_start + 1) { 
            if (m_data) {
               delete [] m_data;
            }
            if (s.m_end - s.m_start) {
               m_alloc = s.m_end - s.m_start + 1;
               m_data = new char [s.m_alloc];
               if (!m_data)
                  throw pexception( pexception::MEMORY_ALLOCATION );
            }
            else {
               m_alloc = 0;
               m_data = NULL;
            }
         }
         m_start = 0;
         m_end = s.m_end - s.m_start;
         if (s.m_end - s.m_start) {
            if (!s.m_data)
               throw exception( exception::BAD_STRING );
            for (int i = 0; i < m_end; i++)
               m_data[i] = s.m_data[i + s.m_start];
            m_data[m_end] = '\0';
         }
      }
      return *this;
   }
   const char *c_str() const
   {
      return m_data + m_start;   // Note! Not persistent: do not pass this but c_strcpy()!
   }
   char *c_strcpy(char *str) const
   {
      return strcpy( str, (m_data + m_start) );
   }
   // Three simple conversions
   int c_int() const;
   double c_double() const;

   char& operator [] (int i) const
   {
      if (i >= (m_end - m_start) || i < 0) 
         throw exception( exception::OUT_OF_RANGE );
      return m_data[i + m_start];
   }
   void clear() 
      { m_start = 0; m_end = 0; }   // Note: no deallocation
   bool empty() const
      { return (m_end == m_start) ? true : false; }
   int size() const
      { return m_alloc; }
   int length() const
      { return m_end - m_start; }

   pstring substr( int start = 0, int length = -1 ) const;

   int findindex( char c, int start = 0, int end = npos ) const;
   int findindexreverse( char c, int start = npos, int end = 0 ) const;
   int findindex( const pstring& str, int start = 0, int end = npos ) const;

   // Full tokenizer:
   pvecstring tokenize( char delim = ' ', bool ignorenulls = false );

   pstring splice( char delim = ' ' );
   pstring splice( const pstring& delim );

   pstring& ltrim( char c = ' ');
   pstring& ltrim( const pstring& str );

   pstring& replace( char a );   // simply remove all instances of char a
   pstring& replace( char a, char b );

   pstring& makelower();
   pstring& makeupper();

   friend bool compare(const pstring& a, const pstring& b, int n);
   friend bool operator == (const pstring& a, const pstring& b);
   friend bool operator != (const pstring& a, const pstring& b);
   friend bool operator <  (const pstring& a, const pstring& b); // for storing in hash tables etc
   friend bool operator >  (const pstring& a, const pstring& b); 

   friend pstring operator + (const pstring& a, const pstring& b);

   friend ostream& operator << (ostream& stream, const pstring& str);
   friend istream& operator >> (istream& stream, pstring& str);

   friend pstring readtoken(istream& stream, 
                            const pstring& delim_list = pstring(" "));

   char *raw() const
      { return m_data; }

   // binary read/write
   bool read(istream& stream);
   bool write(ostream& stream);
};

inline pstring pstringify(const char val)
{
   pstring str(1);
   str[0] = val;
   return str;
}

inline pstring pstringify(const int val, const pstring& format = "% 16d")
{
   pstring str;
   str.m_start = 0;
   str.m_alloc = 16 + format.length();
   str.m_data = new char [str.m_alloc];
   if (!str.m_data) {
      throw pexception( pexception::MEMORY_ALLOCATION );
   }
   sprintf( str.m_data, format.c_str(), val ); 
   str.m_end = strlen(str.m_data);
   return str;
}

inline pstring pstringify(const double& val, const pstring& format = "%+.16le")
{
   pstring str;
   str.m_start = 0;
   str.m_alloc = 24 + format.length();
   str.m_data = new char [str.m_alloc];
   if (!str.m_data) {
      throw pexception( pexception::MEMORY_ALLOCATION );
   }
   sprintf( str.m_data, format.c_str(), val );
   str.m_end = strlen(str.m_data);
   return str;
}

inline pstring pstring::substr(int start, int length) const
{
   if (length < 0 || length > (m_end - m_start - start))
      length = m_end - m_start - start;
   pstring sub;
   sub.m_start = 0;
   sub.m_end = length;
   sub.m_alloc = length + 1;
   sub.m_data = new char [sub.m_alloc];
   for (int i = 0; i < sub.m_end; i++)
      sub.m_data[i] = m_data[i + m_start + start];
   sub.m_data[length] = '\0';
   return sub;
}

inline int pstring::findindex( char c, int start, int end ) const
{
   if (end == pstring::npos || end > m_end - m_start) {
      end = m_end - m_start;
   }
   for (int pos = start; pos < end; pos++) {
      if (m_data[pos + m_start] == c) {
         return pos;
      }
   }
   return pstring::npos;
}

inline int pstring::findindexreverse( char c, int start, int end ) const
{
   if (start == pstring::npos || start > m_end - m_start) {
      start = m_end - m_start;
   }
   for (int pos = start - 1; pos >= end; pos--) {
      if (m_data[pos + m_start] == c) {
         return pos;
      }
   }
   return pstring::npos;
}

inline int pstring::findindex( const pstring& str, int start, int end ) const
{
   if (end == pstring::npos) { 
      end = m_end - m_start;
   }
   for (int pos = start; pos < end; pos++) {
      if (m_data[pos + m_start] == str.m_data[str.m_start]) {
         int mark = pos, remark = pstring::npos;
         while (mark != pstring::npos && ++pos < str.m_end - str.m_start + mark) {
            if (pos >= end) {
               return pstring::npos;
            }
            else if (m_data[pos+m_start] != str.m_data[pos-mark+str.m_start]) {
               mark = pstring::npos;
            }
            else if (m_data[pos+str.m_start] == str.m_data[str.m_start] 
                     && remark == pstring::npos) {
               remark = pos;
            }
         }
         if (mark != pstring::npos) {
            return mark;
         }
         else if (remark != pstring::npos) {
            return findindex( str, remark );
         }
      }
   }
   return pstring::npos;
}

inline pstring& pstring::ltrim( char c )
{
   int pos = m_start;
   while (pos < m_end && m_data[pos] == c)
      pos++;
   m_start = pos;
   return *this;
}

inline pstring& pstring::ltrim( const pstring& str )
{
   int pos = m_start;
   while (pos < m_end && str.findindex(m_data[pos]) != npos )
      pos++;
   m_start = pos;
   return *this;
}

// tokenize: going beyond splice...

inline pvecstring pstring::tokenize( char delim, bool ignorenulls )
{
   pvecstring tokens;
   int first, last = 0;
   while (last + m_start < m_end) {
      first = last;
      last = findindex(delim, first);
      if (last == npos)
         last = m_end - m_start;
      tokens.push_back( substr(first, last - first) );
      if (ignorenulls) {
         while (last + m_start < m_end && m_data[last+m_start] == delim) 
            last++;     // Lose consecutive delimeters
      }
      else {
         last++;        // Lose delimiter
      }
   }
   return tokens;
}

// splice: kind of combined substr, find operation: 

// returns the head, the tail is kept, the delimiter is destroyed
// (the leading whitespace destroy is no longer used: use x.ltrim().splice() )

inline pstring pstring::splice( char delim )
{
   int pos = 0;
   pos = findindex(delim, pos);
   if (pos == npos)
      pos = m_end - m_start;
   pstring str = substr(0, pos);
   pos++; // Lose delimiter
   m_start = m_end > pos + m_start ? pos + m_start : m_end;
   return str;
}

// returns the head, the tail is kept, the delimiter is destroyed

inline pstring pstring::splice( const pstring& delim )
{
   int pos = findindex(delim);
   if (pos == npos)
      pos = m_end - m_start;
   pstring str = substr(0, pos);
   pos += delim.m_end; // Lose delimiter
   m_start = m_end > pos + m_start ? pos + m_start : m_end;
   return str;
}

// Conversion

inline int pstring::c_int() const 
{
   int out = 0;
   if (m_start != m_end) {
      char *endptr = m_data + m_start;
      out = (int) strtol( m_data + m_start, &endptr, 10 );
      if (endptr == m_data + m_start) {
         throw exception( exception::UNABLE_TO_CONVERT );
      }
   }
   return out;
}

inline double pstring::c_double() const 
{
   double out = 0.0;
   if (m_start != m_end) {
      char *endptr = m_data + m_start;
      out = strtod( m_data + m_start, &endptr );
      if (endptr == m_data + m_start) {
         throw exception( exception::UNABLE_TO_CONVERT );
      }
   }
   return out;
}

// Comparison

inline bool compare(const pstring& a, const pstring& b, int n)
{
   if (n > a.length() || n > b.length()) {
      false;
      //throw pstring::exception( pstring::exception::OUT_OF_RANGE );
   }
   for (int i = 0; i < n; i++) {
      if ( a.m_data[i+a.m_start] != b.m_data[i+b.m_start] ) {
         return false;
      }
   }
   return true;
}

inline bool operator != (const pstring& a, const pstring& b)
{
   if (a.length() != b.length()) {
      return true;
   }
   for (int i = 0; i < a.length(); i++) 
   {
      if (a.m_data[i+a.m_start] != b.m_data[i+b.m_start]) {
         return true;
      }
   }
   return false;
}

inline bool operator == (const pstring& a, const pstring& b)
{
   return !(a != b);
}

inline bool operator < (const pstring& a, const pstring& b) // for storing in hash tables etc
{
   for (int i = 0; i < b.length(); i++) {
      if (i == a.length()) {
         return true;   // <- if all characters are the same, but b is longer than a, then a < b
      }
      else if (a.m_data[i+a.m_start] < b.m_data[i+b.m_start]) {
         return true;
      }
      else if (a.m_data[i+a.m_start] > b.m_data[i+b.m_start]) {
         return false;
      }
   }
   return false;
}

inline bool operator > (const pstring& a, const pstring& b) // for storing in hash tables etc 
{
   for (int i = 0; i < a.length(); i++) {
      if (i == b.length()) {
         return true;   // <- if all characters are the same, but a is longer than b, then a > b
      }
      else if (a.m_data[i+a.m_start] > b.m_data[i+b.m_start]) {
         return true;
      }
      else if (a.m_data[i+a.m_start] < b.m_data[i+b.m_start]) {
         return false;
      }
   }
   return false;
}

inline pstring operator + (const pstring& a, const pstring& b)
{
   pstring str(a.length() + b.length());
   str.m_start = 0;
   for (int i = 0; i < str.m_end; i++)
   {
      str.m_data[i] = (i < a.length()) ? a.m_data[i+a.m_start] : 
                                         b.m_data[i+b.m_start - a.length()];
   }
   return str;
}

inline ostream& operator << (ostream& stream, const pstring& str)
{
   if (str.empty()) {
      return stream;
   }
   return ( stream << str.c_str() );
}

inline istream& operator >> (istream& stream, pstring& str)
{
   // default is 128, but grows upto required length
   if (str.m_alloc < 128) {
      str.m_alloc = 128;
      delete [] str.m_data;
      str.m_data = new char [str.m_alloc];
      if (!str.m_data)
         throw pexception( pexception::MEMORY_ALLOCATION );     
   }
   for (int i = 0; ; i++) {
      if (i + 1 == str.m_alloc) {
         char *old_data = str.m_data; 
         str.m_data = new char[str.m_alloc * 2];
         for (int j = 0; j < str.m_alloc; j++)
            str.m_data[j] = old_data[j];
         delete [] old_data;
         str.m_alloc *= 2;
      }
      stream.get( str.m_data[i] ); 
      // cross platform --- just do this yourself: \n is for MAC = 13, UNIX = 10, MS = 13,10
      if (str.m_data[i] == 10 || stream.eof() || stream.fail()) {
         str.m_data[i] = '\0';
         break;
      }
      else if (str.m_data[i] == 13) {
         if (stream.peek() == 10) 
            stream.get( str.m_data[i] );             
         str.m_data[i] = '\0';
         break;
      }
   }
   str.m_start = 0;
   str.m_end = strlen(str.m_data);
   return stream;
}

// binary read / write

inline bool pstring::read(istream& stream)
{
   // actually, required alloc is 1 less than required alloc due to null terminator
   int required_alloc;
   stream.read( (char *) &required_alloc, sizeof(int) );
   if (required_alloc > 0 && m_alloc < required_alloc + 1) {
      m_alloc = required_alloc + 1;
      delete [] m_data;
      m_data = new char [m_alloc];
      if (!m_data)
         throw pexception( pexception::MEMORY_ALLOCATION );     
   }
   if (required_alloc > 0) {
      stream.read(m_data, required_alloc);
      m_data[required_alloc] = '\0';
   }
   m_start = 0;
   m_end = required_alloc;

   return true;
}

inline bool pstring::write(ostream& stream)
{
   int required_alloc = 0;
   if (m_data)
      required_alloc = m_end - m_start;
   stream.write( (char *) &required_alloc, sizeof(int) );
   if (required_alloc > 0)
      stream.write( m_data + m_start, required_alloc );
   return true;
}

// Okay, I give up, here you are, a token reader

inline pstring readtoken(istream& stream, const pstring& delim_list) 
{
   pstring str;
   str.m_start = 0;
   str.m_end = 0;
   str.m_alloc = 128;
   str.m_data = new char [str.m_alloc];

   char single_char;
   int here = -1;

   while ( !stream.eof() && str.m_end < 128 ) {
      single_char = stream.get();
      if (here == -1) {
         if (isprint(single_char) && delim_list.findindex(single_char) == pstring::npos) {
            here = 0;
            str.m_data[ 0 ] = single_char;
            str.m_end++;
         }
      }
      else {
         here++;
         if (!isprint(single_char) || delim_list.findindex(single_char) != pstring::npos) {
            str.m_data[ str.m_end ] = '\0';
            return str;
         }
         else {
            str.m_data[ str.m_end ] = single_char;
            str.m_end++;
         }
      }
   }
   str.m_data[ str.m_end ] = '\0';
   return str;   
}

inline pstring& pstring::replace(char a)
{
   int i = m_start;
   for (int j = m_start; j < m_end; i++, j++) {
      if (a == m_data[j]) {
         j++;
         if (j == m_end) {
            break;
         }
      }
      m_data[i] = m_data[j];
   }
   m_end = i;
   m_data[ i ] = '\0';
   return *this;
}

inline pstring& pstring::replace(char a, char b)
{
   for (int i = m_start; i < m_end; i++) {
      if (a == m_data[i]) {
         m_data[i] = b;
      }
   }
   return *this;
}

inline pstring& pstring::makelower()
{
   for (int i = m_start; i < m_end; i++) {
      // ASCII caps
      if (m_data[i] > 64 && m_data[i] < 91) {
         m_data[i] += 32;
      }
   }
   return *this;
}
inline pstring& pstring::makeupper()
{
   for (int i = m_start; i < m_end; i++) {
      // ASCII lower
      if (m_data[i] > 96 && m_data[i] < 123) {
         m_data[i] -= 32;
      }
   }
   return *this;
}


///////////////////////////////////////////////////////////////////////////////

// Read and write runlength encoded vectors, with byte alignment through T

// note: vector must have been allocated to accept stream
template <class T>
istream& read_rle( istream& stream, T *vector, int length )
{
   unsigned char *data = (unsigned char *) vector;
   for (int i = 0; i < sizeof(T); i++) {
      unsigned char runlength = 0, current, last;
      int count = 0;
      while (count < length) {
         stream.get(current);
         if (count && current == last) {
            stream.get(runlength);
         }
         else {
            last = current;
            runlength = 1;
         }
         for (int i = count; i < count + runlength; i++) {
            data[i + sizeof(T) * count] = current;
         }
         count += (int) runlength;
      }
   }
   return stream;
}

template <class T> ostream& write_rle( ostream& stream, T *vector, int length )
{
   unsigned char *data = (unsigned char *) vector;
   for (int i = 0; i < sizeof(T); i++) {
      unsigned char runlength = 0, current;
      int count = 0;
      while (count < length) {
         do {
            current = data[i + sizeof(T) * count];
            runlength++;
            count++;
         } while (count < length && current == data[i + sizeof(T) * count] && runlength < 255);
         if (runlength == 1) {
            stream.put(current);
         }
         else {
            stream.put(current);
            stream.put(current);
            runlength -= 1; // since we've written current twice anyway to mark the run
            stream.put(runlength);
         }
         runlength = 0;
      }
   }
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

// Hashing for LZW compression

const int HASHBITSIZE = 12;
const int HASHTABLESIZE = 5021; // n.b., prime > 4096
//const int HASHTABLESIZE = 65983; // n.b., prime > 65536
const int HASHMAXVALUE = (1 << HASHBITSIZE) - 1;
const int HASHMAXCODE = HASHMAXVALUE - 1;

struct phash
{
   int code;
   unsigned int prefix;
   unsigned char character;
};

class phashtable
{
protected:
   mutable int m_current;
   int m_nextcode;
   phash m_table[HASHTABLESIZE];
public:
   phashtable();
   void add_encode(unsigned int prefix, unsigned char character);
   void add_decode(unsigned int prefix, unsigned char character);
   int search(unsigned int prefix, unsigned char character) const;
   unsigned char *decode(unsigned char *buffer, unsigned int code);
   int getnextcode()
   { return m_nextcode; }
};

inline phashtable::phashtable()
{
   for (int i = 0; i < HASHTABLESIZE; i++) {
      m_table[i].code = -1;
   }
   m_nextcode = 256; // 0-255 for standard characters
}

inline void phashtable::add_encode(unsigned int prefix, unsigned char character)
{
   if (m_nextcode <= HASHMAXCODE) {
      m_table[m_current].code = m_nextcode;
      m_table[m_current].prefix = prefix;
      m_table[m_current].character = character;
      m_nextcode++;
   }
}

inline void phashtable::add_decode(unsigned int prefix, unsigned char character)
{
   if (m_nextcode <= HASHMAXCODE) {
      m_table[m_nextcode].prefix = prefix;
      m_table[m_nextcode].character = character;
      m_nextcode++;
   }
}

inline int phashtable::search(unsigned int prefix, unsigned char character) const
{
   // the bracket overkill on the following line ensures paftl.h compiles on a GNU compiler
   m_current = (((unsigned int) character) << (HASHBITSIZE-8)) ^ prefix;
   int offset;
   if (m_current == 0) {
      offset = 1;
   }
   else {
      offset = HASHTABLESIZE - m_current;
   }
   while ( m_table[m_current].code != -1 && 
          (m_table[m_current].prefix != prefix || m_table[m_current].character != character))
   {
      m_current -= offset;
      if (m_current < 0)
         m_current += HASHTABLESIZE;
   }
   return m_table[m_current].code;
}

inline unsigned char *phashtable::decode(unsigned char *buffer, unsigned int code)
{
  int i = 0;
  while (code > 255)
  {
    *buffer++ = m_table[code].character;
    code = m_table[code].prefix;
  }
  *buffer = code;
  return buffer;
}

///////////////////////////////////////////////////////////////////////////////

// LZW as of June 2003 US patent has expired
//     as of June 2004 the European patent expires

// Note to paftl users: ensure that you meet patent requirements for your usage

// LZW uses a class so that the hash table may be retained over multiple 
// vector read / writes, as well as holding a read buffer

// 12 bit, 4096 pattern holder

// Note: you must *flush* after writing -- the idea is you can write several
// vectors before a flush

template <class T> class plzw
{
protected:
   // read / write buffering
   unsigned long m_bitbuffer;
   bool m_bitswaiting;
protected:
   bool m_firstever;
   unsigned char m_character;
   unsigned int m_prefix;
   phashtable m_hashtable;
   unsigned char m_decodedstring[HASHTABLESIZE];   // <- impossible that the decode string is ever as long as the hash table size
public:
   plzw();
   istream& read( istream& stream, T *vector, int length );
   ostream& write( ostream& stream, T *vector, int length );
protected:
   istream& get(istream& stream, unsigned int& code);
   ostream& put(ostream& stream, const unsigned int code);
};

template <class T> 
plzw<T>::plzw()
{
   m_firstever = true;
   m_bitswaiting = false;
}

template <class T> 
istream& plzw<T>::read(istream& stream, T *vector, int length )
{
   unsigned char *data = (unsigned char *) vector;
   unsigned char *string;

   if (m_firstever) {
      get(stream, m_prefix);
      m_character = m_prefix;
      data[0] = m_character;
   }

   // T is sequenced, as we assume that patterns will recur through aligned bytes of T
   for (unsigned int i = (m_firstever ? 1 : 0); i < sizeof(T) * length;) {
      unsigned int nextcode;
      get(stream, nextcode);
      if (nextcode >= (unsigned int) m_hashtable.getnextcode()) {
         *m_decodedstring = m_character;
         string = m_hashtable.decode(m_decodedstring + 1, m_prefix);
      }
      else {
         string = m_hashtable.decode(m_decodedstring, nextcode);
      }
      m_character = *string;
      if (i != 0) {
         // this should be skipped on initial read
         m_hashtable.add_decode(m_prefix,m_character);
      }
      while (string >= m_decodedstring && i < sizeof(T) * length) {
         data[i] = *string--;
         i++;
      }
      m_prefix = nextcode;
   }

   // skip to next byte boundary for next read... (and retain context)
   m_bitswaiting = false;
   m_firstever = false;

   return stream;
}

template <class T> 
ostream& plzw<T>::write(ostream& stream, T *vector, int length )
{
   unsigned char *data = (unsigned char *) vector;

   m_prefix = data[0];

   for (unsigned int i = 0; i < sizeof(T) * length; i++) {
      m_character = data[i];
      int code = m_hashtable.search(m_prefix, m_character);
      if (code != -1) {
         m_prefix = code;
      }
      else {
         m_hashtable.add_encode(m_prefix, m_character);
         put(stream, m_prefix);
         m_prefix = m_character;
      }
   }

   // skip to next byte boundary
   put(stream, m_prefix);
   if (m_bitswaiting) {
      stream.put((unsigned char)(m_bitbuffer & 0xFF));
      m_bitswaiting = false;
   }

   return stream;
}

// stored, 1 and 2 are the 12-bit codes,
// a b & c are first 4 bits, second 4 bits and third four bits respectively
// b1 a1
// c2 c1  
// b2 a2

template <class T> 
istream& plzw<T>::get(istream& stream, unsigned int& code)
{
   unsigned char bits;
   if (!m_bitswaiting) {
      stream.get(bits);
      code = bits;
      stream.get(bits);
      code |= (bits & 0x0F) << 8;
      m_bitbuffer = bits & 0xF0;
      m_bitswaiting = true;
   }
   else {
      stream.get(bits);
      code = ((unsigned int)bits) | (m_bitbuffer << 4);
      m_bitswaiting = false;
   }

   return stream;
}

template <class T> 
ostream& plzw<T>::put(ostream& stream, const unsigned int code)
{
   if (!m_bitswaiting) {
      stream.put((unsigned char)(code & 0xFF));
      m_bitbuffer = code >> 8;
      m_bitswaiting = true;
   }
   else {
      unsigned char bits;
      bits = ((code >> 4) & 0xF0) | m_bitbuffer;
      stream.put(bits);
      stream.put((unsigned char)(code & 0xFF));
      m_bitswaiting = false;
   }

   return stream;
}

///////////////////////////////////////////////////////////////////////////////

// } // namespace paftl

#endif
