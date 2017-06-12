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


#ifndef __MGRAPH_H__
#define __MGRAPH_H__

// Interface: the meta graph loads and holds all sorts of arbitrary data...

// Current metagraph version
const int METAGRAPH_VERSION = 440;

// Human readable(ish) metagraph version changes

const int VERSION_ALWAYS_RECORD_BINDISTANCES    = 440;

// 17-Aug-2010 Version stamp for Depthmap 10.08.00
const int VERSION_100800                        = 430;

// 12-Jul-2010 Version stamp for Depthmap 10.07.00
const int VERSION_100700                        = 430;

// 12-Jul-2010 Occlusion distances for agent control test
const int VERSION_OCCDISTANCES                  = 430;

// 07-Feb-2010 Axial lines no longer self-connect
const int VERSION_NO_SELF_CONNECTION            = 420;

// 31-Jan-2009 Maps have sublayers
const int VERSION_MAP_LAYERS                    = 410;
//
// 390 and 400 used only for testing
//
// 31-Jan-2009 this is simply a version stamp for Depthmap 8.14.00 and 8.15.00
// The idea is that I should start writing code that saves in version 380 format
// as a benchmark "save for older version"
const int VERSION_81500                         = 380;
const int VERSION_81400                         = 380;
//
// 20-Apr-2008 The shape map name lookup could have been corrupted.  In addition it's not that useful (never much to sort through, unique layer names not necessary)
const int VERSION_NO_SHAPEMAP_NAME_LOOKUP       = 380;
// 15-Mar-2008
const int VERSION_SHAPE_AREA_PERIMETER          = 370;
const int VERSION_FORGET_COLUMN_CREATOR         = 370;
// 04-Oct-2007
const int VERSION_MAP_TYPES                     = 360;
// 20-Sep-2007
const int VERSION_STORE_COLUMN_CREATOR          = 350;
const int VERSION_ATTRIBUTE_LOCKING             = 350;
// version 340 unused
// 08-Jun-2007: Store all drawing layers as shape maps rather than spacepixels (continued)
const int VERSION_DRAWING_SHAPES_B              = 330;
// 27-Nov-2006: Store all drawing layers as shape maps rather than spacepixels
const int VERSION_DRAWING_SHAPES                = 320;
// 27-Nov-2006: Store axial maps as children of shape maps rather than spacepixels
const int VERSION_AXIAL_SHAPES                  = 310;
// 01-Sep-2006: Store formulae for columns
const int VERSION_STORE_FORMULA                 = 300;
// 14-May-2006: Occlusions with node
const int VERSION_OCCLUSIONS                    = 290;
// 28-Dec-2005: Polygon shapes have centroids
const int VERSION_SHAPE_CENTROIDS               = 280;
// 18-Dec-2005: Mapinfo data read into shape maps instead of axial maps
const int VERSION_MAPINFO_SHAPES                = 270;
// 14-Sep-2005: QtRegion bug with segment maps from imported axial maps fixed:
const int VERSION_AXIAL_REGION_FIX              = 263;
// 01-Sep-2005: Now Pointmaps really ought to store their names:
const int VERSION_POINT_MAP_NAMES               = 262;
// 25-Aug-2005: And an extension to shape maps to make them easier to use as lines or points
const int VERSION_EXTENDED_SHAPE_MAPS           = 261;
// 23-Aug-2005: Although in test from 11-Aug-2005, useful to read the testing graphs created:
const int VERSION_SHAPE_MAPS                    = 260;
// 11-Aug-2005: Also, a set of PointMaps now replace a single instance of PointData
const int VERSION_POINT_MAPS                    = 251;
// 11-Aug-2005: Location stored with points, not depixelated on the fly
const int VERSION_POINT_LOCATIONS               = 250;
// 01-Mar-2005: Quick grid connections
const int VERSION_GRID_CONNECTIONS              = 240;
// 02-Dec-2004: Axial map gates
const int VERSION_GATE_MAPS                     = 230;
// 29-Oct-2004: Store the colour display settings with the graph data
const int VERSION_STORE_GRIDTEXTINFO            = 220;
// 29-Oct-2004: Store the colour display settings with the graph data
const int VERSION_STORE_COLOR                   = 210;
// 16-Jun-2004: New boundary graph (now much simpler: nodes at edge of main graph)
const int VERSION_NEWBOUNDARYGRAPH              = 200;
// 20-May-2004: Each segment must have forward and backward connections listed separately!
const int VERSION_SEGMENT_MAPS_FIX              = 191;
// 17-May-2004: Axial maps can be either segment or axial maps.  Affects ShapeGraph and AxialLine classes
const int VERSION_SEGMENT_MAPS                  = 190;
// 12-May-2004: Extra Mapinfo table data
const int VERSION_MAPINFO_DATA                  = 180;
// 06-May-2004: Explicit links and unlinks for axial lines
const int VERSION_AXIAL_LINKS                   = 170;
// 29-Feb-2004: Attributes table (already used for AxialLines) now used for PointData as well
const int VERSION_ATTRIBUTES_TABLE              = 160;
// File compression introduced
const int VERSION_FILE_COMPRESSION              = 150;
// Some minor modifications to the axial line format... won't load v.130 files
const int VERSION_REVISED_AXIAL                 = 140;
// View class specifies whether axial or vga currently viewed
const int VERSION_VIEW_CLASS_ADDED              = 130;
// A distance stored in the bin
const int VERSION_BINDISTANCES                  = 120;
// A set of nodes on the boundaries of the isovist
const int VERSION_BOUNDARYGRAPH                 = 110;
// Dynamic lines (addable and removable) in the visibility graph
const int VERSION_DYNAMICLINES                  = 100;
// Line layers are coloured...
const int VERSION_LAYERCOLORS                   = 91;
// Blocked locations split into 4, replaces m_noderef
const int VERSION_BLOCKEDQUAD                   = 90;
// Space pixel groups have different space pixels for different layers (at their own resolution!)
const int VERSION_SPACEPIXELGROUPS              = 80;
// The graph state is just recorded
const int VERSION_STATE_RECORDED                = 72;
// The binsizes weren't included in the metagraph 70
const int VERSION_NGRAPH_BINCOUNT               = 71;
// Major, major changes to the graph format (from now on it will now be held in memory only)
const int VERSION_NGRAPH_INTROD                 = 70;
// Slight changes to PointData required for the actual implementation of the quick graph
const int VERSION_SPARK_GRAPH_INTROD            = 61;
// Quick graph... add underlying info about lines into the pointdata structure
const int VERSION_QUICK_GRAPH_INTROD            = 60;
// Layers
const int VERSION_LAYERS_CENTROID_INTROD        = 51;
const int VERSION_LAYERS_INTROD                 = 50;
// version 41 repairs VERSION_EXTRA_POINT_DATA_INTROD bug
const int VERSION_EXTRA_POINT_DATA_INTROD       = 40;
const int VERSION_BINS_INTROD                   = 30;

///////////////////////////////////////////////////////////////////////////////

const unsigned int SALA_SELECTED_COLOR = 0x00FFFF77;
const unsigned int SALA_HIGHLIGHTED_COLOR = 0x0077FFFF;

///////////////////////////////////////////////////////////////////////////////

// Parse errors for MapInfo:

enum { MINFO_OK, MINFO_HEADER, MINFO_TABLE, MINFO_MIFPARSE, MINFO_OBJROWS, MINFO_MULTIPLE };

///////////////////////////////////////////////////////////////////////////////

#include <genlib/paftl.h>
#include <genlib/p2dpoly.h>

#include <salalib/vertex.h>      // mainly deprecated, but includes display params
#include <salalib/fileproperties.h>
#include <salalib/spacepix.h>
#include <salalib/attributes.h>

#include <salalib/pointdata.h>
#include <salalib/connector.h>
#include <salalib/shapemap.h>
#include <salalib/axialmap.h>
#include <salalib/datalayer.h>   // datalayers deprecated

// for agent engine interface
#include <salalib/nagent.h>

///////////////////////////////////////////////////////////////////////////////////

class Communicator;

// A meta graph is precisely what it says it is

class MetaGraph : public SuperSpacePixel, public PointMaps, public FileProperties
{
public:
   enum { ADD = 0x0001, REPLACE = 0x0002, CAT = 0x0010, DXF = 0x0020, NTF = 0x0040, RT1 = 0x0080, GML = 0x0100 };
   enum { NONE = 0x0000, /* GRAPH = 0x0001, Deprecated */ POINTMAPS = 0x0002, LINEDATA = 0x0004, 
          FIXED = 0x0008 /* Deprecated */, ANGULARGRAPH = 0x0010, DATAMAPS = 0x0020, AXIALLINES = 0x0040, /* BOUNDARYGRAPH = 0x0080, Deprecated */ SHAPEGRAPHS = 0x0100,
          PREWRITTEN = 0x1000, BUGGY = 0x8000 };
   enum { NOT_EDITABLE = 0, EDITABLE_OFF = 1, EDITABLE_ON = 2 };
protected:
   int m_file_version;
   int m_state;
   void *m_lock;
public:
   // some display options now set at file level
   bool m_showgrid;
   bool m_showtext;
   //
public:
   ShapeMaps<ShapeMap> m_data_maps;
   ShapeGraphs m_shape_graphs;
public:
   MetaGraph();
   ~MetaGraph();
   //
   int getVersion()
   {
      // note, if unsaved, m_file_version is -1
      return m_file_version;
   }
   bool setLock(void *who) 
   { 
      if (m_lock == 0) {
         m_lock = who;
         return true;
      }
      return false;
   }
   bool releaseLock(void *who)
   {
      if (m_lock != who) {
         return false;
      }
      m_lock = NULL;
      return true;
   }
   //
   void copyLineData(const SuperSpacePixel& meta);
   void copyPointMap(const PointMap& meta);
   //
   int getState() const
      { return m_state; }
   // use with caution: only very rarely needed outside MetaGraph itself
   void setState(int state)
      { m_state = state; }
   //
   // quick loaders from input streams rather than files:
   bool importCat( istream& stream );
   bool importDxf( istream& stream );
   // make a graph using the supplied seed and graph spacing:
   void fastGraph( const Point2f& seed, double spacing );
   //
   int loadLineData( Communicator *communicator, int load_type );
   int loadCat( istream& stream, Communicator *communicator );
   int loadDxf( istream& stream, Communicator *communicator );
   // Quick mod - TV
#if defined(_WIN32)   
   int loadRT1(const pqvector<wstring>& fileset, Communicator *communicator);
#else
   int loadRT1(const pqvector<string>& fileset, Communicator *communicator);
#endif   
   int importTxt( istream& stream, const pstring& layername, bool csv );
   bool undoPoints();
   bool clearPoints();
   bool setGrid( double spacing, const Point2f& offset = Point2f() );                 // override of PointMap
   bool makePoints( const Point2f& p, int semifilled, Communicator *communicator = NULL);  // override of PointMap
   bool makeGraph( Communicator *communicator, int algorithm, double maxdist );
   bool analyseGraph(Communicator *communicator, Options options , bool simple_version); // <- options copied to keep thread safe
   bool analyseAngular( Communicator *communicator, bool analyse_in_memory );
   bool makeAxialLines( Communicator *communicator, bool analyse_in_memory );
   //
   // helpers for editing maps
   bool isEditableMap();
   ShapeMap& getEditableMap();
   // currently only making / moving lines, but should be able to extend this to polys fairly easily:
   bool makeShape(const Line& line);
   bool moveSelShape(const Line& line);
   // onto polys as well:
   bool polyBegin(const Line& line);
   bool polyAppend(const Point2f& point);
   bool polyClose();
   bool polyCancel();
   //
   int addShapeGraph(const pstring& name, int type);
   int addShapeMap(const pstring& name);
   void removeDisplayedMap();
   //
   // various map conversions
   bool convertDrawingToAxial(Communicator *comm, pstring layer_name);  // n.b., name copied for thread safety
   bool convertDataToAxial(Communicator *comm, pstring layer_name, bool keeporiginal, bool pushvalues);
   bool convertDrawingToSegment(Communicator *comm, pstring layer_name);
   bool convertDataToSegment(Communicator *comm, pstring layer_name, bool keeporiginal, bool pushvalues);
   bool convertToData(Communicator *comm, pstring layer_name, bool keeporiginal, int typeflag);  // -1 signifies convert from drawing layer, else convert from data map
   bool convertToDrawing(Communicator *comm, pstring layer_name, int typeflag); // 0 signifies convert from data map, else convert from graph
   bool convertToConvex(Communicator *comm, pstring layer_name, bool keeporiginal, int typeflag); // -1 signifies convert from drawing layer, else convert from data map
   bool convertAxialToSegment(Communicator *comm, pstring layer_name, bool keeporiginal, bool pushvalues, double stubremoval);
   // note: not same categories
   bool convertPointsToShape();
   //bool convertBoundaryGraph( Communicator *communicator );
   //
   // some compatibility with older version horrors:
   int convertDataLayersToShapeMap(DataLayers& datalayers, PointMap& pointmap);
   void convertShapeGraphToShapeMap(const ShapeGraph& axialmap);
   //
   int loadMifMap(Communicator *comm, istream& miffile, istream& midfile);
   bool makeAllLineMap( Communicator *communicator, const Point2f& seed );
   bool makeFewestLineMap( Communicator *communicator, int replace );
   bool analyseAxial( Communicator *communicator, Options options, bool simple_version ); // <- options copied to keep thread safe
   bool analyseSegments( Communicator *communicator, Options options ); // <- options copied to keep thread safe
   bool analyseTopoMet( Communicator *communicator, Options options ); // <- options copied to keep thread safe
   //
   bool hasAllLineMap()
   { return m_shape_graphs.hasAllLineMap(); }
   //
   enum { PUSH_FUNC_MAX = 0, PUSH_FUNC_MIN = 1, PUSH_FUNC_AVG = 2, PUSH_FUNC_TOT = 3};
   bool pushValuesToLayer(int desttype, int destlayer, int push_func, bool count_col = false);
   bool pushValuesToLayer(int sourcetype, int sourcelayer, int desttype, int destlayer, int col_in, int col_out, int push_func, bool count_col = false);
   //
   int getDisplayedMapRef() const;
   //
   // NB -- returns 0 (not editable), 1 (editable off) or 2 (editable on)
   int isEditable() const;
   bool canUndo() const;
   void undo();
   //
   ShapeGraph& getDisplayedShapeGraph()
   { return m_shape_graphs.getDisplayedMap(); }
   ShapeMap& getDisplayedDataMap()
   { return m_data_maps.getDisplayedMap(); }
   ShapeGraphs& getShapeGraphs()
   { return m_shape_graphs; }
   ShapeMaps<ShapeMap>& getDataMaps()
   { return m_data_maps; }
   //
   int getDisplayedMapType();
   //
   QtRegion getBoundingBox() const;
   //
   int getDisplayedAttribute() const;
   void setDisplayedAttribute(int col);
   int addAttribute(const pstring& name);
   void removeAttribute(int col);
   bool isAttributeLocked(int col);
   AttributeTable& getAttributeTable(int type = -1, int layer = -1);
   const AttributeTable& getAttributeTable(int type = -1, int layer = -1) const;
   //
   void loadGraphAgent();
   void unloadGraphAgent();
   //
   int getLineFileCount() const
      { return (int) SuperSpacePixel::size(); }

   // Quick mod - TV
   const pstring& getLineFileName(int file) const
      { return SuperSpacePixel::at(file).getName(); }
   int getLineLayerCount(int file) const
      { return (int) SuperSpacePixel::at(file).size(); }

   //
/*   SpacePixel& getLineLayer(int file, int layer)
      { return SuperSpacePixel::at(file).at(layer); }
   const SpacePixel& getLineLayer(int file, int layer) const
      { return SuperSpacePixel::at(file).at(layer); }*/
   ShapeMap& getLineLayer(int file, int layer)
      { return SuperSpacePixel::at(file).at(layer); }
   const ShapeMap& getLineLayer(int file, int layer) const
      { return SuperSpacePixel::at(file).at(layer); }
   //
   // Some error handling -- the idea is that you catch the error in MetaGraph,
   // return a generic error code and then get your front end to interrogate the 
   // last error (pretty much as per standard Unix etc).
   // May have problems with multithreading.
public:
   class Error 
   {
   protected:
      pstring error;
   public:
      Error(const pstring& err = pstring()) { error = err; }
   };
protected:
   Error m_last_error;
public:
   Error& getLastError()
   { return m_last_error; }
   // for drawing either axial analysis or VGA
protected:
   int m_view_class;
public:
   enum { SHOWHIDEVGA = 0x0100, SHOWVGATOP = 0x0200, SHOWHIDEAXIAL = 0x0400, SHOWAXIALTOP = 0x0800, SHOWHIDESHAPE = 0x1000, SHOWSHAPETOP = 0x2000 };
   enum { VIEWNONE = 0x00, VIEWVGA = 0x01, VIEWBACKVGA = 0x02, VIEWAXIAL = 0x04, VIEWBACKAXIAL = 0x08,
          VIEWLINKS = 0x10, VIEWDATA = 0x20, VIEWBACKDATA = 0x40, VIEWFRONT = 0x25, VIEWBACK = 0x4a };
   //
   int getViewClass()
   { return m_view_class; }
   // These functions make specifying conditions to do things much easier:
   bool viewingNone()
   { return (m_view_class == VIEWNONE); }
   bool viewingProcessed()
   { return ((m_view_class & (VIEWAXIAL | VIEWDATA)) || (m_view_class & VIEWVGA && getDisplayedPointMap().isProcessed())); }
   bool viewingShapes()
   { return (m_view_class & (VIEWAXIAL | VIEWDATA)) != 0; }
   bool viewingProcessedLines()
   { return ((m_view_class & VIEWAXIAL) == VIEWAXIAL); } 
   bool viewingProcessedShapes()
   { return ((m_view_class & VIEWDATA) == VIEWDATA); } 
   bool viewingProcessedPoints()
   { return ((m_view_class & VIEWVGA) && getDisplayedPointMap().isProcessed()); }
   bool viewingUnprocessedPoints()
   { return ((m_view_class & VIEWVGA) && !getDisplayedPointMap().isProcessed()); }
   //
   bool setViewClass(int command);
   //
   double getLocationValue(const Point2f& point);
   //
public:
   // these are dependent on what the view class is:
   bool isSelected()                              // does a selection exist
   {  if (m_view_class & VIEWVGA) 
         return getDisplayedPointMap().isSelected();
      else if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().isSelected();
      else if (m_view_class & VIEWDATA) 
         return m_data_maps.getDisplayedMap().isSelected();
      else 
         return false;
   }
   bool setCurSel( QtRegion& r, bool add = false )  // set current selection
   {  if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().setCurSel(r, add);
      else if (m_view_class & VIEWDATA)
         return m_data_maps.getDisplayedMap().setCurSel( r, add );
      else if (m_view_class & VIEWVGA)
         return getDisplayedPointMap().setCurSel( r, add );
      else if (m_state & POINTMAPS && !getDisplayedPointMap().isProcessed()) // this is a default select application
         return getDisplayedPointMap().setCurSel( r, add );
      else if (m_state & DATAMAPS) // I'm not sure why this is a possibility, but it appears you might have state & DATAMAPS without VIEWDATA...
         return m_data_maps.getDisplayedMap().setCurSel( r, add );
      else 
         return false;
   }
   bool clearSel()
   {
      // really needs a separate clearSel for the datalayers... at the moment this is handled in PointMap
      if (m_view_class & VIEWVGA)
         return getDisplayedPointMap().clearSel();
      else if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().clearSel();
      else if (m_view_class & VIEWDATA) 
         return m_data_maps.getDisplayedMap().clearSel();
      else
         return false;
   }
   int getSelCount()
   {
      if (m_view_class & VIEWVGA)
         return getDisplayedPointMap().getSelCount();
      else if (m_view_class & VIEWAXIAL) 
         return (int) m_shape_graphs.getDisplayedMap().getSelCount();
      else if (m_view_class & VIEWDATA) 
         return (int) m_data_maps.getDisplayedMap().getSelCount();
      else
         return 0;
   }
   float getSelAvg()
   {
      if (m_view_class & VIEWVGA)
         return (float)getDisplayedPointMap().getAttributeTable().getSelAvg();
      else if (m_view_class & VIEWAXIAL) 
         return (float)m_shape_graphs.getDisplayedMap().getAttributeTable().getSelAvg();
      else if (m_view_class & VIEWDATA) 
         return (float)m_data_maps.getDisplayedMap().getAttributeTable().getSelAvg();
      else
         return -1.0f;
   }
   QtRegion getSelBounds()
   {
      if (m_view_class & VIEWVGA)
         return getDisplayedPointMap().getSelBounds();
      else if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().getSelBounds();
      else if (m_view_class & VIEWDATA) 
         return m_data_maps.getDisplayedMap().getSelBounds();
      else
         return QtRegion();
   }
   // setSelSet expects a set of ref ids:
   void setSelSet(const pvecint& selset, bool add = false)
   { if (m_view_class & VIEWVGA && m_state & POINTMAPS)
         getDisplayedPointMap().setCurSel(selset,add);
      else if (m_view_class & VIEWAXIAL) 
         m_shape_graphs.getDisplayedMap().setCurSel(selset,add);
      else // if (m_view_class & VIEWDATA) 
         m_data_maps.getDisplayedMap().setCurSel(selset,add); }
   pvecint& getSelSet()
   {  if (m_view_class & VIEWVGA && m_state & POINTMAPS)
         return getDisplayedPointMap().getSelSet();
      else if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().getSelSet();
      else // if (m_view_class & VIEWDATA) 
         return m_data_maps.getDisplayedMap().getSelSet(); }
   const pvecint& getSelSet() const
   {  if (m_view_class & VIEWVGA && m_state & POINTMAPS)
         return getDisplayedPointMap().getSelSet();
      else if (m_view_class & VIEWAXIAL) 
         return m_shape_graphs.getDisplayedMap().getSelSet();
      else // if (m_view_class & VIEWDATA) 
         return m_data_maps.getDisplayedMap().getSelSet(); }
//
public:
   // no longer supported
   //int addLineDynamic(const Line& l);
   //bool removeLineDynamic(LineKey linekey);
   //
   // Agent engine interface:
protected:
   AgentEngine m_agent_engine;
public:
   AgentEngine& getAgentEngine()
   { return m_agent_engine; }
   void runAgentEngine(Communicator *comm);
   //
public:
   // thru vision
   bool analyseThruVision(Communicator *comm = NULL, int gatelayer = -1);
   // BSP tree for making isovists
protected:
   BSPNode *m_bsp_root;
   bool m_bsp_tree;
public:
   bool makeBSPtree(Communicator *communicator = NULL);
   void resetBSPtree() { m_bsp_tree = false; }
   // returns 0: fail, 1: made isovist, 2: made isovist and added new shapemap layer
   int makeIsovist(Communicator *communicator, const Point2f& p, double startangle = 0, double endangle = 0, bool simple_version = true);
   // returns 0: fail, 1: made isovist, 2: made isovist and added new shapemap layer
   int makeIsovistPath(Communicator *communicator, double fov_angle = 2.0 * M_PI, bool simple_version = true);
   bool makeIsovist(const Point2f& p, Isovist& iso);
protected:
   // properties
public:
   // a few read-write returns:
   enum { OK, WARN_BUGGY_VERSION, WARN_CONVERTED, NOT_A_GRAPH, DAMAGED_FILE, DISK_ERROR, NEWER_VERSION, DEPRECATED_VERSION };
   // likely to use communicator if too slow...
   int read( const pstring& filename );
   int write( const pstring& filename, int version, bool currentlayer = false);
   //
protected:
   pqvector<AttrBody> *m_attr_conv_table;
   int convertAttributes( ifstream& stream, int version );
   int convertVirtualMem( ifstream& stream, int version );
   //
   streampos skipVirtualMem(ifstream& stream, int version);
   streampos copyVirtualMem(ifstream& reader, ofstream& writer, int version);
};

///////////////////////////////////////////////////////////////////////////////

#endif
