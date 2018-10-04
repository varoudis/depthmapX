#pragma once

#include "salalib/spacepix.h"
#include "salalib/axialmap.h"
#include "salalib/connector.h"
#include "genlib/p2dpoly.h"

struct AxialVertexKey
{
   int m_ref_key;
   short m_ref_a;
   short m_ref_b;
   AxialVertexKey(int ref = -1, short a = -1, short b = -1)
   { m_ref_key = ref; m_ref_a = a; m_ref_b = b; }
   friend bool operator == (const AxialVertexKey& a, const AxialVertexKey& b);
   friend bool operator != (const AxialVertexKey& a, const AxialVertexKey& b);
   friend bool operator > (const AxialVertexKey& a, const AxialVertexKey& b);
   friend bool operator < (const AxialVertexKey& a, const AxialVertexKey& b);
};
inline bool operator == (const AxialVertexKey& a, const AxialVertexKey& b)
{ return (a.m_ref_key == b.m_ref_key && a.m_ref_a == b.m_ref_a && a.m_ref_b == b.m_ref_b); }
inline bool operator != (const AxialVertexKey& a, const AxialVertexKey& b)
{ return (a.m_ref_key != b.m_ref_key || a.m_ref_a != b.m_ref_a || a.m_ref_b != b.m_ref_b); }
inline bool operator > (const AxialVertexKey& a, const AxialVertexKey& b)
{ return (a.m_ref_key > b.m_ref_key || (a.m_ref_key == b.m_ref_key && (a.m_ref_a > b.m_ref_a || (a.m_ref_a == b.m_ref_a && a.m_ref_b > b.m_ref_b)))); }
inline bool operator < (const AxialVertexKey& a, const AxialVertexKey& b)
{ return (a.m_ref_key < b.m_ref_key || (a.m_ref_key == b.m_ref_key && (a.m_ref_a < b.m_ref_a || (a.m_ref_a == b.m_ref_a && a.m_ref_b < b.m_ref_b)))); }

const AxialVertexKey NoVertex(-1,-1,-1);

struct AxialVertex : public AxialVertexKey
{
   Point2f m_point;
   Point2f m_openspace;
   Point2f m_a;
   Point2f m_b;
   bool m_clockwise;
   bool m_convex;
   bool m_initialised;
   bool m_axial;
   AxialVertex(const AxialVertexKey& vertex_key = NoVertex, const Point2f& point = Point2f(), const Point2f& openspace = Point2f()) : AxialVertexKey(vertex_key)
   { m_point = point; m_openspace = openspace; m_initialised = false; m_axial = false; }
};

struct RadialKey {
   AxialVertexKey vertex;
   float ang;
   bool segend;
   // padding the remaining three bytes behind the bool - don't use int : 24 as this will grab the next 4 byte block
   char pad1 : 8;
   short pad2 : 16;

   RadialKey(const AxialVertexKey& v = NoVertex, float a = -1.0f, bool se = false) : pad1(0), pad2(0)
   { vertex = v; ang = a; segend = se; }
   RadialKey(const RadialKey& rk) : pad1(0), pad2(0)
   { vertex = rk.vertex; ang = rk.ang; segend = rk.segend; }
};
inline bool operator < (const RadialKey& a, const RadialKey& b)
{ return a.vertex < b.vertex || (a.vertex == b.vertex && (a.ang < b.ang || (a.ang == b.ang && a.segend < b.segend))); }
inline bool operator > (const RadialKey& a, const RadialKey& b)
{ return a.vertex > b.vertex || (a.vertex == b.vertex && (a.ang > b.ang || (a.ang == b.ang && a.segend > b.segend))); }
inline bool operator == (const RadialKey& a, const RadialKey& b)
{ return a.vertex == b.vertex && a.ang == b.ang && a.segend == b.segend; }

struct RadialLine : public RadialKey
{
   Point2f openspace;
   Point2f keyvertex;
   Point2f nextvertex;
   RadialLine(const RadialKey& rk = RadialKey()) : RadialKey(rk) {;}
   RadialLine(const AxialVertexKey& v, bool se, const Point2f& o, const Point2f& k, const Point2f& n)
   { vertex = v; ang = (float) angle(o,k,n); segend = se; openspace = o; keyvertex = k; nextvertex = n; }
   RadialLine(const RadialLine& rl) : RadialKey(rl)
   { openspace = rl.openspace; keyvertex = rl.keyvertex; nextvertex = rl.nextvertex; }
   bool cuts(const Line& l) const;
};

typedef pqvector<RadialKey> RadialKeyList;

struct RadialSegment : public pvecint
{
   RadialKey radial_b;

   // Quick mod - TV
#if defined(_WIN32)
   RadialSegment(RadialKey& rb = RadialKey()) : pvecint()
   { radial_b = rb; }
#else
   RadialSegment(RadialKey& rb /*= RadialKey()*/) : pvecint()
   { radial_b = rb; }
#endif

   // Quick mod - TV
   RadialSegment() : pvecint()
   {
       radial_b = RadialKey();
   }

};

struct PolyConnector {
   Line line;
   RadialKey key;
   PolyConnector(const Line& l = Line(), const RadialKey& k = RadialKey())
   { line = l; key = k; }
};


class AxialPolygons : public SpacePixel
{
   friend class ShapeGraphs;
protected:
   pvecint m_vertex_polys;
   pvecint **m_pixel_polys;
public:
   pqvector<AxialVertex> m_handled_list;
   std::map<Point2f,pqvector<Point2f>> m_vertex_possibles;

   AxialPolygons();
   virtual ~AxialPolygons();
   //
   void clear();
   void init(std::vector<Line> &lines, const QtRegion& region);
   void makeVertexPossibles(const std::vector<Line> &lines, const prefvec<Connector>& connectionset);
   void makePixelPolys();
   //
   AxialVertex makeVertex(const AxialVertexKey& vertexkey, const Point2f& openspace);
   // find a polygon corner visible from seed:
   AxialVertexKey seedVertex(const Point2f& seed);
   // make axial lines from corner vertices, visible from openspace
   void makeAxialLines(pqvector<AxialVertex>& openvertices, prefvec<Line>& lines, KeyVertices &keyvertices, prefvec<PolyConnector>& poly_connections, pqvector<RadialLine>& radial_lines);
   // extra: make all the polygons possible from the set of m_vertex_possibles
   void makePolygons(std::vector<std::vector<Point2f> > &polygons);
};
