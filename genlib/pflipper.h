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

template <class T> class pflipper {
  protected:
    T m_contents[2];
    short parity;

  public:
    pflipper() { parity = 0; }
    pflipper(const T &a, const T &b) {
        parity = 0;
        m_contents[0] = a;
        m_contents[1] = b;
    }
    pflipper(const pflipper &f) {
        parity = f.parity;
        m_contents[0] = f.m_contents[0];
        m_contents[1] = f.m_contents[1];
    }
    virtual ~pflipper() {}
    pflipper &operator=(const pflipper &f) {
        if (this != &f) {
            parity = f.parity;
            m_contents[0] = f.m_contents[0];
            m_contents[1] = f.m_contents[1];
        }
        return *this;
    }
    void flip() { parity = (parity == 0) ? 1 : 0; }
    T &a() { return m_contents[parity]; }
    T &b() { return m_contents[(parity == 0) ? 1 : 0]; }
    const T &a() const { return m_contents[parity]; }
    const T &b() const { return m_contents[(parity == 0) ? 1 : 0]; }
};
