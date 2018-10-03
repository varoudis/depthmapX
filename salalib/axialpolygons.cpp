#include "salalib/axialpolygons.h"
#include "salalib/axialmap.h"
#include "salalib/tidylines.h"
#include "salalib/tolerances.h"
#include "genlib/containerutils.h"

AxialPolygons::AxialPolygons()
{
   m_pixel_polys = NULL;
}

AxialPolygons::~AxialPolygons()
{
   if (m_pixel_polys) {
      for (int i = 0; i < m_cols; i++) {
         delete [] m_pixel_polys[i];
      }
      delete [] m_pixel_polys;
      m_pixel_polys = NULL;
   }
}

AxialVertex AxialPolygons::makeVertex(const AxialVertexKey& vertexkey, const Point2f& openspace)
{
   auto vertPossIter = depthmapX::getMapAtIndex(m_vertex_possibles, vertexkey.m_ref_key);
   AxialVertex av(vertexkey, vertPossIter->first, openspace);

   // n.b., at this point, vertex key m_a and m_b are unfixed
   pqvector<Point2f>& pointlist = vertPossIter->second;
   if (pointlist.size() < 2) {
      return av;
   }

   Point2f o = av.m_point - av.m_openspace;

   // using an anglemap means that there are now no anti-clockwise vertices...
   // TODO: (CS) Double as key is problematic - books have been written about double equality...
   std::map<double,int> anglemap;
   for (size_t i = 0; i < pointlist.size(); i++) {
      anglemap.insert(std::make_pair( angle(openspace,av.m_point,pointlist[i]), i ));
   }

   av.m_ref_a = anglemap.begin()->second;
   // TODO: is this supposed to be av.m_ref_b?
   av.m_ref_a = anglemap.rbegin()->second;
   Point2f a = av.m_point - pointlist.at( anglemap.begin()->second );
   Point2f b = pointlist.at( anglemap.rbegin()->second ) - av.m_point;
   av.m_a = a;
   av.m_b = b;
   a.normalise();
   b.normalise();

   double oa = det(o,a);
   double ob = det(o,b);
   double ab = det(a,b);

   // can't handle these cases
   if (fabs(oa) < TOLERANCE_A || fabs(ob) < TOLERANCE_A || fabs(ab) < TOLERANCE_A) {
      // although note that if ab == 0 and you've already checked intersection, it can't be convex
      return av;
   }

   // ADDED 4-Nov-04 -- In order to stop too many lines being generated, don't include
   // points that do not change surface direction:  -- notice: will create problems with circles
   if (fabs(dot(a,b)) > 0.999) {
      return av;
   }


   if (sgn(oa) == sgn(ob)) {
      // headon collision
      if (sgn(oa) == 1) {
         if (sgn(ab) == 1) {
            // convex clockwise
            av.m_convex = true;
            av.m_clockwise = true;
            av.m_axial = true;
         }
         else {
            // n.b., these are turned away for axial formation
            // concave clockwise
            av.m_convex = false;
            av.m_clockwise = true;
            av.m_axial = false;
         }
      }
   }
   else {
      // glancing blow
      // concave clockwise
      av.m_convex = false;
      av.m_clockwise = true;
      av.m_axial = true;
   }

   av.m_initialised = true;

   return av;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void AxialPolygons::clear()
{
   // clear any existing data
   if (m_pixel_polys) {
      for (int i = 0; i < m_cols; i++) {
         delete [] m_pixel_polys[i];
      }
      delete [] m_pixel_polys;
      m_pixel_polys = NULL;
   }

   m_vertex_possibles.clear();
   m_vertex_polys.clear();
   m_handled_list.clear();
}

void AxialPolygons::init(std::vector<Line>& lines, const QtRegion& region)
{
   // init pixelbase members
   m_region = region;

   // now tidy
   TidyLines tidier;
   tidier.tidy(lines, m_region);

   // for easier debugging, the axial code is reused to make segments
   ShapeGraph firstpass;
   firstpass.init(lines.size(),m_region); // used to be double density
   firstpass.initialiseAttributesAxial();
   size_t i;
   for (i = 0; i < lines.size(); i++) {
      firstpass.makeLineShape(lines[i]);
   }
   firstpass.makeConnections();

   lines.clear();
   prefvec<Connector> connectionset;

   // interesting... 1.0 may or may not work as intended
   firstpass.makeSegmentMap(lines, connectionset, 1.0);

   // now we have a set of lines and a set of connections...
   // ...for the second pass, a bit of retro fitting to my original code is
   // required
   makeVertexPossibles(lines, connectionset);

   initLines(lines.size(), m_region.bottom_left, m_region.top_right, 2);
   // need to init before making pixel polys...
   makePixelPolys();
   // now also add lines
   for (auto vertexPoss: m_vertex_possibles) {
      for (size_t j = 0; j < vertexPoss.second.size(); j++) {
         addLine(Line(vertexPoss.first,vertexPoss.second.at(j)));
      }
   }
   sortPixelLines();
}

void AxialPolygons::makeVertexPossibles(const std::vector<Line>& lines, const prefvec<Connector>& connectionset)
{
   m_vertex_possibles.clear();
   m_vertex_polys.clear();
   int currpoly = -1;

   size_t i = 0;

   // TODO: (CS) these should be vectors, not raw pointers.
   int *found[2];
   found[0] = new int [lines.size()];
   found[1] = new int [lines.size()];
   for (i = 0; i < lines.size(); i++) {
      found[0][i] = -1;
      found[1][i] = -1;
   }
   pqvector<Point2f> pointlookup;
   // three pass operation: (1) stack the lines
   for (i = 0; i < lines.size(); i++) {
      if (found[0][i] == -1) {
         pointlookup.push_back(lines[i].start());
         m_vertex_possibles.insert(std::make_pair(pointlookup.tail(),pqvector<Point2f>()));
         m_vertex_polys.push_back(-1); // <- n.b., dummy entry for now, maintain with vertex possibles
         found[0][i] = pointlookup.size() - 1;
         for (size_t j = 0; j < connectionset[i].m_back_segconns.size(); j++) {
            const SegmentRef& segref = connectionset[i].m_back_segconns.key(j);
            int forwback = (segref.dir == 1) ? 0 : 1;
            found[forwback][segref.ref] = found[0][i];
         }
      }
      if (found[1][i] == -1) {
         pointlookup.push_back(lines[i].end());
         m_vertex_possibles.insert(std::make_pair(pointlookup.tail(),pqvector<Point2f>()));
         m_vertex_polys.push_back(-1); // <- n.b., dummy entry for now, maintain with vertex possibles
         found[1][i] = pointlookup.size() - 1;
         for (size_t j = 0; j < connectionset[i].m_forward_segconns.size(); j++) {
            const SegmentRef& segref = connectionset[i].m_forward_segconns.key(j);
            int forwback = (segref.dir == 1) ? 0 : 1;
            found[forwback][segref.ref] = found[1][i];
         }
      }
   }
   // three pass operation: (2) connect up vertex possibles
   for (i = 0; i < lines.size(); i++) {
      if (found[0][i] == -1 || found[1][i] == -1) {
         // TODO: (CS) What are these integers being thrown?!
         throw 1;
      }
      auto index0 = m_vertex_possibles.find(pointlookup.at(found[0][i]));
      auto index1 = m_vertex_possibles.find(pointlookup.at(found[1][i]));
      if (index0 == m_vertex_possibles.end() || index1 == m_vertex_possibles.end()) {
         // TODO: (CS) What are these integers being thrown?!
         throw 2;
      }
      index0->second.add(pointlookup.at(found[1][i]));
      index1->second.add(pointlookup.at(found[0][i]));
   }
   delete [] found[0];
   delete [] found[1];
   // three pass operation: (3) create vertex poly entries
   int current_poly = -1;
   for (i = 0; i < m_vertex_possibles.size(); i++) {
      if (m_vertex_polys[i] == -1) {
         current_poly++;
         pvecint addlist;
         addlist.push_back(i);
         while (addlist.size()) {
            m_vertex_polys[addlist.tail()] = current_poly;
            pqvector<Point2f>& connections = depthmapX::getMapAtIndex(m_vertex_possibles, addlist.tail())->second;
            addlist.pop_back();
            for (size_t j = 0; j < connections.size(); j++) {
               int index = depthmapX::findIndexFromKey(m_vertex_possibles, connections[j]);
               if (index == -1) {
                  throw 3;
               }
               if (m_vertex_polys[index] == -1) {
                  addlist.push_back(index);
               }
            }
         }
      }
   }
}

void AxialPolygons::makePixelPolys()
{
   int i = 0;

   // record all of this onto the pixel polygons
   if (m_pixel_polys)
   {
      for (i = 0; i < m_cols; i++) {
         delete [] m_pixel_polys[i];
      }
      delete [] m_pixel_polys;
      m_pixel_polys = NULL;
   }
   m_pixel_polys = new pvecint *[m_cols];
   for (i = 0; i < m_cols; i++) {
      m_pixel_polys[i] = new pvecint[m_rows];
   }
   // now register the vertices in each pixel...
   int j = -1;
   for (auto vertPoss: m_vertex_possibles) {
      j++;
      PixelRef pix = pixelate(vertPoss.first);
      m_pixel_polys[pix.x][pix.y].push_back(j);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////

// almost identical to original!

AxialVertexKey AxialPolygons::seedVertex(const Point2f& seed)
{
   AxialVertexKey seedvertex = NoVertex;
   PixelRef seedref = pixelate(seed);
   bool foundvertex = false;
   // for spiralling outwards to find a vertex:
   int dir = PixelRef::HORIZONTAL;
   int sidelength = 1;
   int runlength = 0;
   int allboundaries = 0;

   while (!foundvertex) {
      for (size_t i = 0; i < m_pixel_polys[seedref.x][seedref.y].size(); i++) {
         int vertexref = m_pixel_polys[seedref.x][seedref.y][i];
         const Point2f& trialpoint = depthmapX::getMapAtIndex(m_vertex_possibles, vertexref)->first;
         if (!intersect_exclude(Line(seed,trialpoint))) {
            // yay... ...but wait... we need to see if it's a proper polygon vertex first...
            seedvertex = vertexref;
            foundvertex = true;
         }
      }
      if (!foundvertex) {
         seedref = seedref.move(dir);
         // spiral outwards:
         if (++runlength == sidelength) {
            switch (dir) {
            case PixelRef::HORIZONTAL:
               dir = PixelRef::VERTICAL; runlength = 0;
               break;
            case PixelRef::VERTICAL:
               dir = PixelRef::NEGHORIZONTAL; runlength = 0; sidelength++;
               break;
            case PixelRef::NEGHORIZONTAL:
               dir = PixelRef::NEGVERTICAL; runlength = 0;
               break;
            case PixelRef::NEGVERTICAL:
               dir = PixelRef::HORIZONTAL; runlength = 0; sidelength++;
               break;
            }
         }
         // check to make sure not off edge of system:
         if (seedref.x < 0) {
            allboundaries |= 0x01; seedref.x = 0;
         }
         if (seedref.y < 0) {
            allboundaries |= 0x02; seedref.y = 0;
         }
         if (seedref.x >= m_cols) {
            allboundaries |= 0x04; seedref.x = m_cols - 1;
         }
         if (seedref.y >= m_rows) {
            allboundaries |= 0x08; seedref.y = m_rows - 1;
         }
         if (allboundaries == 0x0f) {
            return NoVertex;
         }
      }
   }
   return seedvertex;
}

// adds any axial lines from this point to the list of lines, adds any unhandled visible vertices it finds to the openvertices list
// axial lines themselves are added to the lines list - the axial line is only there to record the key vertices that comprise the line
void AxialPolygons::makeAxialLines(pqvector<AxialVertex>& openvertices, prefvec<Line>& lines, prefvec<pvecint>& keyvertices, prefvec<PolyConnector>& poly_connections, pqvector<RadialLine>& radial_lines)
{
   AxialVertex vertex = openvertices.tail();
   openvertices.pop_back();

   m_handled_list.add(vertex);

   int i = -1;
   for (auto vertPoss: m_vertex_possibles) {
      i++;
      if (i == vertex.m_ref_key) {
         continue;
      }
      bool possible = false, stubpossible = false;
      Point2f p = vertPoss.first - vertex.m_point;
      if (vertex.m_convex) {
         if (det(vertex.m_a,p) > 0 && det(vertex.m_b,p) > 0) {
            possible = true;
         }
      }
      else {
         // left of b and right of a or left of a and right of b
         if (det(p,vertex.m_a) * det(p,vertex.m_b) < 0) {
            possible = true;
         }
         else if (det(p,vertex.m_a) < TOLERANCE_A && det(p,vertex.m_b) < TOLERANCE_A) {
            stubpossible = true;
         }
      }
      if (possible || stubpossible) {
         Line line(vertPoss.first,vertex.m_point);
         if (!intersect_exclude(line)) {
            AxialVertex next_vertex = makeVertex(AxialVertexKey(i),vertex.m_point);
            if (next_vertex.m_initialised && m_handled_list.searchindex(next_vertex) == paftl::npos) {
               openvertices.add(next_vertex); // <- note, add ignores duplicate adds (each vertex tends to be added multiple times before this vertex is handled itself)
               bool shortline_segend = false;
               Line shortline = line;
               if (!vertex.m_convex && possible) {
                  Line ext(line.t_end(), line.t_end() + (line.t_end() - line.t_start()));
                  ext.ray(1, m_region);
                  cutLine(ext, 1);
                  line = Line(line.t_start(), ext.t_end());
                  // for radial line segend calc:
                  if (det(-p,vertex.m_b) < 0) {
                     shortline_segend = true;
                  }
               }
               if (m_vertex_polys[vertex.m_ref_key] != m_vertex_polys[next_vertex.m_ref_key]) { // must be on separate polygons
                  // radial line(s) (for new point)
                  RadialLine radialshort(next_vertex, shortline_segend, vertex.m_point,next_vertex.m_point,next_vertex.m_point+next_vertex.m_b);
                  poly_connections.push_back( PolyConnector(shortline, (RadialKey)radialshort) );
                  radial_lines.add(radialshort);
                  if (!vertex.m_convex && possible) {
                     Line longline = Line(vertPoss.first,line.t_end());
                     RadialLine radiallong(radialshort);
                     radiallong.segend = shortline_segend ? 0 : 1;
                     poly_connections.push_back( PolyConnector(longline, (RadialKey)radiallong) );
                     radial_lines.add(radiallong);
                  }
               }
               shortline_segend = false;
               if (!next_vertex.m_convex && next_vertex.m_axial) {
                  Line ext(line.t_start() - (line.t_end() - line.t_start()), line.t_start());
                  ext.ray(0, m_region);
                  cutLine(ext, 0);
                  line = Line(ext.t_start(), line.t_end());
                  // for radial line segend calc:
                  if (det(p,next_vertex.m_b) < 0) {
                     shortline_segend = true;
                  }
               }
               if (m_vertex_polys[vertex.m_ref_key] != m_vertex_polys[next_vertex.m_ref_key]) { // must be on separate polygons
                  // radial line(s) (for original point)
                  RadialLine radialshort(vertex, shortline_segend, next_vertex.m_point,vertex.m_point,vertex.m_point+vertex.m_b);
                  poly_connections.push_back( PolyConnector(shortline, (RadialKey)radialshort) );
                  radial_lines.add(radialshort);
                  if (!next_vertex.m_convex && next_vertex.m_axial) {
                     Line longline = Line(line.t_start(),vertex.m_point);
                     RadialLine radiallong(radialshort);
                     radiallong.segend = shortline_segend ? 0 : 1;
                     poly_connections.push_back( PolyConnector(longline, (RadialKey)radiallong) );
                     radial_lines.add(radiallong);
                  }
               }
               if (possible && next_vertex.m_axial) {
                  // axial line
                  lines.push_back(line);
                  keyvertices.push_back(pvecint());
                  if (vertex.m_convex) {
                     keyvertices.tail().add(vertex.m_ref_key);
                  }
                  if (next_vertex.m_convex) {
                     keyvertices.tail().add(next_vertex.m_ref_key);
                  }
               }
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////////

// not really used as yet, a feature to make all the polygons from the vertex
// possibles list

void AxialPolygons::makePolygons(std::vector<std::vector<Point2f>>& polygons)
{
   prefvec<pvecint> handled_list;
   for (size_t j = 0; j < m_vertex_possibles.size(); j++) {
      handled_list.push_back(pvecint());
   }

   int i = -1;
   for (auto vertPoss: m_vertex_possibles) {
      i++;
      if (vertPoss.second.size() == 1) {
         continue;
      }
      for (size_t j = 0; j < vertPoss.second.size(); j++) {
         if (handled_list[i].findindex(j) != paftl::npos) {
            continue;
         }
         handled_list[i].push_back(j);
         const Point2f& key = vertPoss.first;
         std::vector<Point2f> polygon;
         polygon.push_back(key);
         Point2f curr = vertPoss.second.at(j);
         Point2f last = key;
         bool good = true;
         while (curr != key) {
            auto vertPossIter = m_vertex_possibles.find(curr);
            polygon.push_back(curr);
            // hunt down left most
            int winner = -1, wayback = -1;
            double minangle = 2 * M_PI;
            for (size_t k = 0; k < vertPossIter->second.size(); k++) {
               Point2f next = vertPossIter->second.at(k);
               if (last != next) {
                  double thisangle = angle(last,curr,next);
                  if (thisangle < minangle) {
                     // check not going to a dead end:
                     if (m_vertex_possibles.find(vertPossIter->second.at(k))->second.size() > 1) {
                        minangle = thisangle;
                        winner = k;
                     }
                  }
               }
               else {
                  wayback = k;
               }
            }
            if (winner == -1) {
               // this happens when you follow a false trail -- go back the way you came!
               winner = wayback;
            }
            handled_list[std::distance(m_vertex_possibles.begin(), vertPossIter)].push_back(winner);
            last = curr;
            curr = vertPossIter->second.at(winner);
         }
         if (good) {
            polygons.push_back(polygon);
         }
         good = true;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////////

bool RadialLine::cuts(const Line& l) const
{
   if (fabs(det(l.end() - keyvertex,l.end() - l.start())) < TOLERANCE_A) {
      // point on line, check that openspace and next vertex are on opposite sides of the line
      Point2f x = l.end() - keyvertex;
      Point2f y = nextvertex - keyvertex;
      Point2f z = openspace - keyvertex;
      x.normalise();
      y.normalise();
      z.normalise();
      if (sgn(det(x, y)) == sgn(det(x, z)) && fabs(det(x, z)) > TOLERANCE_A) {
         return false;
      }
   }
   // keyvertex not on line... the line's been cut:
   return true;
}
