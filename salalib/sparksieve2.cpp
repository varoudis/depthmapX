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



// This is my code to make a set of axial lines from a set of boundary lines

/////////////////////////////////////////////////////////////////////////////////

// New spark sieve implemementation (more accurate)

#include <math.h>

#include <salalib/mgraph.h>
#include <salalib/spacepix.h>
#include <salalib/datalayer.h>
#include <salalib/pointdata.h>

#include "sparksieve2.h"

sparkSieve2::sparkSieve2( const Point2f& centre, double maxdist )
{
   m_centre = centre;
   m_maxdist = maxdist;

   m_gaps.push_back( sparkZone2(0.0, 1.0) );
}

sparkSieve2::~sparkSieve2()
{
}

bool sparkSieve2::testblock( const Point2f& point, const pqmap<int,Line>& lines, double tolerance )
{
   Line l(m_centre, point);

   // maxdist is to construct graphs with a maximum visible distance: (-1.0 is infinite)
   if (m_maxdist != -1.0 && l.length() > m_maxdist) {
      return true;
   }

   for (size_t i = 0; i < lines.size(); i++)
   {
      // Note: must check regions intersect before using this intersect_line test -- see notes on intersect_line
      if (intersect_region(l,lines.value(i),tolerance) && intersect_line(l,lines.value(i),tolerance)) {
         return true;
      }
   }

   return false;
}

//

void sparkSieve2::block( const pqmap<int,Line>& lines, int q )
{
   for (size_t i = 0; i < lines.size(); i++) {
      double a = tanify(lines.value(i).start(), q);
      double b = tanify(lines.value(i).end(), q);

      sparkZone2 block;
      if (a < b) {
         block.start = a - 1e-10;   // 1e-10 required for floating point error
         block.end = b + 1e-10;
      }
      else {
         block.start = b - 1e-10;   // 1e-10 required for floating point error
         block.end = a + 1e-10;
      }
      // this creates a list of blocks sorted by start location
      m_blocks.add(block);
   }
}

void sparkSieve2::collectgarbage()
{
   m_gaps.first();

   for (size_t i = 0; i < m_blocks.size() && !m_gaps.is_tail(); i++)
   {
      if (m_blocks[i].end < (*m_gaps).start) {
         continue;
      }
      sparkZone2& block = m_blocks[i];
      bool create = true;
      if (block.start <= (*m_gaps).start) {
         create = false;
         if (block.end > (*m_gaps).start) {
            // simply move the start in front of us
            (*m_gaps).start = block.end;
         }
      }
      if (block.end >= (*m_gaps).end) {
         create = false;
         if (block.start < (*m_gaps).end) {
            // move the end behind us
            (*m_gaps).end = block.start;
         }
      }
      if ((*m_gaps).end <= (*m_gaps).start + 1e-10) { // 1e-10 required for floating point error
         i--;  // on the next iteration, stay with this block
         m_gaps.postdel();
      }
      else if (block.end > (*m_gaps).end) {
         i--;  // on the next iteration, stay with this block
         m_gaps++;
      }
      else if (create) {
         // add a new gap (has to be behind us), and move the start in front of us
         m_gaps.preins( sparkZone2( (*m_gaps).start, block.start ) );
         (*m_gaps).start = block.end;
      }
   }
   // reset blocks for next row:
   m_blocks.clear();
}

// q quadrants:
//
//      \ 6 | 7 /
//      0 \ | / 1
//      - -   - -
//      2 / | \ 3
//      / 4 | 5 \
//

double sparkSieve2::tanify( const Point2f& point, int q )
{
   switch (q)
   {
   case 0:
      return (point.y - m_centre.y) / (m_centre.x - point.x);
      break;
   case 1:
      return (point.y - m_centre.y) / (point.x - m_centre.x);
      break;
   case 2:
      return (m_centre.y - point.y) / (m_centre.x - point.x);
      break;
   case 3:
      return (m_centre.y - point.y) / (point.x - m_centre.x);
      break;
   case 4:
      return (m_centre.x - point.x) / (m_centre.y - point.y);
      break;
   case 5:
      return (point.x - m_centre.x) / (m_centre.y - point.y);
      break;
   case 6:
      return (m_centre.x - point.x) / (point.y - m_centre.y);
      break;
   case 7:
      return (point.x - m_centre.x) / (point.y - m_centre.y);
      break;
   }
   return -1.0;
}
