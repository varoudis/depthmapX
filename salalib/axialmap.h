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


#ifndef __SHAPEGRAPH_H__
#define __SHAPEGRAPH_H__

#include "salalib/spacepixfile.h"
#include "spacepix.h"
#include "connector.h"

struct AxialVertex;
struct AxialVertexKey;
struct RadialLine;
struct PolyConnector;

struct ValuePair;

// used during angular analysis
struct AnalysisInfo
{
   // lists used for multiple radius analysis
   bool leaf;
   bool choicecovered;
   SegmentRef previous;
   int depth;
   double choice;            
   double weighted_choice;
   double weighted_choice2; //EFEF
   AnalysisInfo() {
      choicecovered = false; leaf = true; previous = SegmentRef(); depth = 0; choice = 0.0; weighted_choice = 0.0; weighted_choice2 = 0.0; 
   }
   void clearLine() {
      choicecovered = false; leaf = true; previous = SegmentRef(); depth = 0; // choice values are cummulative and not cleared
   }
};

class MapInfoData;

typedef std::vector<std::vector<int>> KeyVertices;

class ShapeGraph : public ShapeMap
{
   friend class AxialMinimiser;
   friend class MapInfoData;
protected:
   KeyVertices m_keyvertices;       // but still need to return keyvertices here
   int m_keyvertexcount;
protected:
public:
   bool outputMifPolygons(std::ostream& miffile, std::ostream& midfile) const;
   void outputNet(std::ostream& netfile) const;
public:
   ShapeGraph(const std::string& name = "<axial map>", int type = ShapeMap::AXIALMAP);
   virtual ~ShapeGraph() {;}
   void initialiseAttributesAxial();
   void makeConnections(const KeyVertices &keyvertices = KeyVertices());
   //void initAttributes();
   bool integrate(Communicator *comm = NULL, const pvecint& radius = pvecint(), bool choice = false, bool local = false, bool fulloutput = false, int weighting_col = -1, bool simple_version = true);
   bool stepdepth(Communicator *comm = NULL);
   bool analyseAngular(Communicator *comm, const pvecdouble& radius);
   // extra parameters for selection_only and interactive are for parallel process extensions
   int analyseTulip(Communicator *comm, int tulip_bins, bool choice, int radius_type, const pvecdouble& radius, int weighting_col, int weighting_col2 = -1, int routeweight_col = -1, bool selection_only = false, bool interactive = true);
   bool angularstepdepth(Communicator *comm);
   // the two topomet analyses can be found in topomet.cpp:
   bool analyseTopoMet(Communicator *comm, int analysis_type, double radius, bool sel_only);
   bool analyseTopoMetPD(Communicator *comm, int analysis_type);
   // lineset and connectionset are filled in by segment map
   void makeNewSegMap();
   void makeSegmentMap(std::vector<Line> &lineset, prefvec<Connector>& connectionset, double stubremoval);
   void initialiseAttributesSegment();
   void makeSegmentConnections(prefvec<Connector>& connectionset);
   void pushAxialValues(ShapeGraph& axialmap);
   //
   virtual bool read( std::istream& stream, int version );
   bool readold( std::istream& stream, int version );
   virtual bool write( std::ofstream& stream, int version );
   void writeAxialConnectionsAsDotGraph(std::ostream &stream);
   void writeAxialConnectionsAsPairsCSV(std::ostream &stream);
   void writeSegmentConnectionsAsPairsCSV(std::ostream &stream);
   void unlinkAtPoint(const Point2f& unlinkPoint);
   void unlinkFromShapeMap(const ShapeMap& shapemap);
};

#endif
