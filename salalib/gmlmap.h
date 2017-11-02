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

#ifndef __GMLMAP_H__
#define __GMLMAP_H__

#include <string>
#include <vector>

typedef prefvec<pqvector<Point2f>> polyset;

class GMLMap
{
protected:
   QtRegion m_region;
public:
   pqmap<std::string,polyset> m_keys;
public:
   GMLMap() {;}
   bool parse(const std::vector<std::string>& fileset, Communicator *communicator);
   Point2f getBottomLeft()
   { return m_region.bottom_left; }
   Point2f getTopRight()
   { return m_region.top_right; }
   QtRegion getRegion()
   { return m_region; }
};

#endif
