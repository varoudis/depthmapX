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

// Feasibilty study for GML loader

#include <iostream>
#include <fstream>

using namespace std;

#include <genlib/paftl.h>
#include <genlib/comm.h>  // For communicator

#include <salalib/mgraph.h>
#include "gmlmap.h"

bool GMLMap::parse(const pvecstring& fileset, Communicator *comm)
{
   __time64_t time = 0;
   qtimer( time, 0 );
     
   bool firstever = true;
   int tracking = 0;
   int linecount = 0;
   int feature = -1;
   std::string key;

   for (int i = 0; i < fileset.size(); i++) {

      ifstream stream(fileset[i].c_str());

      while (!stream.eof())
      {
         std::string line(4096);
         stream >> line;
         linecount++;

         if (line.length()) {
            switch (tracking) {
               case 0:
                  if (compare(line,"<osgb:RoadLink",14)) {
                     feature = 0;
                     tracking = 1;
                  }
                  else if (compare(line,"<osgb:PathLink",14)) {
                     feature = 1;
                     tracking = 1;
                  }
                  else if (compare(line,"<osgb:ConnectingLink",20)) {
                     feature = 2;
                     tracking = 1;
                  }
                  break;
               case 1:
                  if (compare(line,"</osgb:RoadLink",15) || compare(line,"</osgb:PathLink",15) || compare(line,"</osgb:ConnectingLink",21)) {
                     tracking = 0;
                     feature = -1;
                  }
                  else if (compare(line,"<osgb:descriptiveTerm>",22)) {
                     // pick out description (might not be only one)
                     int begin = line.findindex('>');
                     int end = line.findindexreverse('<');
                     if (begin != -1 && end != -1) {
                        auto thisdesc = line.substr(begin+1,end-begin-1);
                        if (key.empty()) {
                           key = thisdesc;
                        }
                        else {
                           key = key + " / " + thisdesc;
                        }
                     }
                  }
                  else if (compare(line,"<gml:LineString",15)) {
                     tracking = 2;
                  }
                  break;
               case 2:
                  if (compare(line,"</gml:LineString",16)) {
                     key.clear();
                     tracking = 1;
                  }
                  else if (compare(line,"<gml:co",7)) {
                     pvecstring tokens = line.tokenize(' ',true);
                     pvecpoint poly;
                     for (int j = 0 ; j < tokens.size(); j++) {
                        if (j == tokens.size() - 1) {
                           // strip </gml:coordinate>
                           int x = tokens[j].findindexreverse('<');
                           tokens[j] = tokens[j].substr(0,x);
                        }
                        if (j == 0) {
                           // strip <gml:coordinate>
                           int x = tokens[j].findindex('>');
                           tokens[j] = tokens[j].substr(x+1);
                        }
                        pvecstring subtokens = tokens[j].tokenize(',');
                        if (subtokens.size() == 2) {
                           poly.push_back( Point2f(subtokens[0].c_double(),subtokens[1].c_double()) );
                           if (!firstever) {
                              m_region.encompass(poly.tail());
                           }
                           else {
                              m_region = QtRegion(poly.head(),poly.head());
                              firstever = false;
                           }
                        }
                     }
                     if (poly.size()) {
                        if (key.empty()) {
                           key = "Miscellaneous";
                        }
                        int n = m_keys.searchindex(key);
                        if (n == -1) {
                           n = m_keys.add(key,polyset(),paftl::ADD_HERE);
                        }
                        m_keys.value(n).push_back(poly);
                     }
                  }
                  break;
            }
         }

         if (comm)
         {
            if (qtimer( time, 500 )) {
               if (comm->IsCancelled()) {
                  throw Communicator::CancelledException();
               }
            }
         }
      }
   }

   return !firstever;
}
