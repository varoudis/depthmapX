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



#ifndef __TOPOMET_H__
#define __TOPOMET_H__

enum {TOPOMET_METHOD_TOPOLOGICAL = 0, TOPOMET_METHOD_METRIC = 1};

struct TopoMetSegmentRef {
   int ref;
   int dir;
   double dist;
   int previous;
   bool done;
   TopoMetSegmentRef(int r = -1, int d = -1, double di = 0.0, int p = -1)
   { ref = r; dir = d; dist = di; previous = p; done = false; }
};

// should be double not float!

struct TopoMetSegmentChoice {
   double choice;
   double wchoice;
   TopoMetSegmentChoice()
   { choice = 0.0; wchoice = 0.0; }
};

struct SegInfo {
   double length;
   int layer;
   SegInfo()
   { length = 0.0f; layer = 0; }
};



#endif
