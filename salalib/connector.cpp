// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2011-2012, Tasos Varoudis

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "salalib/connector.h"

#include "genlib/containerutils.h"
#include "genlib/readwritehelpers.h"
#include "genlib/legacyconverters.h"
#include "genlib/comm.h" // for communicator

#include <math.h>
#include <float.h>
#include <time.h>

bool Connector::read( std::istream& stream)
{
   m_connections.clear();
   m_forward_segconns.clear();
   m_back_segconns.clear();

   // n.b., must set displayed attribute as soon as loaded...
   dXreadwrite::readIntoVector(stream, m_connections);

   stream.read((char *)&m_segment_axialref, sizeof(m_segment_axialref));

   pmap<SegmentRef, float> forward_segconns;
   forward_segconns.read(stream);
   m_forward_segconns = genshim::toSTLMap(forward_segconns);

   pmap<SegmentRef, float> back_segconns;
   back_segconns.read(stream);
   m_back_segconns = genshim::toSTLMap(back_segconns);

   return true;
}

bool Connector::write( std::ofstream& stream )
{
   // n.b., must set displayed attribute as soon as loaded...
   dXreadwrite::writeVector(stream, m_connections);
   stream.write((char *)&m_segment_axialref, sizeof(m_segment_axialref));
   genshim::toPMap(m_forward_segconns).write(stream);
   genshim::toPMap(m_back_segconns).write(stream);

   return true;
}

/////////////////////////////////////////////////////////////////////////////////

// Cursor extras

int Connector::count(int mode) const
{
   int c = 0;
   switch (mode) {
   case CONN_ALL:
      c = m_connections.size();
      break;
   case SEG_CONN_ALL:
      c = m_back_segconns.size() + m_forward_segconns.size();
      break;
   case SEG_CONN_FW:
      c = m_forward_segconns.size();
      break;
   case SEG_CONN_BK:
      c = m_back_segconns.size();
      break;
   }
   return c;
}
int Connector::getConnectedRef(int cursor, int mode) const
{
   int cur = -1;
   if (cursor != -1) {
      switch (mode) {
      case CONN_ALL:
         if (cursor < int(m_connections.size())) {
            cur = m_connections[size_t(cursor)];
         }
         break;
      case SEG_CONN_ALL:
         if (cursor < int(m_back_segconns.size())) {
            cur = depthmapX::getMapAtIndex(m_back_segconns, cursor)->first.ref;
         }
         else if (size_t(cursor) - m_back_segconns.size() < m_forward_segconns.size()) {
            cur = depthmapX::getMapAtIndex(m_forward_segconns, cursor - int(m_back_segconns.size()))->first.ref;
         }
         break;
      case SEG_CONN_FW:
         if (cursor < int(m_forward_segconns.size())) {
            cur = depthmapX::getMapAtIndex(m_forward_segconns, cursor)->first.ref;
         }
         break;
      case SEG_CONN_BK:
         if (cursor < int(m_back_segconns.size())) {
            cur = depthmapX::getMapAtIndex(m_back_segconns, cursor)->first.ref;
         }
         break;
      }
   }
   return cur;
}
int Connector::direction(int cursor, int mode) const
{
   int direction = 0;
   if (cursor != -1) {
      switch (mode) {
      case SEG_CONN_ALL:
         if (cursor < (int)m_back_segconns.size()) {
            direction = depthmapX::getMapAtIndex(m_back_segconns, cursor)->first.dir;
         }
         else if (size_t(cursor) - m_back_segconns.size() < m_forward_segconns.size()) {
            direction = depthmapX::getMapAtIndex(m_forward_segconns, cursor - int(m_back_segconns.size()))->first.dir;
         }
         break;
      case SEG_CONN_FW:
         direction = depthmapX::getMapAtIndex(m_forward_segconns, cursor)->first.dir;
         break;
      case SEG_CONN_BK:
         direction = depthmapX::getMapAtIndex(m_back_segconns, cursor)->first.dir;
         break;
      }
   }
   return direction;
}
float Connector::weight(int cursor, int mode) const
{
   float weight = 0.0f;
   if (cursor != -1) {
      switch (mode) {
      case SEG_CONN_ALL:
         if (cursor < int(m_back_segconns.size())) {
            weight = depthmapX::getMapAtIndex(m_back_segconns, cursor)->second;
         }
         else if (size_t(cursor) - m_back_segconns.size() < m_forward_segconns.size()) {
            weight = depthmapX::getMapAtIndex(m_forward_segconns, cursor - int(m_back_segconns.size()))->second;
         }
         break;
      case SEG_CONN_FW:
         weight = depthmapX::getMapAtIndex(m_forward_segconns, cursor)->second;
         break;
      case SEG_CONN_BK:
         weight = depthmapX::getMapAtIndex(m_back_segconns, cursor)->second;
         break;
      }
   }
   return weight;
}
