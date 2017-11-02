#ifndef __IDEPTHMAP_H__
#define __IDEPTHMAP_H__

///////////////////////////////////////////////////////////////////////////////////////

// File: idepthmap.h
// Author: Alasdair Turner
// Copyright (C) 2007-2010 University College London

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

// This defines interface for a DLL module library used in the Depthmap application
// THIRD PARTY DEVELOPERS: DO NOT MODIFY THIS FILE

// It's a little complicated to look at, but it doesn't actually *do* that much

// Firstly, it has miscellaneous structs to help the interface:
// struct DPoint;
// struct DLine;

// Secondly, it defines your interface to Depthmap:
// class IComm;          (to communicate the state of your analysis back to Depthmap)
// class IVGAMap;        (for VGA analysis)
// class IShapeMap;      (for axial, segment and data analysis)   
// class IAttributes;    (your interface to attribute values, whether or not
//                                   the graph is a visibility graph or any other sort of graph)
// class IGraphFile;     (for analysis of more than one map)   
//
// (Note, despite the separate attribute interface, if you wish to display a certain
//  attribute you must use the "setDisplayedAttribute" function within the relevant 
//  graph interface, i.e., through IVGAMap or IShapeMap)

// Thirdly, it defines function prototypes for three functions you implement:
//
// char *getModuleName();                    Return the name of your module (which will be added to the tools menu)
// char *getAnalysisName(int analysis);      Return the name of an individual analysis within the module (which will be added as a menu item to your module)
// int getAnalysisType(int analysis);        Return the type of analysis according to menu option.  
//
// getAnalysisType must have one of 
// DLL_NO_ANALYSIS, DLL_VGA_ANALYSIS, DLL_AXIAL_ANALYSIS, DLL_SEGMENT_ANALYSIS, DLL_DATA_ANALYSIS, DLL_ATTRIBUTE_ANALYSIS or DLL_GRAPH_FILE_ANALYSIS set, 
//
// bool preprocess(int analysis);      // Optional, for setting up
//
// Write the an appropriate function for your analysis type:
//
// bool processVGA(int analysis, IComm *, IVGAMap *, IAttributes *);       // For DLL_VGA_ANALYSIS
// bool processShape(int analysis, IComm *, IShapeMap *, IAttributes *);   // For DLL_AXIAL_ANALYSIS, DLL_SEGMENT_ANALYSIS or DLL_ATTRIBUTE_ANALYSIS
// bool processAttributes(int analysis, IComm *, IAttributes *);           // For DLL_ATTRIBUTE_ANALYSIS
// bool processGraphFile(int analysis, IComm *, IGraphFile *);             // For DLL_GRAPH_FILE_ANALYSIS
//
// void postprocess(int analysis);     // Optional, for any tidying up
//
// You MUST supply getModuleName, getAnalysisType, getAnalysisName 
//
// If the analysisType function specifies anything other than DLL_NO_ANALYSIS, then
// at least one of processVGA, processShape, processAttributes or processGraphFile must also be defined
// (as appropriate to the analysis type selected)
//
// The functions preprocess and postprocess are optional
//
// Note that DLL_NO_ANALYSIS calls preprocess only:
// For example, use this for to set module options or to display an about box
//

// Do not alter this version number, it is the minimum
// compatible version of Depthmap for your DLL
#define DLL_DEPTHMAP_VERSION 10.04

#include <math.h>
#include <set>
// Windows math.h does not define M_PI
#define DLL_PI 3.1415926535897932384626433832795

// Modified by Dream
#if defined(_WIN32)

#ifdef _DEPTHMAP
   // This is the version for the Depthmap app
   #define DepthmapClass __declspec(dllexport)
#else
   // This is the version for your module
   #define DepthmapClass __declspec(dllimport)
   #define DllProc __declspec(dllexport)
#endif

#else
   #define DepthmapClass
   #define DllProc
#endif

// this is a helper struct (with some basic functionality)
// this is not exported as DLLs do not support operator overrides
struct DepthmapClass DPoint
{
   double x;
   double y;
   DPoint()
   { x = 0.0; y = 0.0; }
   DPoint(double p, double q)
   { x = p; y = q; }
};

// this is a helper struct
struct DepthmapClass DLine
{
   DPoint a;
   DPoint b;
   DLine()
   { ; }
   DLine(const DPoint& p, const DPoint& q)
   { a = p; b = q; }
};

// send communications to and from the app:
// typically, set your number of records, then each iteration set the current record,
// check isProcessCancelled -- if so, please tidy up and return false from process function
class DepthmapClass IComm
{
public:
   IComm();
   virtual ~IComm();
   bool isProcessCancelled();
   void setNumSteps(int steps);
   void setCurrentStep(int step);
   void setNumRecords(int records);
   void setCurrentRecord(int record);
   //
   bool isValid();
public:
   // IComm messages: specify the callback function below if you want to use these
   enum { ICOMM_NUM_STEPS, ICOMM_CURRENT_STEP, ICOMM_NUM_RECORDS, ICOMM_CURRENT_RECORD };
public:
	// These are intended to be used through SalaDLL rather than standard Depthmap
   void cancelProcess();
   int getNumSteps();
   int getCurrentStep();
   int getNumRecords();
   int getCurrentRecord();
   //
   // Do NOT close the IComm unless you know what you are doing:
   void close();
private:
   // INTERNAL TO DEPTHMAP
   // the data that is transferred to your app:
   // do not change this variable
   void *m_data;
public:
   // INTERNAL TO DEPTHMAP
   // Depthmap calls these functions to interface the data
   // do not call these functions
   void setData(void *data);
   void *getData();
};

// avoid using this in your own Depthmap DLLs (it is designed for the sala.dll redistributable)
// instead, use the IComm passed to you via the processing functions
DepthmapClass IComm *getIComm();

// this class forms your interface to Depthmap VGA functionality:
class DepthmapClass IVGAMap
{
// your public interface:
public:
   IVGAMap();
   virtual ~IVGAMap();
public:
   enum {IVGA_STEP, IVGA_METRIC, IVGA_ANGULAR};
protected:
   // POINT INTERFACE
   // this is the main iterator, it is used for all getPoint... functions
   int m_cursor_point;
   // secondary iterator for selected points
   std::set<int>::iterator m_cursor_selected_point;
public:
   // if you know the id, you can move the cursor to this point:
   void setCursorPoint(int id)
   { m_cursor_point = id; }
   //
   // iterator for all points (uses m_cursor_point)
   int getPointCount();
   int getFirstPoint();
   int getNextPoint();
   //
   // iterator for selected points in the Depthmap UI (uses m_cursor_selected_point)
   int getSelectedPointCount();
   int getFirstSelectedPoint();
   int getNextSelectedPoint();
   //
   // clear selection: straight forward I hope
   void clearSelection();
   // must know the id to get this (if replace is "true" then any existing current selection will be cleared)
   void selectPoint(int id, bool replace = false);
   //
   // some basic info for the current point:
   //
   // the physical location of the point
   DPoint getLocation(int id);
   // getting the point for the physical location (returns -1 if it does not correspond to a node)
   // (note, it may not have attribute data available: also check isGraphNode)
   int getNode(DPoint& point);
   // is the node simply empty? (i.e., no graph information)
   bool isEmptyNode(int id);
   // does the node have VGA / attribute information available for it?
   bool isGraphNode(int id);
   // is the node on the edge of the graph
   bool isBoundaryNode(int id);
   // merge two points together (use id and -1 as parameters to unmerge points)
   void setMergePoint(int id, int merge_id);
   // which point (if any) is the node conecptually merged with (returns -1 if not merged)
   int getMergePoint(int id);
   // assign arbitrary data to the point
   void setUserData(int id, void *data);
   // retrieve arbitrary data from the point
   void *getUserData(int id);
   // the longest line of sight length for bin (0-31) (use -1 for longest line of sight in any direction)
   double getLoSLength(int id, int bin = -1, int type = 0); // type 0: standard LoS, type 1: longest occluded dist in this direction
   //
   // iterator for points connected to a node (set bin to -1 to get all points or bin in range 0-31)
   // note, these functions use internal cursors within Depthmap and not available to us
   int getConnectedPointCount(int id, int bin = -1);
   int getFirstConnectedPoint(int id, int bin = -1);
   int getNextConnectedPoint(int id, int bin = -1);
   //
   // straight forward grid connections -- returns direction of connections
   // bitwise or-ed together if there is an accessible grid location in that direction
   enum { CONNECT_E = 0x01, CONNECT_NE = 0x02, CONNECT_N = 0x04, CONNECT_NW = 0x08, 
          CONNECT_W = 0x10, CONNECT_SW = 0x20, CONNECT_S = 0x40, CONNECT_SE = 0x80 };
   char getGridConnections(int id);
   double getGridSpacing();
   //
   // Get the currently displayed attribute column
   const char *getDisplayedAttributeColumn();
   // Display the attribute column 
   // (note: do this last! -- if you do it half way through, you will confuse colour scheme)
   void setDisplayedAttributeColumn(const char *attribute);
   //
   // method should be one of IVGA_STEP, IVGA_METRIC or IVGA_ANGULAR
   void analysePointDepth(IComm *comm, int method);
   //
private:
   // INTERNAL TO DEPTHMAP
   // the data that is transferred to your app:
   // do not change this variable
   void *m_data;
public:
   // INTERNAL TO DEPTHMAP
   // Depthmap calls this function to give the interface the data
   // do not call this function
   void setData(void *data);
};

// this class forms your interface to Depthmap Axial, Segment, Drawing and Data functionality
// IMPORTANT: Note that not all functions work with all sorts of graph
class DepthmapClass IShapeMap
{
// your public interface:
public:
   IShapeMap();
   virtual ~IShapeMap();
protected:
   // SHAPE INTERFACE
   // this is the main iterator, it is used for all getShape... functions
   int m_cursor_shape;
   // secondary iterator for selected shape
   std::set<int>::iterator m_cursor_selected_shape;
public:
   // IDrawingMap is for drawing without attributes
   // IDataMap is for map with attributes
   // IAxialMap is designed for axial line maps only
   // ISegmentMap is designed for segment maps only
   // IConvexMap is designed for maps with polygons only and manually added connections
   // IShapeMap is designed for a graph with generic shapes intersecting
   enum { IDRAWINGMAP = 0x01, IDATAMAP = 0x02, ICONVEXMAP = 0x08, IAXIALMAP = 0x20, ISEGMENTMAP = 0x40, ISHAPEMAP = 0x80 };
   // some basic information about the map:
   const char *getMapName();
   // map type for some usually encountered types
   bool isAxialMap();
   bool isSegmentMap();
   bool isDataMap();
   bool isDrawingMap();
   // for further control, get the map type explicitly
   int getMapType();
   // is it currently displayed
   bool isVisible();
   //
   // if you know the id, you can move the cursor to this shape:
   void setCursorShape(int id)
   { m_cursor_shape = id; }
   //
   // iterator for all shapes (uses m_cursor_shape)
   int getShapeCount();
   int getFirstShape();
   int getNextShape();
   //
   // iterator for selected shapes in the Depthmap UI (uses m_cursor_selected_shape)
   int getSelectedShapeCount();
   int getFirstSelectedShape();
   int getNextSelectedShape();
   //
   // clear selection: straight forward I hope
   void clearSelection();
   // must know the id to get this (if replace is "true" then any existing current selection will be cleared)
   void selectShape(int id, bool replace = false);
   // I'm not sure how useful this function will be, but you might want to 
   // find shapes within a radius of a point:
   int selectShapesWithinRadius(DPoint& point, double dist);
   // selection to layer converts the current selection to a layer within your map:
   void makeLayerFromSelection(const char *layer_name);
   //
   // iterator for connections -- note that in segment maps you 
   // will have 'forward' and 'back' connections at each end of the line
   // a data map can still call these functions, but will not return any results
   // note, these functions use internal cursors within Depthmap and not available to us
   enum { CONN_ALL, CONN_FW, CONN_BK };
   int getConnectedShapeCount(int id, int dir = CONN_ALL);
   int getFirstConnectedShape(int id, int dir = CONN_ALL);
   int getNextConnectedShape(int id,  int dir = CONN_ALL);
   // note that getConnectionWeight and getConnectionDirection get the connection from the
   // current position of the connected line cursor,
   int getConnectionDirection(int id, int dir = CONN_ALL);
   double getConnectionWeight(int id, int dir = CONN_ALL);
   //
   // used to determine type of shape:
   bool isPointShape(int id);
   bool isLineShape(int id);
   bool isPolyLineShape(int id);
   bool isPolygonShape(int id);
   // the physical point coordinates
   // (note, you should check it's a point first, otherwise this will return the centroid of the shape
   DPoint getPointCoords(int id);
   // the physical line coordinates
   // (note, you should check it's a line first, otherwise this will return the bounding box of the shape
   DLine getLineCoords(int id);
   // for polylines and polygons, use a pair of functions:
   int getVertexCount(int id);
   DPoint getVertex(int id, int v);
   //
   // getting the shape id from a physical location (returns -1 if it does not correspond to a shape)
   // note: uses a selection algorithm to find a shape close to the point
   int getShape(DPoint& point);
   // Get the currently displayed attribute
   const char *getDisplayedAttributeColumn();
   // Display the attribute column 
   // (note: do this last! -- if you do it half way through, you will confuse colour scheme)
   void setDisplayedAttributeColumn(const char *attribute);
   //
   //////////////////////////////////////////////////////////////////////////////
   // EDITING THE MAP (from Dmap version 8)
   // Before you start you may want to clear everything: WARNING!, this deletes absolutely everything in your map!
   void clearMap();
   // Note that you *MUST* call commitShapes() after adding shapes
   // 1) Adding / removing shapes
   // Adding a shape works very much like OpenGL:
   // Start with beginShape then call vertex for each vertex, before calling endShape
   // endShape returns the id of the new shape.
   // The shapes do not have to be lines!
   // Note that links are not made automatically for overlapping geometry:
   // you must use connectDirected, connectUndirected, or connectIntersected
   void beginShape();
   void addVertex(DPoint& point);
   // set the 'open' parameter to true if you want to create a point, line or polyline, otherwise, for a closed polygon, set to false
   int endShape(bool open);
   // after adding geometry, you must commitShapes --
   // you can finish adding all the geometry you want (e.g., load a file of shapes) before calling commitShapes, 
   // but intersection testing and screen draw rely on commitShapes being called
   void commitShapes();
   // removeShape straight forward (also removes all links associated with this shape), returns true if shape is removed successfully
   // (note, no need to call commit)
   bool removeShape(int id);
   //
   // For axial / segment (and other types of graph), you can add connections:
   // 2) Adding / removing connections
   // Use connectUndirected to add a single link with all graph types except segment
   // -- returns true if connection is made or weight modified, note: uses links / unlinks table
   bool connectUndirected(int id1, int id2);
   // Use connectDirected with segment graphs *only*
   // Note that despite the "fromdir" and "todir" (CONN_FW or CONN_BK) this only ever connects from the "from" node to the "to" node
   // e.g.., a connection back from the from node to the to node would use CONN_BK for direction
   bool connectDirected(int from, int fromdir, int to, int todir, double conn_weight);
   // connectIntersected makes undirected (two way) connections from the shape 
   // to all existing shapes it intersects or touches: returns the number of connections made
   int connectIntersected(int id);
   // makeAllConnections *clears* everything (including all existing attributes, links, unlinks) before connecting intersecting shapes:
   void makeAllConnections();
   // disconnect removes connections between the shapes if the connections exist:
   // returns true if the shapes were previously connected, false if they were not previously connected
   bool disconnect(int id1, int id2);
   //
public:
   // This calls the standard Depthmap analyseSegments (technically, Tulip 1024 analysis) 
   // if interactive is false, then analyseSegments will only analyse the current selection (if a selection has been made)
   // if weighting_column is null, then segment length will be used for the weighting
   // returns the number of segments analysed
   int analyseSegments(IComm *comm, size_t radius_count, float *radius_array, const char *weighting_column, const char *weighting_column2, const char *routeweight_column, bool interactive);
   // Export the map.  Currently only supports MapInfo MIF/MID combination.  Type MUST be "MIF" (in capitals)
   bool exportMap(const char *filename, const char *type);
   // Load unlinks for an axial map relies on you having an IShapeMap with a set of unlink points in it
   bool loadUnlinks(IShapeMap *unlink_points_map);
   //
   //////////////////////////////////////////////////////////////////////////////////////
private:
   // INTERNAL TO DEPTHMAP
   // the data that is transferred to your app:
   // do not change this variable
   void *m_data;
public:
   // INTERNAL TO DEPTHMAP
   // Depthmap calls this function to give the interface the data
   // do not call this function
   void setData(void *data);
};

// IAttributes
// this class forms your interface to attributes, it can be
// used with any analysis, just use the id code of the 
// relevant data type (point, line or segment) to access the 
// attribute for it
class DepthmapClass IAttributes
{
protected:
   int m_cursor_column;
   int m_cursor_row;
public:
   IAttributes();
   ~IAttributes();
   // Get a list of attributes
   int getAttributeColumnCount();
   const char *getFirstAttributeColumn();
   const char *getNextAttributeColumn();
   // Get the row entries from the attributes
   int getAttributeRowCount();
   int getFirstAttributeRow();
   int getNextAttributeRow();
   // Either insert an attribute, or, if the column exists, clear it: (you cannot clear the Ref Number column or a locked column)
   bool insertAttributeColumn(const char *attribute);
   // Delete an attribute column (you cannot delete the Ref Number column or a locked column)
   bool deleteAttributeColumn(const char *attribute);
   // Rename an attribute column (you cannot rename the Ref Number column or a locked column)
   bool renameAttributeColumn(const char *oldname, const char *newname);
   // Check that an attribute is valid
   bool isValidAttributeColumn(const char *attribute);
   // Check to see if the attribute is locked (e.g., Connectivity on an axial map)
   bool isLockedAttributeColumn(const char *attribute);
   // get an attribute from the attribute table for a point or line:
   float getAttribute(int row, const char *attribute);
   // set an attribute in the attribute table for a point or line:
   void setAttribute(int row, const char *attribute, float value);
   // increment an attribute in the attribute table for a point or line:
   void incrAttribute(int row, const char *attribute);
   // helpers: import and export of tables (as tab delimited text files)
   // import: merge adds columns together
   bool importTable(const char *filename, bool merge);
   // note, columns are flagged internally in Depthmap after internal processes
   bool exportTable(const char *filename, bool updated_only);
private:
   // INTERNAL TO DEPTHMAP
   // the data that is transferred to your app:
   // do not change this variable
   void *m_data;
   // new for 708: attribute table records the analysis type
   // this is required because the sort of id used differs when using PointMaps (a "pixel" id) to all other maps
   int m_analysis_type;
public:
   // INTERNAL TO DEPTHMAP
   // Depthmap calls this function to give the interface the data
   // do not call this function
   void setData(void *data, int analysis_type);
};

// From DMap version 8 you can also access the graph file directly
// This should free you up to write inter-map analyses

class DepthmapClass IGraphFile
{
public:
   IGraphFile();
   virtual ~IGraphFile();
public:
   // VGA straight forward (note: no insert as yet)
   int getVGAMapCount();
   IVGAMap *getFirstVGAMap();
   IVGAMap *getNextVGAMap();
   // Shape maps: used for segment, axial, data and drawing maps
   int getShapeMapCount();
   IShapeMap *getFirstShapeMap();
   IShapeMap *getNextShapeMap();
   IShapeMap *insertShapeMap(const char *name, int type); // for type, see IShapeMap types above
public:
   // retrieve the attribute table for either vga or shape layer interfaces:
   IAttributes *getAttributes(IVGAMap *ivga);
   IAttributes *getAttributes(IShapeMap *ishape);
   // get the view order: 0 is topmost, then 1, and so on, to bottom most. -1 is not displayed
   int getViewOrder(IVGAMap *ivga);
   int getViewOrder(IShapeMap *ishape);
   //
   // Push values between maps (values are transferred between overlapping geometry)
   // Push type suggests what to do if more than one object is overlapping
   enum { IPUSH_MAX, IPUSH_MIN, IPUSH_AVG, IPUSH_TOT };
   // set dest_attribute to NULL to get an automatically assigned output column name
   bool pushValuesToMap(IShapeMap *source, IShapeMap *dest, const char *source_attribute, const char *dest_attribute, int push_type);
   bool pushValuesToMap(IVGAMap *source, IShapeMap *dest, const char *source_attribute, const char *dest_attribute, int push_type);
   bool pushValuesToMap(IShapeMap *source, IVGAMap *dest, const char *source_attribute, const char *dest_attribute, int push_type);
public:
   // These allow extra control for interface to standard Depthmap functions
   // Please use with caution when writing your own interfaces -- these are intended for sala.dll redistributables rather than Depthmap processing
   //
   // Import a map from a CAT, DXF or MIF / MID combination
   // Note: type must be in capitals: e.g., "MIF" for MIF / MID
   // Currently, CAT and DXF return NULL regardless of success or failure
   // (as they create drawing maps which are currently unsupported through the IShapeMap interface
   IShapeMap *importMap(const char *filename, const char *type, const char *newmapname);
   //
   // make an axial map from a data map or a drawing map
   IShapeMap *makeAxialMapFromBaseMap(IComm *comm, IShapeMap *basemap, const char *newmapname);
   // make a segment map from an axial map (cuts segments at axial-axial intersections)
   IShapeMap *makeSegmentMapFromAxialMap(IComm *comm, IShapeMap *axialmap, const char *newmapname, double stubremoval); // n.b. converts an axial map
   //
   bool save(const char *filename);
   //
   friend DepthmapClass IGraphFile *newGraphFile();
   friend DepthmapClass IGraphFile *openGraphFile(const char *filename);
   //
private:
   // INTERNAL TO DEPTHMAP
   // the data that is transferred to your app:
   // do not change this variable
   void *m_data;
public:
   // INTERNAL TO DEPTHMAP
   // Depthmap calls this function to give the interface the data
   // do not call this function
   void setData(void *data);
};

// You can even open your own graph files or create new ones
// use with caution in your own interfaces
DepthmapClass IGraphFile *newGraphFile();
DepthmapClass IGraphFile *openGraphFile(const char *filename);
DepthmapClass void closeGraphFile(IGraphFile *& file); // use this to close and tidy up a graph file created with "new" or "open"

// functions which Depthmap will load

enum {DLL_NO_ANALYSIS = 0x01, 
      DLL_VGA_ANALYSIS = 0x02, 
      DLL_AXIAL_ANALYSIS = 0x04, 
      DLL_SEGMENT_ANALYSIS = 0x08, 
      DLL_DATA_ANALYSIS = 0x10, 
      DLL_ATTRIBUTE_ANALYSIS = 0x20,
      DLL_ANALYSIS_TYPE = 0x3f, // note this deliberately doesn't include DLL_GRAPH_FILE_ANALYSIS
      DLL_GRAPH_FILE_ANALYSIS = 0x40 };

// Depthmap will load your functions with these function specs:
typedef char *(*FUNC_GETMODULENAME)();
typedef int  (*FUNC_GETANALYSISTYPE)(int analysis);
typedef char *(*FUNC_GETANALYSISNAME)(int analysis);
typedef bool (*FUNC_PREPROCESS)(int analysis);
typedef bool (*FUNC_PROCESSVGA)(int analysis, IComm *, IVGAMap *, IAttributes *);
typedef bool (*FUNC_PROCESSSHAPE)(int analysis, IComm *, IShapeMap *, IAttributes *);
typedef bool (*FUNC_PROCESSATTRIBUTES)(int analysis, IComm *, IAttributes *);
typedef bool (*FUNC_PROCESSGRAPHFILE)(int analysis, IComm *, IGraphFile *);
typedef void (*FUNC_POSTPROCESS)(int analysis);
typedef float (*FUNC_GETMODULEVERSION)();

#ifndef _DEPTHMAP 
   // getModuleName
   // use this function to return the name of your module -- you MUST write this function
   // (which will be added to the tools menu when you import the analysis / Depthmap starts up)
   extern "C" {DllProc char *getModuleName();}
   // getAnalysisType
   // use this function to return what type of analysis your module performs:
   // it must be one of DLL_NO_ANALYSIS, DLL_VGA_ANALYSIS, DLL_AXIAL_ANALYSIS, DLL_SEGMENT_ANALYSIS or DLL_DATA_ANALYSIS
   extern "C" {DllProc int getAnalysisType(int analysis);}
   // getAnalysisName
   // use this function to name up to 8 different analyses within a module
   // you MUST supply at least one analysis name
   // Depthmap will call with i = 0,1,2,3... etc.  
   // Return NULL when there are no more analyses
   extern "C" {DllProc char *getAnalysisName(int analysis);}
   // preprocess
   // in preprocess, you might open a dialog box and allow the user to enter options
   // parameter "analysis" corresponds to analyses named by getAnalysisName, 
   // if there are no named analyses, the parameter is set to -1
   // return true for Depthmap to run process, or false to stop without processing
   extern "C" {DllProc bool preprocess(int analysis);}
   // process (processVGA and processShape)
   // in process, you do the analysis your module performs, this will be run in a 
   // separate thread, when finished, Depthmap will redraw and update attribute tables
   // parameters analysis corresponds to analyses named by getAnalysisName, 
   // if there are no named analyses, this is set to -1
   // IComm * allows you to tell the front end how far you are through your analysis
   // please also check the isCancelled() function frequently to see if the user wants to stop analysis
   // processVGA
   // IVGAMap * gives you access to the graph itself -- see above for details
   // return true for success, false for failure
   extern "C" {DllProc bool processVGA(int analysis, IComm *, IVGAMap *, IAttributes *);}
   // processShape
   // (note process shape is used for *all of* segment analysis, axial analysis and data analysis)
   // IShapeMap * gives you access to the graph itself -- see above for details
   // return true for success, false for failure
   extern "C" {DllProc bool processShape(int analysis, IComm *, IShapeMap *, IAttributes *);}
   // processAttributes
   // for certain functions you may not care what sort of graph is shown, you may simply
   // want to change the attributes: use this function to do so
   extern "C" {DllProc bool processAttributes(int analysis, IComm *, IAttributes *);}
   // processGraphFile
   // for certain functions you may need access to more than one layer
   // this function allows you access to the top level graph file
   extern "C" {DllProc bool processGraphFile(int analysis, IComm *, IGraphFile *);}
   // postprocess
   // in postprocess, you might want to open a dialog box to give the user a summary of
   // what happened during the analysis
   // parameter analysis corresponds to analyses named by getAnalysisName, 
   // if there are no named analyses, this is set to -1
   extern "C" {DllProc void postprocess(int analysis);}
   //
   // OTHER INTERNAL METHODS
   // do not alter this function:
   extern "C" {DllProc float getModuleVersion(); }
   // this appears to work, even though it's expecting an extern function:
   extern "C" float getModuleVersion();
#endif

inline float getModuleVersion() 
{ return (float)DLL_DEPTHMAP_VERSION; }


#endif
