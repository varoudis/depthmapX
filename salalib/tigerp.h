// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2018, Petros Koutsolampros

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

#pragma once

#include "genlib/p2dpoly.h"
#include <vector>
#include <map>

// look up is the tiger (major) line category:
// string is A1, A2, A3 (road types) or B1, B2 (railroad types)
// C,D etc are not currently parsed, but given the nice file format 
// (thank you US Census Bureau!) they can easily be added

class TigerChain
{
public:
   std::vector<Line> lines;
   TigerChain() {;}
};

class TigerCategory
{
public:
   std::vector<TigerChain> chains;
   TigerCategory() {;}
};

class TigerMap
{
protected:
   QtRegion m_region;
   bool m_init;
public:
   std::map<std::string,TigerCategory> m_categories;
   TigerMap() { m_init = false;}

   void parse(const std::vector<std::string> &fileset, Communicator *communicator);

   Point2f getBottomLeft()
   { return m_region.bottom_left; }
   Point2f getTopRight()
   { return m_region.top_right; }
   QtRegion getRegion()
   { return m_region; }
};
