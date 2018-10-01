// Copyright (c) 1996-2011 Alasdair Turner (a.turner@ucl.ac.uk)
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

#pragma once

#include "genlib/exceptions.h"
#include <istream>
#include <sstream>

// psubvec is based on pvector, designed for arrays of chars or shorts, it subsumes itself
// so can be stored as a single pointer: useful if you have a lot of empty arrays

template <class T> class psubvec
{
public:
   static const T npos = -1;
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
   std::istream& read( std::istream& stream, std::streampos offset = -1 );
   std::ostream& write( std::ostream& stream );
};

template <class T>
psubvec<T>::psubvec(const psubvec& v)
{
   if (v.m_data) {
      T length = v.m_data[0];
      T count = 0;
      while (length >>= 1) // find bit length (note: cannot assume int)
         count++;
      m_data = new T [2 << count];
      if (m_data == NULL) {
          std::stringstream message;
          message << "Memory allocation error: " << (sizeof(T) * size_t(2 << count));
          throw depthmapX::RuntimeException(message.str());
      }
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
         m_data = new T [2 << count];
         if (m_data == NULL) {
             std::stringstream message;
             message << "Memory allocation error: " << (sizeof(T) * size_t(2 << count));
             throw depthmapX::RuntimeException(message.str());
         }
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
      T length = m_data[0] + 1;
      if ((length & (length - 1)) == 0) { // determine if next length would be power of 2
         T *new_data = new T [length << 1];
         if (new_data == NULL) {
             std::stringstream message;
             message << "Memory allocation error: " << (sizeof(T) * size_t(length << 1));
            throw depthmapX::RuntimeException(message.str());
         }
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
std::istream& psubvec<T>::read( std::istream& stream, std::streampos offset )
{
   if (m_data) {
      delete [] m_data;
      m_data = NULL;
   }
   T length;
   stream.read( (char *) &length, sizeof(T) );
   if (length) {
      T copy = length;
       T count = 0;
      while (length >>= 1) // find bit length (note: cannot assume int)
        count++;
      m_data = new T [2 << count];
      if (m_data == NULL) {
          std::stringstream message;
          message << "Memory allocation error: " << (sizeof(T) * size_t(2 << count));
          throw depthmapX::RuntimeException(message.str());
      }
      stream.read((char *) &m_data, sizeof(T)*(copy+1) );
   }
   return stream;
}

template <class T>
std::ostream& psubvec<T>::write( std::ostream& stream )
{
   if (m_data) {
      stream.write((char *) &m_data, sizeof(T)*(m_data[0]+1));
   }
   return stream;
}
