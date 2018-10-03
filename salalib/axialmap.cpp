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

#include "salalib/alllinemap.h"
#include "salalib/tolerances.h"
#include <math.h>
#include <float.h>
#include <time.h>
#include <genlib/paftl.h>
#include <genlib/comm.h>  // For communicator

#include <salalib/mgraph.h> // purely for the version info --- as phased out should replace
#include <salalib/axialmap.h>

#include <salalib/pointdata.h>   // need the pointdata for the convert boundary graph to axial map routine
#include <salalib/ngraph.h>      // ditto ngraph
#include "MapInfoData.h"

#include "genlib/stringutils.h"
#include "genlib/containerutils.h"
#include "genlib/readwritehelpers.h"

#ifndef _WIN32
#define _finite finite
#endif

////////////////////////////////////////////////////////////////////////////////////////////


static std::string makeFloatRadiusText(double radius)
{
   std::string radius_text;
   if (radius > 100.0) {
      radius_text = dXstring::formatString(radius,"%.f");
   }
   else if (radius < 0.1) {
      radius_text = dXstring::formatString(radius,"%.4f");
   }
   else {
      radius_text = dXstring::formatString(radius,"%.2f");
   }
   return radius_text;
}

////////////////////////////////////////////////////////////////////////////////////////////

// Axial map helper: convert a radius for angular analysis

static std::string makeRadiusText(int radius_type, double radius)
{
   std::string radius_text;
   if (radius != -1) {
      if (radius_type == Options::RADIUS_STEPS) {
         radius_text = std::string(" R") + dXstring::formatString(int(radius),"%d") + " step";
      }
      else if (radius_type == Options::RADIUS_METRIC) {
         radius_text = std::string(" R") + makeFloatRadiusText(radius) + " metric";
      }
      else { // radius angular
         radius_text = std::string(" R") + makeFloatRadiusText(radius);
      }
   }
   return radius_text;
}

//////////////////////////////////////////////////////////////////////////////////////////

ShapeGraph::ShapeGraph(const std::string& name, int type) : ShapeMap(name,type)
{
   m_keyvertexcount = 0;
   m_hasgraph = true;
}

////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShapeGraph::initialiseAttributesAxial()
{
    m_attributes.clear();
    // note, expects these to be numbered 0, 1...
    int conn_col = m_attributes.insertLockedColumn("Connectivity");
    int leng_col = m_attributes.insertLockedColumn("Line Length");

}

void ShapeGraph::makeConnections(const KeyVertices &keyvertices)
{
   m_connectors.clear();
   m_links.clear();
   m_unlinks.clear();
   m_keyvertices.clear();

   // note, expects these to be numbered 0, 1...
   int conn_col = m_attributes.getColumnIndex("Connectivity");
   int leng_col = m_attributes.getColumnIndex("Line Length");

   int i = -1;
   for (auto shape: m_shapes) {
      i++;
      int key = shape.first;
      int rowid = m_attributes.getRowid(key);
      // all indices should match...
      m_connectors.push_back( Connector() );
      int connectivity = getLineConnections( key, m_connectors[i].m_connections, TOLERANCE_B*__max(m_region.height(),m_region.width()));
      m_attributes.setValue(rowid, conn_col, (float) connectivity );
      m_attributes.setValue(rowid, leng_col, (float) shape.second.getLine().length() );
      if (keyvertices.size()) {
         // note: depends on lines being recorded in same order as keyvertices...
         m_keyvertices.push_back( keyvertices[i] );
      }
   }

   m_displayed_attribute = -1; // <- override if it's already showing
   setDisplayedAttribute(conn_col);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool ShapeGraph::outputMifPolygons(std::ostream& miffile, std::ostream& midfile) const
{
   // take lines from lines layer and make into regions (using the axial polygons)
   std::vector<Line> lines;
   for (auto shape: m_shapes) {
      lines.push_back(shape.second.getLine());
   }
   AxialPolygons polygons;
   polygons.init(lines, m_region);

   std::vector<std::vector<Point2f>> newpolygons;
   polygons.makePolygons(newpolygons);

   MapInfoData mapinfodata;
   if (m_hasMapInfoData) {
      mapinfodata.m_coordsys = m_mapinfodata.m_coordsys;
      mapinfodata.m_bounds = m_mapinfodata.m_bounds;
   }
   mapinfodata.exportPolygons(miffile, midfile, newpolygons, m_region);

   return true;
}

void ShapeGraph::outputNet(std::ostream& netfile) const
{
   double maxdim = __max(m_region.width(),m_region.height());
   Point2f offset = Point2f((maxdim - m_region.width())/(2.0*maxdim),(maxdim - m_region.height())/(2.0*maxdim));
   if (isSegmentMap()) {
      netfile << "*Vertices " << m_shapes.size() * 2 << std::endl;
      int i = -1;
      for (auto shape: m_shapes) {
         i++;
         Line li = shape.second.getLine();
         Point2f p1 = li.start();
         Point2f p2 = li.end();
         p1.x = offset.x + (p1.x - m_region.bottom_left.x) / maxdim;
         p2.x = offset.x + (p2.x - m_region.bottom_left.x) / maxdim;
         p1.y = 1.0 - (offset.y + (p1.y - m_region.bottom_left.y) / maxdim);
         p2.y = 1.0 - (offset.y + (p2.y - m_region.bottom_left.y) / maxdim);
         netfile << (i * 2 + 1) << " \"" << i << "a\" " << p1.x << " " << p1.y << std::endl;
         netfile << (i * 2 + 2) << " \"" << i << "b\" " << p2.x << " " << p2.y << std::endl;
      }
      netfile << "*Edges" << std::endl;
      for (size_t i = 0; i < m_shapes.size(); i++) {
         netfile << (i * 2 + 1) << " " << (i * 2 + 2) << " 2" << std::endl;
      }
      netfile << "*Arcs" << std::endl;
      // this makes an assumption about which is the "start" and which is the "end"
      // it works for an automatically converted axial map, I'm not sure it works for others...
      for (size_t j = 0; j < m_connectors.size(); j++) {
         const Connector& conn = m_connectors[j];
         for (auto& segconn: conn.m_forward_segconns) {
            SegmentRef ref = segconn.first;
            float weight = segconn.second;
            netfile << (j * 2 + 1) << " " << (ref.ref * 2 + ((ref.dir == 1) ? 1 : 2)) << " " << weight << std::endl;
         }
         for (auto& segconn: conn.m_back_segconns) {
            SegmentRef ref = segconn.first;
            float weight = segconn.second;
            netfile << (j * 2 + 2) << " " << (ref.ref * 2 + ((ref.dir == 1) ? 1 : 2)) << " " << weight << std::endl;
         }
      }
   }
   else {
      netfile << "*Vertices " << m_shapes.size() << std::endl;
      int i = -1;
      for (auto shape: m_shapes) {
         i++;
         Point2f p = shape.second.getCentroid();
         p.x = offset.x + (p.x - m_region.bottom_left.x) / maxdim;
         p.y = 1.0 - (offset.y + (p.y - m_region.bottom_left.y) / maxdim);
         netfile << (i + 1) << " \"" << i << "\" " << p.x << " " << p.y << std::endl;
      }
      netfile << "*Edges" << std::endl;
      for (size_t j = 0; j < m_connectors.size(); j++) {
         const Connector& conn = m_connectors[j];
         for (size_t k = 0; k < conn.m_connections.size(); k++) {
            size_t to = conn.m_connections[k];
            if (j < to) {
               netfile << (j+1) << " " << (to + 1) << " 1" << std::endl;
            }
         }
      }
   }
}

typedef pvector<IntPair> IntPairVector;

// n.b., translate radius list before entry
bool ShapeGraph::integrate(Communicator *comm, const pvecint& radius_list, bool choice, bool local, bool fulloutput, int weighting_col, bool simple_version)
{
   // note, from 10.0, Depthmap no longer includes *self* connections on axial lines
   // self connections are stripped out on loading graph files, as well as no longer made

   time_t atime = 0;
   if (comm) {
      qtimer( atime, 0 );
      comm->CommPostMessage( Communicator::NUM_RECORDS, m_connectors.size() );
   }

   // note: radius must be sorted lowest to highest, but if -1 occurs ("radius n") it needs to be last...
   // ...to ensure no mess ups, we'll re-sort here:
   bool radius_n = false;
   pvecint radius;
   for (size_t i = 0; i < radius_list.size(); i++) {
      if (radius_list[i] == -1) {
         radius_n = true;
      }
      else {
         radius.add(radius_list[i]);
      }
   }
   if (radius_n) {
      radius.push_back(-1);
   }

   // retrieve weighted col data, as this may well be overwritten in the new analysis:
   pvecdouble weights;
   std::string weighting_col_text;
   if (weighting_col != -1) {
      weighting_col_text = m_attributes.getColumnName(weighting_col);
      for (size_t i = 0; i < m_connectors.size(); i++) {
         weights.push_back(m_attributes.getValue(i,weighting_col));
      }
   }

   // first enter the required attribute columns:
   size_t r;
   for (r = 0; r < radius.size(); r++) {
      std::string radius_text;
      if (radius[r] != -1) {
         radius_text = std::string(" R") + dXstring::formatString(int(radius[r]),"%d");
      }
      if (choice) {
         std::string choice_col_text = std::string("Choice") + radius_text;
         m_attributes.insertColumn(choice_col_text.c_str());
         std::string n_choice_col_text = std::string("Choice [Norm]") + radius_text;
         m_attributes.insertColumn(n_choice_col_text.c_str());
         if (weighting_col != -1) {
            std::string w_choice_col_text = std::string("Choice [") + weighting_col_text + " Wgt]" + radius_text;
            m_attributes.insertColumn(w_choice_col_text.c_str());
            std::string nw_choice_col_text = std::string("Choice [") + weighting_col_text + " Wgt][Norm]" + radius_text;
            m_attributes.insertColumn(nw_choice_col_text.c_str());
         }
      }

      if(!simple_version) {
          std::string entropy_col_text = std::string("Entropy") + radius_text;
          m_attributes.insertColumn(entropy_col_text.c_str());
      }

      std::string integ_dv_col_text = std::string("Integration [HH]") + radius_text;
      m_attributes.insertColumn(integ_dv_col_text.c_str());

      if(!simple_version) {
          std::string integ_pv_col_text = std::string("Integration [P-value]") + radius_text;
          m_attributes.insertColumn(integ_pv_col_text.c_str());
          std::string integ_tk_col_text = std::string("Integration [Tekl]") + radius_text;
          m_attributes.insertColumn(integ_tk_col_text.c_str());
          std::string intensity_col_text = std::string("Intensity") + radius_text;
          m_attributes.insertColumn(intensity_col_text.c_str());
          std::string harmonic_col_text = std::string("Harmonic Mean Depth") + radius_text;
          m_attributes.insertColumn(harmonic_col_text.c_str());
      }

      std::string depth_col_text = std::string("Mean Depth") + radius_text;
      m_attributes.insertColumn(depth_col_text.c_str());
      std::string count_col_text = std::string("Node Count") + radius_text;
      m_attributes.insertColumn(count_col_text.c_str());

      if(!simple_version) {
          std::string rel_entropy_col_text = std::string("Relativised Entropy") + radius_text;
          m_attributes.insertColumn(rel_entropy_col_text);
      }

      if (weighting_col != -1) {
         std::string w_md_col_text = std::string("Mean Depth [") + weighting_col_text + " Wgt]" + radius_text;
         m_attributes.insertColumn(w_md_col_text.c_str());
         std::string total_weight_text = std::string("Total ") + weighting_col_text + radius_text;
         m_attributes.insertColumn(total_weight_text.c_str());
      }
      if (fulloutput) {
         if(!simple_version) {
             std::string penn_norm_text = std::string("RA [Penn]") + radius_text;
             m_attributes.insertColumn(penn_norm_text);
         }
         std::string ra_col_text = std::string("RA") + radius_text;
         m_attributes.insertColumn(ra_col_text.c_str());

         if(!simple_version) {
             std::string rra_col_text = std::string("RRA") + radius_text;
             m_attributes.insertColumn(rra_col_text.c_str());
         }

         std::string td_col_text = std::string("Total Depth") + radius_text;
         m_attributes.insertColumn(td_col_text.c_str());
      }
      //
   }
   if (local) {
      if(!simple_version) {
          m_attributes.insertColumn("Control");
          m_attributes.insertColumn("Controllability");
      }
   }
   // then look up all the columns... eek:
   pvecint choice_col, n_choice_col, w_choice_col, nw_choice_col, entropy_col, integ_dv_col, integ_pv_col, integ_tk_col, intensity_col,
           depth_col, count_col, rel_entropy_col, penn_norm_col, w_depth_col, total_weight_col, ra_col, rra_col, td_col, harmonic_col;
   for (r = 0; r < radius.size(); r++) {
      std::string radius_text;
      if (radius[r] != -1) {
         radius_text = std::string(" R") + dXstring::formatString(int(radius[r]),"%d");
      }
      if (choice) {
         std::string choice_col_text = std::string("Choice") + radius_text;
         choice_col.push_back(m_attributes.getColumnIndex(choice_col_text.c_str()));
         std::string n_choice_col_text = std::string("Choice [Norm]") + radius_text;
         n_choice_col.push_back(m_attributes.getColumnIndex(n_choice_col_text.c_str()));
         if (weighting_col != -1) {
            std::string w_choice_col_text = std::string("Choice [") + weighting_col_text + " Wgt]" + radius_text;
            w_choice_col.push_back(m_attributes.getColumnIndex(w_choice_col_text.c_str()));
            std::string nw_choice_col_text = std::string("Choice [") + weighting_col_text + " Wgt][Norm]" + radius_text;
            nw_choice_col.push_back(m_attributes.getColumnIndex(nw_choice_col_text.c_str()));
         }
      }
      if(!simple_version) {
          std::string entropy_col_text = std::string("Entropy") + radius_text;
          entropy_col.push_back(m_attributes.getColumnIndex(entropy_col_text.c_str()));
      }

      std::string integ_dv_col_text = std::string("Integration [HH]") + radius_text;
      integ_dv_col.push_back(m_attributes.getColumnIndex(integ_dv_col_text.c_str()));

      if(!simple_version) {
          std::string integ_pv_col_text = std::string("Integration [P-value]") + radius_text;
          integ_pv_col.push_back(m_attributes.getColumnIndex(integ_pv_col_text.c_str()));
          std::string integ_tk_col_text = std::string("Integration [Tekl]") + radius_text;
          integ_tk_col.push_back(m_attributes.getColumnIndex(integ_tk_col_text.c_str()));
          std::string intensity_col_text = std::string("Intensity") + radius_text;
          intensity_col.push_back(m_attributes.getColumnIndex(intensity_col_text.c_str()));
          std::string harmonic_col_text = std::string("Harmonic Mean Depth") + radius_text;
          harmonic_col.push_back(m_attributes.getColumnIndex(harmonic_col_text.c_str()));
      }

      std::string depth_col_text = std::string("Mean Depth") + radius_text;
      depth_col.push_back(m_attributes.getColumnIndex(depth_col_text.c_str()));
      std::string count_col_text = std::string("Node Count") + radius_text;
      count_col.push_back(m_attributes.getColumnIndex(count_col_text.c_str()));

      if(!simple_version) {
          std::string rel_entropy_col_text = std::string("Relativised Entropy") + radius_text;
          rel_entropy_col.push_back(m_attributes.getColumnIndex(rel_entropy_col_text.c_str()));
      }

      if (weighting_col != -1) {
         std::string w_md_col_text = std::string("Mean Depth [") + weighting_col_text + " Wgt]" + radius_text;
         w_depth_col.push_back(m_attributes.getColumnIndex(w_md_col_text.c_str()));
         std::string total_weight_col_text = std::string("Total ") + weighting_col_text + radius_text;
         total_weight_col.push_back(m_attributes.getColumnIndex(total_weight_col_text.c_str()));
      }
      if (fulloutput) {
         std::string ra_col_text = std::string("RA") + radius_text;
         ra_col.push_back(m_attributes.getColumnIndex(ra_col_text.c_str()));

         if(!simple_version) {
             std::string penn_norm_text = std::string("RA [Penn]") + radius_text;
             penn_norm_col.push_back(m_attributes.getColumnIndex(penn_norm_text));
             std::string rra_col_text = std::string("RRA") + radius_text;
             rra_col.push_back(m_attributes.getColumnIndex(rra_col_text.c_str()));
         }

         std::string td_col_text = std::string("Total Depth") + radius_text;
         td_col.push_back(m_attributes.getColumnIndex(td_col_text.c_str()));
      }
   }
   int control_col, controllability_col;
   if (local) {
       if(!simple_version) {
           control_col = m_attributes.getColumnIndex("Control");
           controllability_col = m_attributes.getColumnIndex("Controllability");
       }
   }

   // for choice
   AnalysisInfo **audittrail;
   if (choice) {
      audittrail = new AnalysisInfo *[m_connectors.size()];
      for (size_t i = 0; i < m_connectors.size(); i++) {
         audittrail[i] = new AnalysisInfo [radius.size()];
      }
   }

   // n.b., for this operation we assume continuous line referencing from zero (this is silly?)
   // has already failed due to this!  when intro hand drawn fewest line (where user may have deleted)
   // it's going to get worse...

   bool *covered = new bool [m_connectors.size()];
   for (size_t i = 0; i < m_connectors.size(); i++) {
      for (size_t j = 0; j < m_connectors.size(); j++) {
         covered[j] = false;
      }
      if (choice) {
         for (size_t k = 0; k < m_connectors.size(); k++) {
            audittrail[k][0].previous.ref = -1; // note, 0th member used as radius doesn't matter
            // note, choice columns are not cleared, but cummulative over all shortest path pairs
         }
      }

      if (local) {
         double control = 0.0;
         const std::vector<int>& connections = m_connectors[i].m_connections;
         pvecint totalneighbourhood;
         for (int connection: connections) {
            // n.b., as of Depthmap 10.0, connections[j] and i cannot coexist
            // if (connections[j] != i) {
               totalneighbourhood.add(connection); // <- note add does nothing if member already exists
               int retro_size = 0;
               auto& retconnectors = m_connectors[size_t(connection)].m_connections;
               for (auto retconnector: retconnectors) {
                   retro_size++;
                   totalneighbourhood.add(retconnector); // <- note add does nothing if member already exists
               }
               control += 1.0 / double(retro_size);
            //}
         }

         if(!simple_version) {
             if (connections.size() > 0) {
                 m_attributes.setValue(i, control_col, float(control) );
                 m_attributes.setValue(i, controllability_col, float( double(connections.size()) / double(totalneighbourhood.size()-1)) );
             }
             else {
                 m_attributes.setValue(i, control_col, -1 );
                 m_attributes.setValue(i, controllability_col, -1 );
             }
         }
      }

      pvecint depthcounts;
      depthcounts.push_back(0);
      Connector& thisline = m_connectors[i];
      pflipper<IntPairVector> foundlist;
      foundlist.a().push_back(IntPair(i,-1));
      covered[i] = true;
      int total_depth = 0, depth = 1, node_count = 1, pos = -1, previous = -1; // node_count includes this 1
      double weight = 0.0, rootweight = 0.0, total_weight = 0.0, w_total_depth = 0.0;
      if (weighting_col != -1) {
         rootweight = weights[i];
         // include this line in total weights (as per nodecount)
         total_weight += rootweight;
      }
      register int index = -1;
      for (size_t r = 0; r < radius.size(); r++) {
         while (foundlist.a().size()) {
            if (!choice) {
               index = foundlist.a().tail().a;
            }
            else {
               pos = pafrand() % foundlist.a().size();
               index = foundlist.a().at(pos).a;
               previous = foundlist.a().at(pos).b;
               audittrail[index][0].previous.ref = previous; // note 0th member used here: can be used individually different radius previous
            }
            Connector& line = m_connectors[index];
            double control = 0;
            for (size_t k = 0; k < line.m_connections.size(); k++) {
               if (!covered[line.m_connections[k]]) {
                  covered[line.m_connections[k]] = true;
                  foundlist.b().push_back(IntPair(line.m_connections[k],index));
                  if (weighting_col != -1) {
                     // the weight is taken from the discovered node:
                     weight = weights[line.m_connections[k]];
                     total_weight += weight;
                     w_total_depth += depth * weight;
                  }
                  if (choice && previous != -1) {
                     // both directional paths are now recorded for choice
                     // (coincidentally fixes choice problem which was completely wrong)
                     int here = index; // note: start counting from index as actually looking ahead here
                     while (here != i) { // not i means not the current root for the path
                        audittrail[here][r].choice += 1;
                        audittrail[here][r].weighted_choice += weight * rootweight;
                        here = audittrail[here][0].previous.ref; // <- note, just using 0th position: radius for the previous doesn't matter in this analysis
                     }
                     if (weighting_col != -1) {
                        // in weighted choice, root node and current node receive values:
                        audittrail[i][r].weighted_choice += (weight * rootweight) * 0.5;
                        audittrail[line.m_connections[k]][r].weighted_choice += (weight * rootweight) * 0.5;
                     }
                  }
                  total_depth += depth;
                  node_count++;
                  depthcounts.tail() += 1;
               }
            }
            if (!choice)
               foundlist.a().pop_back();
            else
               foundlist.a().remove_at(pos);
            if (!foundlist.a().size()) {
               foundlist.flip();
               depth++;
               depthcounts.push_back(0);
               if (radius[r] != -1 && depth > radius[r]) {
                  break;
               }
            }
         }
         // set the attributes for this node:
         m_attributes.setValue(i,count_col[r],float(node_count));
         if (weighting_col != -1) {
            m_attributes.setValue(i,total_weight_col[r],float(total_weight));
         }
         // node count > 1 to avoid divide by zero (was > 2)
         if (node_count > 1) {
            // note -- node_count includes this one -- mean depth as per p.108 Social Logic of Space
            double mean_depth = double(total_depth) / double(node_count - 1);
            m_attributes.setValue(i,depth_col[r],float(mean_depth));
            if (weighting_col != -1) {
               // weighted mean depth:
               m_attributes.setValue(i,w_depth_col[r],float(w_total_depth/total_weight));
            }
            // total nodes > 2 to avoid divide by 0 (was > 3)
            if (node_count > 2 && mean_depth > 1.0) {
               double ra = 2.0 * (mean_depth - 1.0) / double(node_count - 2);
               // d-value / p-value from Depthmap 4 manual, note: node_count includes this one
               double rra_d = ra / dvalue(node_count);
               double rra_p = ra / dvalue(node_count);
               double integ_tk = teklinteg(node_count, total_depth);
               m_attributes.setValue(i,integ_dv_col[r],float(1.0/rra_d));

               if(!simple_version) {
                   m_attributes.setValue(i,integ_pv_col[r],float(1.0/rra_p));
                   if (total_depth - node_count + 1 > 1) {
                       m_attributes.setValue(i,integ_tk_col[r],float(integ_tk));
                   }
                   else {
                       m_attributes.setValue(i,integ_tk_col[r],-1.0f);
                   }
               }

               if (fulloutput) {
                  m_attributes.setValue(i,ra_col[r],float(ra));

                  if(!simple_version) {
                      m_attributes.setValue(i,rra_col[r],float(rra_d));
                  }
                  m_attributes.setValue(i,td_col[r],float(total_depth));

                  if(!simple_version) {
                      // alan's palm-tree normalisation: palmtree
                      double dmin = node_count - 1;
                      double dmax = palmtree(node_count, depth - 1);
                      if (dmax != dmin) {
                          m_attributes.setValue(i,penn_norm_col[r],float((dmax - total_depth)/(dmax - dmin)));
                      }
                  }
               }
            }
            else {
               m_attributes.setValue(i,integ_dv_col[r],-1.0f);

               if(!simple_version) {
                   m_attributes.setValue(i,integ_pv_col[r],-1.0f);
                   m_attributes.setValue(i,integ_tk_col[r],-1.0f);
               }
               if (fulloutput) {
                  m_attributes.setValue(i,ra_col[r],-1.0f);

                  if(!simple_version) {
                      m_attributes.setValue(i,rra_col[r],-1.0f);
                  }

                  m_attributes.setValue(i,td_col[r],-1.0f);

                  if(!simple_version) {
                      m_attributes.setValue(i,penn_norm_col[r],-1.0f);
                  }
               }
            }

            if(!simple_version) {
                double entropy = 0.0, intensity = 0.0, rel_entropy = 0.0, factorial = 1.0, harmonic = 0.0;
                for (size_t k = 0; k < depthcounts.size(); k++) {
                    if (depthcounts[k] != 0) {
                        // some debate over whether or not this should be node count - 1
                        // (i.e., including or not including the node itself)
                        double prob = double(depthcounts[k]) / double(node_count);
                        entropy -= prob * log2( prob );
                        // Formula from Turner 2001, "Depthmap"
                        factorial *= double(k + 1);
                        double q = (pow( mean_depth, double(k) ) / double(factorial)) * exp(-mean_depth);
                        rel_entropy += (double) prob * log2( prob / q );
                        //
                        harmonic += 1.0 / double(depthcounts[k]);
                    }
                }
                harmonic = double(depthcounts.size()) / harmonic;
                if (total_depth > node_count) {
                    intensity = node_count * entropy / (total_depth - node_count);
                }
                else {
                    intensity = -1;
                }
                m_attributes.setValue(i,entropy_col[r],float(entropy));
                m_attributes.setValue(i,rel_entropy_col[r],float(rel_entropy));
                m_attributes.setValue(i,intensity_col[r],float(intensity));
                m_attributes.setValue(i,harmonic_col[r],float(harmonic));
            }
         }
         else {
            m_attributes.setValue(i,depth_col[r],-1.0f);
            m_attributes.setValue(i,integ_dv_col[r],-1.0f);

            if(!simple_version) {
                m_attributes.setValue(i,integ_pv_col[r],-1.0f);
                m_attributes.setValue(i,integ_tk_col[r],-1.0f);
                m_attributes.setValue(i,entropy_col[r],-1.0f);
                m_attributes.setValue(i,rel_entropy_col[r],-1.0f);
                m_attributes.setValue(i,harmonic_col[r],-1.0f);
            }
         }

      }
      //
      if (comm) {
         if (qtimer( atime, 500 )) {
            if (comm->IsCancelled()) {
               delete [] covered;
               throw Communicator::CancelledException();
            }
            comm->CommPostMessage( Communicator::CURRENT_RECORD, i );
         }
      }
   }
   delete [] covered;
   if (choice) {
      for (size_t i = 0; i < m_connectors.size(); i++) {
         double total_choice = 0.0, w_total_choice = 0.0;
         for (size_t r = 0; r < radius.size(); r++) {
            total_choice += audittrail[i][r].choice;
            w_total_choice += audittrail[i][r].weighted_choice;
            // n.b., normalise choice according to (n-1)(n-2)/2 (maximum possible through routes)
            double node_count = m_attributes.getValue(i,count_col[r]);
            double total_weight;
            if (weighting_col != -1) {
                total_weight = m_attributes.getValue(i,total_weight_col[r]);
            }
            if (node_count > 2) {
               m_attributes.setValue(i,choice_col[r],float(total_choice));
               m_attributes.setValue(i,n_choice_col[r],float(2.0*total_choice/((node_count-1)*(node_count-2))));
               if (weighting_col != -1) {
                  m_attributes.setValue(i,w_choice_col[r],float(w_total_choice));
                  m_attributes.setValue(i,nw_choice_col[r],float(2.0*w_total_choice/(total_weight*total_weight)));
               }
            }
            else {
               m_attributes.setValue(i,choice_col[r],-1);
               m_attributes.setValue(i,n_choice_col[r],-1);
               if (weighting_col != -1) {
                  m_attributes.setValue(i,w_choice_col[r],-1);
                  m_attributes.setValue(i,nw_choice_col[r],-1);
               }
            }
         }
      }
      for (size_t i = 0; i < m_connectors.size(); i++) {
         delete [] audittrail[i];
      }
      delete [] audittrail;
   }

   m_displayed_attribute = -1; // <- override if it's already showing
   setDisplayedAttribute(integ_dv_col.tail());

   return true;
}

bool ShapeGraph::stepdepth(Communicator *comm)
{
   std::string stepdepth_col_text = std::string("Step Depth");
   int stepdepth_col = m_attributes.insertColumn(stepdepth_col_text.c_str());

   bool *covered = new bool [m_connectors.size()];
   for (size_t i = 0; i < m_connectors.size(); i++) {
      covered[i] = false;
   }
   pflipper<pvecint> foundlist;
   for(auto& lineindex: m_selection_set) {
      foundlist.a().push_back(lineindex);
      covered[lineindex] = true;
      m_attributes.setValue(lineindex,stepdepth_col,0.0f);
   }
   int depth = 1;
   while (foundlist.a().size()) {
      Connector& line = m_connectors[foundlist.a().tail()];
      for (size_t k = 0; k < line.m_connections.size(); k++) {
         if (!covered[line.m_connections[k]]) {
            covered[line.m_connections[k]] = true;
            foundlist.b().push_back(line.m_connections[k]);
            m_attributes.setValue(line.m_connections[k],stepdepth_col,float(depth));
         }
      }
      foundlist.a().pop_back();
      if (!foundlist.a().size()) {
         foundlist.flip();
         depth++;
      }
   }
   delete [] covered;

   m_displayed_attribute = -1; // <- override if it's already showing
   setDisplayedAttribute(stepdepth_col);

   return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool ShapeGraph::read(std::istream &stream, int version )
{
   m_attributes.clear();
   m_connectors.clear();
   m_selection = false;
   m_map_type = ShapeMap::EMPTYMAP;

   bool segmentmap = false;
   // note that keyvertexcount and keyvertices are different things! (length keyvertices not the same as keyvertexcount!)
   stream.read((char *)&m_keyvertexcount,sizeof(m_keyvertexcount));
   int size;
   stream.read((char *)&size,sizeof(size));
   for (int i = 0; i < size; i++) {
      m_keyvertices.push_back(std::vector<int>());
      dXreadwrite::readIntoVector(stream, m_keyvertices.back());
   }
   // now base class read:
   ShapeMap::read(stream,version);

   return true;
}

bool ShapeGraph::readold( std::istream& stream, int version )
{
   // read in from old base class
   SpacePixel linemap;
   linemap.read(stream, version);
   const std::map<int,LineTest>& lines = linemap.getAllLines();

   m_name = linemap.getName();

   // now copy to new base class:
   init(lines.size(),linemap.getRegion());
   for (auto line: lines) {
      makeLineShape(line.second.line);
   }
   // n.b., we now have to reclear attributes!
   m_attributes.clear();

   // continue old read:
   int pushmap = -1;

   char segmentmapc = stream.get();
   if (segmentmapc == '1') {
      m_map_type = ShapeMap::SEGMENTMAP;
   }
   else {
      m_map_type = ShapeMap::AXIALMAP;
   }

   char gatemapc = stream.get();
   if (gatemapc == '1') {
      m_map_type = ShapeMap::DATAMAP;
   }
   stream.read((char *)&pushmap,sizeof(pushmap));


   int displayed_attribute;  // n.b., temp variable necessary to force recalc below
   stream.read((char *)&displayed_attribute,sizeof(displayed_attribute));

   m_attributes.read(stream,version);
   int size;
   stream.read((char *)&size,sizeof(size));
   for (int j = 0; j < size; j++) {
      m_keyvertices.push_back(std::vector<int>());    // <- these were stored with the connector
      int key;
      stream.read((char *)&key,sizeof(key)); // <- key deprecated
      m_connectors.push_back(Connector());
      m_connectors[size_t(j)].read(stream);
   }
   stream.read((char *)&m_keyvertexcount,sizeof(m_keyvertexcount));


   m_links.read(stream);
   m_unlinks.read(stream);

   char x = stream.get();
   if (x == 'm') {
      m_mapinfodata = MapInfoData();
      m_mapinfodata.read(stream,version);
      m_hasMapInfoData = true;
   }


   // now, as soon as loaded, must recalculate our screen display:
   // note m_displayed_attribute should be -2 in order to force recalc...
   m_displayed_attribute = -2;
   setDisplayedAttribute(displayed_attribute);

   return true;
}

bool ShapeGraph::write( std::ofstream& stream, int version )
{
   // note keyvertexcount and keyvertices are different things!  (length keyvertices not the same as keyvertexcount!)
   stream.write((char *)&m_keyvertexcount,sizeof(m_keyvertexcount));
   int size = m_keyvertices.size();
   stream.write((char *)&size,sizeof(size));
   for (size_t i = 0; i < m_keyvertices.size(); i++) {
      dXreadwrite::writeVector(stream, m_keyvertices[i]);
   }

   // now simply run base class write:
   ShapeMap::write(stream,version);

   return true;
}

void ShapeGraph::writeAxialConnectionsAsDotGraph(std::ostream &stream)
{
    const std::vector<Connector>& connectors = ShapeMap::getConnections();

    stream << "strict graph {" << std::endl;

    stream.precision(12);

    for (size_t i = 0; i < connectors.size(); i++) {
        const std::vector<int>& connections = connectors[i].m_connections;
        for (int connection: connections) {
            stream << "    " << i << " -- " << connection << std::endl;
        }
    }
    stream << "}" << std::endl;
}

void ShapeGraph::writeAxialConnectionsAsPairsCSV(std::ostream &stream)
{
    const std::vector<Connector>& connectors = ShapeMap::getConnections();

    stream.precision(12);

    stream << "refA,refB" << std::endl;

    for (size_t i = 0; i < connectors.size(); i++) {
        auto& connections = connectors[i].m_connections;
        if (i != 0) stream << std::endl;
        for (auto iter = connections.begin(); iter != connections.end(); ++iter) {
            if (iter != connections.begin()) stream << std::endl;
            stream << i << "," << *iter;
        }
    }
}

void ShapeGraph::writeSegmentConnectionsAsPairsCSV(std::ostream &stream)
{
    const std::vector<Connector>& connectors = ShapeMap::getConnections();

    stream.precision(12);

    stream << "refA,refB,ss_weight,for_back,dir";

    // directed links
    for (size_t i = 0; i < connectors.size(); i++) {
        for (auto& segconn: connectors[i].m_forward_segconns) {
            stream << std::endl;
            stream << i << "," << segconn.first.ref
                   << "," << segconn.second
                   << "," << 0 // forward
                   << "," << int(segconn.first.dir);
        }

        for (auto& segconn: connectors[i].m_back_segconns) {
            stream << std::endl;
            stream << i << "," << segconn.first.ref
                   << "," << segconn.second
                   << "," << 1 // back
                   << "," << int(segconn.first.dir);
        }
    }
}

void ShapeGraph::unlinkAtPoint(const Point2f& unlinkPoint) {
    pqvector<Point2f> closepoints;
    prefvec<IntPair> intersections;
    PixelRef pix = pixelate(unlinkPoint);
    std::vector<ShapeRef>& pix_shapes = m_pixel_shapes[size_t(pix.x + pix.y*m_cols)];
    auto iter = pix_shapes.begin();
    for (; iter != pix_shapes.end(); ++iter) {
       for (auto jter = iter; jter != pix_shapes.end(); ++jter) {
          auto aIter = m_shapes.find(int(iter->m_shape_ref));
          auto bIter = m_shapes.find(int(jter->m_shape_ref));
          int a = int(std::distance(m_shapes.begin(), aIter));
          int b = int(std::distance(m_shapes.begin(), bIter));
          auto& connections = m_connectors[size_t(a)].m_connections;
          if (aIter != m_shapes.end() && bIter != m_shapes.end()
                  && aIter->second.isLine() && bIter->second.isLine()
                  && std::find(connections.begin(), connections.end(), b) != connections.end()) {
             closepoints.push_back( intersection_point(aIter->second.getLine(), bIter->second.getLine(), TOLERANCE_A) );
             intersections.push_back( IntPair(a,b) );
          }
       }
    }
    double mindist = -1.0;
    int minpair = -1;
    for (size_t j = 0; j < closepoints.size(); j++) {
       if (minpair == -1 || dist(unlinkPoint,closepoints[j]) < mindist) {
          mindist = dist(unlinkPoint,closepoints[j]);
          minpair = int(j);
       }
    }
    if (minpair != -1) {
       unlinkShapes(intersections[size_t(minpair)].a, intersections[size_t(minpair)].b, false);
    }
    else {
       std::cerr << "eek!";
    }
}
////////////////////////////////////////////////////////////////////////////

// this unlink options was originally excised on the version 7 recode
// however, it is *very specific* to axial maps, and so have been reincluded here

void ShapeGraph::unlinkFromShapeMap(const ShapeMap& shapemap)
{
   // used to make a shape map from every axial intersection,

   // find lines in rough vincinity of unlink point, and check for the closest
   // pair to unlink:

   const std::map<int,SalaShape>& polygons = shapemap.getAllShapes();
   for (auto polygon: polygons) {
      // just use the points:
      if (polygon.second.isPoint()) {
         unlinkAtPoint(polygon.second.getPoint());
      }
   }

   // reset displayed attribute if it happens to be "Connectivity":
   int conn_col = m_attributes.getColumnIndex("Connectivity");
   if (getDisplayedAttribute() == conn_col) {
      invalidateDisplayedAttribute();
      setDisplayedAttribute(conn_col);  // <- reflect changes to connectivity counts
   }
}



///////////////////////////////////////////////////////////////////////////////

// Two ways to make a segment map

// Method 1: direct linkage of endpoints where they touch

void ShapeGraph::makeNewSegMap()
{
   // now make a connection set from the ends of lines:
   prefvec<Connector> connectionset;
   std::map<int,Line> lineset;
   for (auto shape: m_shapes) {
      if (shape.second.isLine()) {
         connectionset.push_back(Connector());
         lineset[shape.first] = shape.second.getLine();
      }
   }

   double maxdim = __max(m_region.width(),m_region.height());

   int seg_a = -1;
   for (auto seg_a_line: lineset) {
       seg_a++;
      // n.b., vector() is based on t_start and t_end, so we must use t_start and t_end here and throughout
      PixelRef pix1 = pixelate(seg_a_line.second.t_start());
      std::vector<ShapeRef> &shapes1 = m_pixel_shapes[size_t(pix1.x + pix1.y*m_cols)];
      for (auto& shape: shapes1) {
         auto seg_b_iter = lineset.find(int(shape.m_shape_ref));
         int seg_b = int(std::distance(lineset.begin(), seg_b_iter));
         if (seg_b_iter != lineset.end() && seg_a < seg_b) {
            Point2f alpha = seg_a_line.second.vector();
            Point2f beta  = seg_b_iter->second.vector();
            alpha.normalise();
            beta.normalise();
            if (approxeq(seg_a_line.second.t_start(),seg_b_iter->second.t_start(),(maxdim*TOLERANCE_B))) {
               float x = float(2.0 * acos(__min(__max(-dot(alpha,beta),-1.0),1.0)) / M_PI);
               depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_back_segconns, SegmentRef(1,seg_b), x);
               depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_back_segconns, SegmentRef(1,seg_a), x);
            }
            if (approxeq(seg_a_line.second.t_start(),seg_b_iter->second.t_end(),(maxdim*TOLERANCE_B))) {
               float x = float(2.0 * acos(__min(__max(-dot(alpha,-beta),-1.0),1.0)) / M_PI);
               depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_back_segconns, SegmentRef(-1,seg_b), x);
               depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_forward_segconns, SegmentRef(1,seg_a), x);
            }
         }
      }
      PixelRef pix2 = pixelate(depthmapX::getMapAtIndex(m_shapes, seg_a)->second.getLine().t_end());
      std::vector<ShapeRef> &shapes2 = m_pixel_shapes[size_t(pix2.x + pix2.y*m_cols)];
      for (auto& shape: shapes2) {
         auto seg_b_iter = lineset.find(int(shape.m_shape_ref));
         int seg_b = int(std::distance(lineset.begin(), seg_b_iter));
         if (seg_b_iter != lineset.end() && seg_a < seg_b) {
            Point2f alpha = seg_a_line.second.vector();
            Point2f beta  = seg_b_iter->second.vector();
            alpha.normalise();
            beta.normalise();
            if (approxeq(seg_a_line.second.t_end(),seg_b_iter->second.t_start(),(maxdim*TOLERANCE_B))) {
               float x = float(2.0 * acos(__min(__max(-dot(-alpha,beta),-1.0),1.0)) / M_PI);
               depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_forward_segconns, SegmentRef(1,seg_b), x);
               depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_back_segconns, SegmentRef(-1,seg_a), x);
            }
            if (approxeq(seg_a_line.second.t_end(),seg_b_iter->second.t_end(),(maxdim*TOLERANCE_B))) {
               float x = float(2.0 * acos(__min(__max(-dot(-alpha,-beta),-1.0),1.0)) / M_PI);
               depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_forward_segconns, SegmentRef(-1,seg_b), x);
               depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_forward_segconns, SegmentRef(-1,seg_a), x);
            }
         }
      }
   }

   // initialise attributes now separated from making the connections
   makeSegmentConnections(connectionset);
}

// Method 2: Making a segment map (in two stages)

// One: take the original axial map and split it up
// (note: you need to start from an axial map,
//  but the map could have been created from a road-centre-line
//  graph or equivalent -- reason is that you might want to
//  preserve unlinks in your angular mapping)

// A "linetest" is used in order to use the test component to
// identify the original axial line this line segment is
// associated with

void ShapeGraph::makeSegmentMap(std::vector<Line>& lineset, prefvec<Connector>& connectionset, double stubremoval)
{
   // the first (key) pair is the line / line intersection, second is the pair of associated segments for the first line
   std::map<OrderedIntPair,IntPair> segmentlist;

   // this code relies on the polygon order being the same as the connections

   auto iter = m_shapes.begin();
   for (size_t i = 0; i < m_connectors.size(); i++) {
      auto shape = iter->second;
      iter++;
      if (!shape.isLine()) {
         continue;
      }
      const Line& line = shape.getLine();
      pmap<double,int> breaks;
      int axis = line.width() >= line.height() ? XAXIS : YAXIS;
      // we need the breaks ordered from start to end of the line
      // this is automatic for XAXIS, but on YAXIS, need to know
      // if the line is ascending or decending
      int parity = (axis == XAXIS) ? 1 : line.sign();

      auto& connections = m_connectors[i].m_connections;
      for (size_t j = 0; j < connections.size(); j++) {
         // find the intersection point and add...
         // note: more than one break at the same place allowed
         auto shapeJ = depthmapX::getMapAtIndex(m_shapes, connections[j])->second;
         if (i != connections[j] && shapeJ.isLine()) {
            breaks.add( parity * line.intersection_point( shapeJ.getLine(), axis, TOLERANCE_A ), connections[j], paftl::ADD_DUPLICATE );
         }
      }
      // okay, now we have a list from one end of the other of lines this line connects with
      Point2f lastpoint = line.start();
      int seg_a = -1, seg_b = -1;
      double neardist;
      // TOLERANCE_C is introduced as of 01.08.2008 although it is a fix to a bug first
      // found in July 2006.  It has been set "high" deliberately (1e-6 = a millionth of the line height / width)
      // in order to catch small errors made by operators or floating point errors in other systems
      // when drawing, for example, three axial lines intersecting
      if  (stubremoval == 0.0) {
         // if 0, convert to tolerance
         stubremoval = TOLERANCE_C;
      }
      neardist = (axis == XAXIS) ? (line.width() * stubremoval) : (line.height() * stubremoval);
      double overlapdist = (axis == XAXIS) ? (line.width() * TOLERANCE_C) : (line.height() * TOLERANCE_C);
      //
      for (size_t k = 0; k < breaks.size(); ) {
         pvecint keylist;
         if (seg_a == -1) {
            Point2f thispoint = line.point_on_line(parity * breaks.key(k),axis);
            if (fabs(parity * breaks.key(k) - line.start()[axis]) < neardist) {
               seg_a = -1;
               lastpoint = thispoint;
            }
            else  {
               Line segment_a(line.start(),thispoint);
               lineset.push_back(segment_a);
               connectionset.push_back(Connector(i));
               seg_a = lineset.size() - 1;
            }
            lastpoint = thispoint;
         }
         //
         double here = parity * breaks.key(k);
         while (k < breaks.size() && fabs(parity * breaks.key(k) - here) < overlapdist) {
            keylist.push_back(breaks.value(k));
            k++;
         }
         //
         if (k == breaks.size() && fabs(line.end()[axis] - parity * breaks.key(k-1)) < neardist) {
            seg_b = -1;
         }
         else {
            Point2f thispoint;
            if (k < breaks.size()) {
               thispoint = line.point_on_line(parity * breaks.key(k),axis);
            }
            else {
               thispoint = line.end();
            }
            Line segment_b(lastpoint,thispoint);
            lineset.push_back(segment_b);
            connectionset.push_back(Connector(i));
            seg_b = lineset.size() - 1;
            //
            lastpoint = thispoint;
         }
         //
         for (size_t j = 0; j < keylist.size(); j++) {
         //
            if (keylist[j] < (int)i) {
               // other line already segmented, look up in segment list,
               // and join segments together nicely
               auto segIter = segmentlist.find(OrderedIntPair(keylist[j],i));
               if (segIter != segmentlist.end()) {   // <- if it isn't -1 something has gone badly wrong!
                  int seg_1 = segIter->second.a;
                  int seg_2 = segIter->second.b;
                  if (seg_a != -1) {
                     if (seg_1 != -1) {
                        Point2f alpha = lineset[size_t(seg_a)].start() - lineset[size_t(seg_a)].end();
                        Point2f beta  = lineset[size_t(seg_1)].start() - lineset[size_t(seg_1)].end();
                        alpha.normalise();
                        beta.normalise();
                        float x = float(2.0 * acos(__min(__max(-dot(alpha,beta),-1.0),1.0)) / M_PI);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_forward_segconns, SegmentRef(-1,seg_1), x);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_1)].m_forward_segconns, SegmentRef(-1,seg_a), x);
                     }
                     if (seg_2 != -1) {
                        Point2f alpha = lineset[size_t(seg_a)].start() - lineset[size_t(seg_a)].end();
                        Point2f beta  = lineset[size_t(seg_2)].end() - lineset[size_t(seg_2)].start();
                        alpha.normalise();
                        beta.normalise();
                        float x = float(2.0 * acos(__min(__max(-dot(alpha,beta),-1.0),1.0)) / M_PI);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_forward_segconns, SegmentRef(1,seg_2), x);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_2)].m_back_segconns, SegmentRef(-1,seg_a), x);
                     }
                  }
                  if (seg_b != -1) {
                     if (seg_1 != -1) {
                        Point2f alpha = lineset[size_t(seg_b)].end() - lineset[size_t(seg_b)].start();
                        Point2f beta  = lineset[size_t(seg_1)].start() - lineset[size_t(seg_1)].end();
                        alpha.normalise();
                        beta.normalise();
                        float x = float(2.0 * acos(__min(__max(-dot(alpha,beta),-1.0),1.0)) / M_PI);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_back_segconns, SegmentRef(-1,seg_1), x);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_1)].m_forward_segconns, SegmentRef(1,seg_b), x);
                     }
                     if (seg_2 != -1) {
                        Point2f alpha = lineset[size_t(seg_b)].end() - lineset[size_t(seg_b)].start();
                        Point2f beta  = lineset[size_t(seg_2)].end() - lineset[size_t(seg_2)].start();
                        alpha.normalise();
                        beta.normalise();
                        float x = float(2.0 * acos(__min(__max(-dot(alpha,beta),-1.0),1.0)) / M_PI);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_back_segconns, SegmentRef(1,seg_2), x);
                        depthmapX::addIfNotExists(connectionset[size_t(seg_2)].m_back_segconns, SegmentRef(1,seg_b), x);
                     }
                  }
               }
            }
            else {
               // other line still to be segmented, add ourselves to segment list
               // to be added later
               segmentlist.insert(std::make_pair( OrderedIntPair(i,keylist[j]), IntPair(seg_a,seg_b) ));
            }
         }
         if (seg_a != -1 && seg_b != -1) {
            depthmapX::addIfNotExists(connectionset[size_t(seg_a)].m_forward_segconns, SegmentRef(1,seg_b), 0.0f);
            depthmapX::addIfNotExists(connectionset[size_t(seg_b)].m_back_segconns, SegmentRef(-1,seg_a), 0.0f);
         }
         seg_a = seg_b;
      }
   }
}

void ShapeGraph::initialiseAttributesSegment()
{
    m_attributes.clear();

    // note, expects these in alphabetical order to preserve numbering:
    m_attributes.insertLockedColumn("Axial Line Ref");
    m_attributes.insertLockedColumn("Segment Length");
}

// now segments and connections are listed separately...
// put them together in a new map

void ShapeGraph::makeSegmentConnections(prefvec<Connector>& connectionset)
{
   m_connectors.clear();

   // note, expects these in alphabetical order to preserve numbering:
   int w_conn_col = m_attributes.insertColumn("Angular Connectivity");
   int uw_conn_col = m_attributes.insertLockedColumn("Connectivity");

   int ref_col = m_attributes.getColumnIndex("Axial Line Ref");
   int leng_col = m_attributes.getColumnIndex("Segment Length");

   int i = -1;
   for (auto shape: m_shapes) {
       i++;
       Connector& connector = connectionset[size_t(i)];
      int rowid = m_attributes.getRowid(shape.first);

      m_attributes.setValue(rowid, ref_col, float(connector.m_segment_axialref));
      m_attributes.setValue(rowid, leng_col, float(shape.second.getLine().length()));

      // all indices should match... (including lineset/connectionset versus m_shapes)
      m_connectors.push_back( connector );
      float total_weight = 0.0f;
      for (auto iter = connector.m_forward_segconns.begin(); iter != connector.m_forward_segconns.end(); ++iter) {
         total_weight += iter->second;
      }
      for (auto iter = connector.m_back_segconns.begin(); iter != connector.m_back_segconns.end(); ++iter) {
         total_weight += iter->second;
      }
      m_attributes.setValue(rowid, w_conn_col, float(total_weight));
      m_attributes.setValue(rowid, uw_conn_col, float(connector.m_forward_segconns.size() + connector.m_back_segconns.size()));

      // free up connectionset as we go along:
      connectionset.free_at(i);
   }

   m_displayed_attribute = -2; // <- override if it's already showing
   setDisplayedAttribute(uw_conn_col);
}

// this pushes axial map values to a segment map
// the segment map is 'this', the axial map is passed:

void ShapeGraph::pushAxialValues(ShapeGraph& axialmap)
{
   if (m_attributes.getColumnIndex("Axial Line Ref") == -1) {
      // this should never happen
      // AT: I am converting this to throw an error
      throw MetaGraph::Error("Axial line ref does not exist");
   }

   pvecint colindices;
   for (int i = 0; i < axialmap.m_attributes.getColumnCount(); i++) {
      std::string colname = std::string("Axial ") + axialmap.m_attributes.getColumnName(i);
      colindices.push_back(m_attributes.insertColumn(colname));
   }
   for (int j = 0; j < m_attributes.getRowCount(); j++) {
      int axialref = (int) m_attributes.getValue(j,"Axial Line Ref");
      for (int k = 0; k < axialmap.m_attributes.getColumnCount(); k++) {
         float val = axialmap.m_attributes.getValue(axialref,k);
         // need to look up the column index:
         m_attributes.setValue(j,colindices[k],val);
      }
   }
}

bool ShapeGraph::analyseAngular(Communicator *comm, const pvecdouble& radius_list)
{
   if (m_map_type != ShapeMap::SEGMENTMAP) {
      return false;
   }

   time_t atime = 0;
   if (comm) {
      qtimer( atime, 0 );
      comm->CommPostMessage( Communicator::NUM_RECORDS, m_connectors.size() );
   }

   // note: radius must be sorted lowest to highest, but if -1 occurs ("radius n") it needs to be last...
   // ...to ensure no mess ups, we'll re-sort here:
   bool radius_n = false;
   pvecdouble radius;
   for (size_t i = 0; i < radius_list.size(); i++) {
      if (radius_list[i] == -1.0) {
         radius_n = true;
      }
      else {
         radius.add(radius_list[i]);
      }
   }
   if (radius_n) {
      radius.push_back(-1.0);
   }

   pvecint depth_col, count_col, total_col;
   // first enter table values
   size_t r;
   for (r = 0; r < radius.size(); r++) {
      std::string radius_text = makeRadiusText(Options::RADIUS_ANGULAR,radius[r]);
      std::string depth_col_text = std::string("Angular Mean Depth") + radius_text;
      m_attributes.insertColumn(depth_col_text.c_str());
      std::string count_col_text = std::string("Angular Node Count") + radius_text;
      m_attributes.insertColumn(count_col_text.c_str());
      std::string total_col_text = std::string("Angular Total Depth") + radius_text;
      m_attributes.insertColumn(total_col_text.c_str());
   }

   for (r = 0; r < radius.size(); r++) {
      std::string radius_text = makeRadiusText(Options::RADIUS_ANGULAR,radius[r]);
      std::string depth_col_text = std::string("Angular Mean Depth") + radius_text;
      depth_col.push_back(m_attributes.getColumnIndex(depth_col_text.c_str()));
      std::string count_col_text = std::string("Angular Node Count") + radius_text;
      count_col.push_back(m_attributes.getColumnIndex(count_col_text.c_str()));
      std::string total_col_text = std::string("Angular Total Depth") + radius_text;
      total_col.push_back(m_attributes.getColumnIndex(total_col_text.c_str()));
   }


   bool *covered = new bool [m_connectors.size()];
   for (size_t i = 0; i < m_connectors.size(); i++) {
      for (size_t j = 0; j < m_connectors.size(); j++) {
         covered[j] = false;
      }
      pmap<float,SegmentData> anglebins;
      anglebins.add(0.0f,SegmentData(0,i,SegmentRef(),0,0.0,0));
      Connector& thisline = m_connectors[i];
      pvecdouble total_depth;
      pvecint node_count;
      size_t r;
      for (r = 0; r < radius.size(); r++) {
         total_depth.push_back(0.0);
         node_count.push_back(0);
      }
      // node_count includes this one, but will be added in next algo:
      while (anglebins.size()) {
         SegmentData lineindex = anglebins.value(0);
         if (!covered[lineindex.ref]) {
            covered[lineindex.ref] = true;
            double depth_to_line = anglebins.key(0);
            total_depth[lineindex.coverage] += depth_to_line;
            node_count[lineindex.coverage] += 1;
            anglebins.remove_at(0);
            Connector& line = m_connectors[lineindex.ref];
            if (lineindex.dir != -1) {
               for (auto& segconn: line.m_forward_segconns) {
                  if (!covered[segconn.first.ref]) {
                     double angle = depth_to_line + segconn.second;
                     int rbin = lineindex.coverage;
                     while (rbin != radius.size() && radius[rbin] != -1 && angle > radius[rbin]) {
                        rbin++;
                     }
                     if (rbin != radius.size()) {
                        anglebins.add(angle, SegmentData(segconn.first,SegmentRef(),0,0.0,rbin), paftl::ADD_DUPLICATE);
                     }
                  }
               }
            }
            if (lineindex.dir != 1) {
               for (auto& segconn: line.m_back_segconns) {
                  if (!covered[segconn.first.ref]) {
                     double angle = depth_to_line + segconn.second;
                     int rbin = lineindex.coverage;
                     while (rbin != radius.size() && radius[rbin] != -1 && angle > radius[rbin]) {
                        rbin++;
                     }
                     if (rbin != radius.size()) {
                        anglebins.add(angle, SegmentData(segconn.first,SegmentRef(),0,0.0,rbin), paftl::ADD_DUPLICATE);
                     }
                  }
               }
            }
         }
         else {
            anglebins.remove_at(0);
         }
      }
      // set the attributes for this node:
      int curs_node_count = 0;
      double curs_total_depth = 0.0;
      for (r = 0; r < radius.size(); r++) {
         curs_node_count += node_count[r];
         curs_total_depth += total_depth[r];
         m_attributes.setValue(i,count_col[r],float(curs_node_count));
         if (curs_node_count > 1) {
            // note -- node_count includes this one -- mean depth as per p.108 Social Logic of Space
            double mean_depth = curs_total_depth / double(curs_node_count - 1);
            m_attributes.setValue(i,depth_col[r],float(mean_depth));
            m_attributes.setValue(i,total_col[r],float(curs_total_depth));
         }
         else {
            m_attributes.setValue(i,depth_col[r],-1);
            m_attributes.setValue(i,total_col[r],-1);
         }
      }
      //
      if (comm) {
         if (qtimer( atime, 500 )) {
            if (comm->IsCancelled()) {
               delete [] covered;
               throw Communicator::CancelledException();
            }
            comm->CommPostMessage( Communicator::CURRENT_RECORD, i );
         }
      }
   }
   delete [] covered;

   m_displayed_attribute = -2; // <- override if it's already showing
   setDisplayedAttribute(depth_col.tail());

   return true;
}

// extra parameters for selection_only and interactive are for parallel process extensions
int ShapeGraph::analyseTulip(Communicator *comm, int tulip_bins, bool choice, int radius_type, const pvecdouble& radius_list, int weighting_col, int weighting_col2, int routeweight_col, bool selection_only, bool interactive)
{
   int processed_rows = 0;

   if (m_map_type != ShapeMap::SEGMENTMAP) {
      return processed_rows;
   }

   time_t atime = 0;

   if (comm) {
      qtimer( atime, 0 );
      comm->CommPostMessage( Communicator::NUM_RECORDS, (selection_only ? m_selection_set.size() : m_connectors.size()) );
   }

   // note: radius must be sorted lowest to highest, but if -1 occurs ("radius n") it needs to be last...
   // ...to ensure no mess ups, we'll re-sort here:
   bool radius_n = false;
   pvecdouble radius_unconverted;
   for (size_t i = 0; i < radius_list.size(); i++) {
      if (radius_list[i] == -1.0) {
         radius_n = true;
      }
      else {
         radius_unconverted.add(radius_list[i]);
      }
   }
   if (radius_n) {
      radius_unconverted.push_back(-1.0);
   }

   // retrieve weighted col data, as this may well be overwritten in the new analysis:
   pvecfloat weights;
   pvecfloat routeweights;  //EF
   std::string weighting_col_text;

   if (weighting_col != -1) {
      weighting_col_text = m_attributes.getColumnName(weighting_col);
      for (size_t i = 0; i < m_connectors.size(); i++) {
         weights.push_back(m_attributes.getValue(i,weighting_col));
      }
   }
   else { // Normal run // TV
      for (size_t i = 0; i < m_connectors.size(); i++) {
         weights.push_back(1.0f);
      }
   }
   //EF routeweight*
   std::string routeweight_col_text;
   if (routeweight_col != -1) {
       //we normalise the column values between 0 and 1 and reverse it so that high values can be treated as a 'low cost' - similar to the angular cost
      double max_value = m_attributes.getMaxValue(routeweight_col);
     routeweight_col_text = m_attributes.getColumnName(routeweight_col);
      for (size_t i = 0; i < m_connectors.size(); i++) {
         routeweights.push_back(1.0-(m_attributes.getValue(i, routeweight_col)/max_value)); //scale and revert!
      }
   }
   else { // Normal run // TV
      for (size_t i = 0; i < m_connectors.size(); i++) {
         routeweights.push_back(1.0f);
      }
   }
   //*EF routeweight

   //EFEF*
   //for origin-destination weighting
   pvecfloat weights2;
   std::string weighting_col_text2;
   if (weighting_col2 != -1) {
      weighting_col_text2 = m_attributes.getColumnName(weighting_col2);
      for (size_t i = 0; i < m_connectors.size(); i++) {
         weights2.push_back(m_attributes.getValue(i,weighting_col2));
      }
   }
   else { // Normal run // TV
      for (size_t i = 0; i < m_connectors.size(); i++) {
         weights2.push_back(1.0f);
      }
   }
   //*EFEF

   std::string tulip_text = std::string("T") + dXstring::formatString(tulip_bins,"%d");

   // first enter the required attribute columns:
   size_t r;
   for (r = 0; r < radius_unconverted.size(); r++) {
      std::string radius_text = makeRadiusText(radius_type, radius_unconverted[r]);
      int choice_col = -1, n_choice_col = -1, w_choice_col = -1, nw_choice_col = -1;
      if (choice) {
            //EF routeweight *
            if (routeweight_col != -1) {
                std::string choice_col_text = tulip_text + " Choice [Route weight by " + routeweight_col_text + "]"+ radius_text;
                m_attributes.insertColumn(choice_col_text.c_str());
                if (weighting_col != -1) {
                    std::string w_choice_col_text = tulip_text + " Choice [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;

                    m_attributes.insertColumn(w_choice_col_text.c_str());
                }
                //EFEF*
                if (weighting_col2 != -1) {
                    std::string w_choice_col_text2 = tulip_text + " Choice [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + "-" + weighting_col_text2 + " Wgt]]" + radius_text;

                    m_attributes.insertColumn(w_choice_col_text2.c_str());
                }
                //*EFEF
            }
            //*EF routeweight
            else { // Normal run // TV
                std::string choice_col_text = tulip_text + " Choice" + radius_text;
                m_attributes.insertColumn(choice_col_text.c_str());
                if (weighting_col != -1) {
                    std::string w_choice_col_text = tulip_text + " Choice [" + weighting_col_text + " Wgt]" + radius_text;
                    m_attributes.insertColumn(w_choice_col_text.c_str());
                }
                //EFEF*
                if (weighting_col2 != -1) {
                    std::string w_choice_col_text2 = tulip_text + " Choice [" + weighting_col_text + "-" + weighting_col_text2 + " Wgt]" + radius_text;
                    m_attributes.insertColumn(w_choice_col_text2.c_str());
                }
                //*EFEF
            }
      }

        //EF routeweight *
        if (routeweight_col != -1) {
         std::string integ_col_text = tulip_text + " Integration [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_integ_col_text = tulip_text + " Integration [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;

         std::string count_col_text = tulip_text + " Node Count [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string td_col_text = tulip_text + " Total Depth [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            // '[' comes after 'R' in ASCII, so this column will come after Mean Depth R...
            std::string w_td_text = tulip_text + " Total Depth [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;
            std::string total_weight_text = tulip_text + " Total " + weighting_col_text + " [Route weight by " + routeweight_col_text + "]" +radius_text;

         m_attributes.insertColumn(integ_col_text.c_str());
            m_attributes.insertColumn(count_col_text.c_str());
            m_attributes.insertColumn(td_col_text.c_str());
            if (weighting_col != -1) {
            m_attributes.insertColumn(w_integ_col_text.c_str());
                m_attributes.insertColumn(w_td_text.c_str());
                m_attributes.insertColumn(total_weight_text.c_str());
            }
        }
        //*EF routeweight
        else { // Normal run // TV
            std::string integ_col_text = tulip_text + " Integration" + radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_integ_col_text = tulip_text + " Integration [" + weighting_col_text + " Wgt]" + radius_text;

            std::string count_col_text = tulip_text + " Node Count" + radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string td_col_text = tulip_text + " Total Depth" + radius_text; // <- note, the fact this is a tulip is unnecessary
            // '[' comes after 'R' in ASCII, so this column will come after Mean Depth R...
            std::string w_td_text = tulip_text + " Total Depth [" + weighting_col_text + " Wgt]" + radius_text;
            std::string total_weight_text = tulip_text + " Total " + weighting_col_text + radius_text;

         m_attributes.insertColumn(integ_col_text.c_str());
            m_attributes.insertColumn(count_col_text.c_str());
            m_attributes.insertColumn(td_col_text.c_str());
            if (weighting_col != -1) {
            m_attributes.insertColumn(w_integ_col_text.c_str());
                m_attributes.insertColumn(w_td_text.c_str());
                m_attributes.insertColumn(total_weight_text.c_str());
            }
        }
   }
   pvecint choice_col, w_choice_col, w_choice_col2, count_col, integ_col, w_integ_col, td_col, w_td_col, total_weight_col;
   // then look them up! eek....
   for (r = 0; r < radius_unconverted.size(); r++) {
      std::string radius_text = makeRadiusText(radius_type, radius_unconverted[r]);
      if (choice) {
            //EF routeweight *
            if (routeweight_col != -1) {
                std::string choice_col_text = tulip_text + " Choice [Route weight by " + routeweight_col_text + "]"+ radius_text;
                choice_col.push_back(m_attributes.getColumnIndex(choice_col_text.c_str()));
                if (weighting_col != -1) {
                    std::string w_choice_col_text = tulip_text + " Choice [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;
                    w_choice_col.push_back(m_attributes.getColumnIndex(w_choice_col_text.c_str()));
                }
                //EFEF*
                if (weighting_col2 != -1) {
                    std::string w_choice_col_text2 = tulip_text + " Choice [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + "-" + weighting_col_text2 + " Wgt]]" + radius_text;
                    w_choice_col2.push_back(m_attributes.getColumnIndex(w_choice_col_text2.c_str()));
                }
                //*EFEF
            }
            //* EF routeweight
            else { // Normal run // TV
                std::string choice_col_text = tulip_text + " Choice" + radius_text;
                choice_col.push_back(m_attributes.getColumnIndex(choice_col_text.c_str()));
                if (weighting_col != -1) {
                    std::string w_choice_col_text = tulip_text + " Choice [" + weighting_col_text + " Wgt]" + radius_text;
                    w_choice_col.push_back(m_attributes.getColumnIndex(w_choice_col_text.c_str()));
                }
                //EFEF*
                if (weighting_col2 != -1) {
                    std::string w_choice_col_text2 = tulip_text + " Choice [" + weighting_col_text + "-" + weighting_col_text2 + " Wgt]" + radius_text;
                    w_choice_col2.push_back(m_attributes.getColumnIndex(w_choice_col_text2.c_str()));
                }
                //*EFEF
            }

      }
        //EF routeweight *
        if (routeweight_col != -1) {
         std::string integ_col_text = tulip_text + " Integration [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_integ_col_text = tulip_text + " Integration [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;

         std::string count_col_text = tulip_text + " Node Count [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string td_col_text = tulip_text + " Total Depth [Route weight by " + routeweight_col_text + "]"+ radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_td_text = tulip_text + " Total Depth [[Route weight by " + routeweight_col_text + "][" + weighting_col_text + " Wgt]]" + radius_text;
            std::string total_weight_col_text = tulip_text + " Total " + weighting_col_text + " [Route weight by " + routeweight_col_text + "]" +radius_text;

         integ_col.push_back(m_attributes.getColumnIndex(integ_col_text.c_str()));
            count_col.push_back(m_attributes.getColumnIndex(count_col_text.c_str()));
            td_col.push_back(m_attributes.getColumnIndex(td_col_text.c_str()));
            if (weighting_col != -1) {
                // '[' comes after 'R' in ASCII, so this column will come after Mean Depth R...
            w_integ_col.push_back(m_attributes.getColumnIndex(w_integ_col_text.c_str()));
                w_td_col.push_back(m_attributes.getColumnIndex(w_td_text.c_str()));
                total_weight_col.push_back(m_attributes.getColumnIndex(total_weight_col_text.c_str()));
            }
        }
        //* EF routeweight
        else { // Normal run // TV
            std::string integ_col_text = tulip_text + " Integration" + radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_integ_col_text = tulip_text + " Integration [" + weighting_col_text + " Wgt]" + radius_text;

         std::string count_col_text = tulip_text + " Node Count" + radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string td_col_text = tulip_text + " Total Depth" + radius_text; // <- note, the fact this is a tulip is unnecessary
            std::string w_td_text = tulip_text + " Total Depth [" + weighting_col_text + " Wgt]" + radius_text;
            std::string total_weight_col_text = tulip_text + " Total " + weighting_col_text + radius_text;

         integ_col.push_back(m_attributes.getColumnIndex(integ_col_text.c_str()));
            count_col.push_back(m_attributes.getColumnIndex(count_col_text.c_str()));
            td_col.push_back(m_attributes.getColumnIndex(td_col_text.c_str()));
            if (weighting_col != -1) {
                // '[' comes after 'R' in ASCII, so this column will come after Mean Depth R...
            w_integ_col.push_back(m_attributes.getColumnIndex(w_integ_col_text.c_str()));
                w_td_col.push_back(m_attributes.getColumnIndex(w_td_text.c_str()));
                total_weight_col.push_back(m_attributes.getColumnIndex(total_weight_col_text.c_str()));
            }
        }
   }

   tulip_bins /= 2;  // <- actually use semicircle of tulip bins
   tulip_bins += 1;

   std::vector<std::vector<SegmentData>> bins(tulip_bins);

   AnalysisInfo ***audittrail;
   unsigned int **uncovered;
   audittrail = new AnalysisInfo **[m_connectors.size()];
   uncovered = new unsigned int *[m_connectors.size()];
   for (size_t i = 0; i < m_connectors.size(); i++) {
      audittrail[i] = new AnalysisInfo *[radius_unconverted.size()];
      for (size_t j = 0; j < radius_unconverted.size(); j++) {
         audittrail[i][j] = new AnalysisInfo[2];
      }
      uncovered[i] = new unsigned int [2];

   }
   pvecdouble radius;
   for (r = 0; r < radius_unconverted.size(); r++) {
      if (radius_type == Options::RADIUS_ANGULAR && radius_unconverted[r] != -1) {
         radius.push_back(floor(radius_unconverted[r] * tulip_bins * 0.5));
      }
      else {
         radius.push_back(radius_unconverted[r]);
      }
   }
   // entered once for each segment
   int length_col = m_attributes.getColumnIndex("Segment Length");
   pvecfloat lengths;
   if (length_col != -1) {
      for (size_t i = 0; i < m_connectors.size(); i++) {
         lengths.push_back(m_attributes.getValue(i,length_col));
      }
   }

   int radiussize = radius.size();
   int radiusmask = 0;
   for (int i = 0; i < radiussize; i++) {
      radiusmask |= (1 << i);
   }

   for (size_t rowid = 0; rowid < m_connectors.size(); rowid++) {

      if (selection_only) {
         // could use m_selection_set.searchindex(rowid) to find
         // if this row is selected as m_selection_set is ordered for axial and segment maps, etc
         // BUT, actually quicker to check the tag in the attributes that shows it's selected
         if (!m_attributes.isSelected(rowid)) {
            continue;
         }
      }

      for (int k = 0; k < tulip_bins; k++) {
         bins[k].clear();
      }
      for (size_t j = 0; j < m_connectors.size(); j++) {
         for (int dir = 0; dir < 2; dir++) {
            for (int k = 0; k < radiussize; k++) {
               audittrail[j][k][dir].clearLine();
            }
            uncovered[j][dir] = radiusmask;
         }
      }

      double rootseglength = m_attributes.getValue(rowid,length_col);
      double rootweight = (weighting_col != -1) ? weights[rowid] : 0.0;
        //EFEF
        double rootweight2 = (weighting_col2 != -1) ? weights2[rowid] : 0.0;
        //EFEF

      // setup: direction 0 (both ways), segment i, previous -1, segdepth (step depth) 0, metricdepth 0.5 * rootseglength, bin 0
      SegmentData segmentData(0,rowid,SegmentRef(),0,0.5*rootseglength,radiusmask);
      auto it = std::lower_bound(bins[0].begin(), bins[0].end(), segmentData);
      if(it == bins[0].end() || segmentData != *it) {
          bins[0].insert(it, segmentData);
      }
      // this version below is only designed to be used temporarily --
      // could be on an option?
      //bins[0].push_back(SegmentData(0,rowid,SegmentRef(),0,0.0,radiusmask));
      Connector& thisline = m_connectors[rowid];
      pvecint node_count;
      double weight = 0.0;
      int depthlevel = 0;
      int opencount = 1;
      size_t currentbin = 0;
      while (opencount) {
         while (!bins[currentbin].size()) {
            depthlevel++;
            currentbin++;
            if (currentbin == tulip_bins) {
               currentbin = 0;
            }
         }
         SegmentData lineindex = bins[currentbin].back();
         bins[currentbin].pop_back();
         //
         opencount--;

         int ref = lineindex.ref;
         int dir = (lineindex.dir == 1) ? 0 : 1;
         int coverage = lineindex.coverage & uncovered[ref][dir];
         if (coverage != 0) {
            register int rbin = 0;
            int rbinbase;
            if (lineindex.previous.ref != -1) {
               uncovered[ref][dir] &= ~coverage;
               while (((coverage >> rbin) & 0x1) == 0)
                  rbin++;
               rbinbase = rbin;
               while (rbin < radiussize) {
                  if (((coverage >> rbin) & 0x1) == 1) {
                     audittrail[ref][rbin][dir].depth = depthlevel;
                     audittrail[ref][rbin][dir].previous = lineindex.previous;
                     audittrail[lineindex.previous.ref][rbin][(lineindex.previous.dir == 1) ? 0 : 1].leaf = false;
                  }
                  rbin++;
               }
            }
            else {
               rbinbase = 0;
               uncovered[ref][0] &= ~coverage;
               uncovered[ref][1] &= ~coverage;
            }
            Connector& line = m_connectors[ref];
            float seglength;
            register int extradepth;
            if (lineindex.dir != -1) {
               for (auto& segconn: line.m_forward_segconns) {
                  rbin = rbinbase;
                  SegmentRef conn = segconn.first;
                  if ((uncovered[conn.ref][(conn.dir == 1 ? 0 : 1)] & coverage) != 0) {
                            //EF routeweight*
                            if (routeweight_col != -1) {  //EF here we do the weighting of the angular cost by the weight of the next segment
                                                    //note that the content of the routeweights array is scaled between 0 and 1 and is reversed
                                                    // such that: = 1.0-(m_attributes.getValue(i, routeweight_col)/max_value)
                                extradepth = (int) floor(segconn.second * tulip_bins * 0.5 * routeweights[conn.ref]);
                            }
                            //*EF routeweight
                            else {
                                extradepth = (int) floor(segconn.second * tulip_bins * 0.5);
                            }
                            seglength = lengths[conn.ref];
                     switch (radius_type) {
                     case Options::RADIUS_ANGULAR:
                        while (rbin != radiussize && radius[rbin] != -1 && depthlevel+extradepth > (int) radius[rbin]) {
                           rbin++;
                        }
                        break;
                     case Options::RADIUS_METRIC:
                        while (rbin != radiussize && radius[rbin] != -1 && lineindex.metricdepth+seglength*0.5 > radius[rbin]) {
                           rbin++;
                        }
                        break;
                     case Options::RADIUS_STEPS:
                        if (rbin != radiussize && radius[rbin] != -1 && lineindex.segdepth >= (int) radius[rbin]) {
                           rbin++;
                        }
                        break;
                     }
                     if ((coverage >> rbin) != 0) {
                         SegmentData sd(conn,SegmentRef(1,lineindex.ref),lineindex.segdepth+1,lineindex.metricdepth+seglength,(coverage >> rbin) << rbin);
                         size_t bin = (currentbin + tulip_bins + extradepth) % tulip_bins;
                         depthmapX::insert_sorted(bins[bin],sd);
                         opencount++;
                     }
                  }
               }
            }
            if (lineindex.dir != 1) {
               for (auto& segconn: line.m_back_segconns) {
                  rbin = rbinbase;
                  SegmentRef conn = segconn.first;
                  if ((uncovered[conn.ref][(conn.dir == 1 ? 0 : 1)] & coverage) != 0) {
                            //EF routeweight*
                            if (routeweight_col != -1) {  //EF here we do the weighting of the angular cost by the weight of the next segment
                                                    //note that the content of the routeweights array is scaled between 0 and 1 and is reversed
                                                    // such that: = 1.0-(m_attributes.getValue(i, routeweight_col)/max_value)
                                extradepth = (int) floor(segconn.second * tulip_bins * 0.5 * routeweights[conn.ref]);
                            }
                            //*EF routeweight
                            else {
                                extradepth = (int) floor(segconn.second * tulip_bins * 0.5);
                            }
                     seglength = lengths[conn.ref];
                     switch (radius_type) {
                     case Options::RADIUS_ANGULAR:
                        while (rbin != radiussize && radius[rbin] != -1 && depthlevel+extradepth > (int) radius[rbin]) {
                           rbin++;
                        }
                        break;
                     case Options::RADIUS_METRIC:
                        while (rbin != radiussize && radius[rbin] != -1 && lineindex.metricdepth+seglength*0.5 > radius[rbin]) {
                           rbin++;
                        }
                        break;
                     case Options::RADIUS_STEPS:
                        if (rbin != radiussize && radius[rbin] != -1 && lineindex.segdepth >= (int) radius[rbin]) {
                           rbin++;
                        }
                        break;
                     }
                     if ((coverage >> rbin) != 0) {
                         SegmentData sd(conn,SegmentRef(-1,lineindex.ref),lineindex.segdepth+1,lineindex.metricdepth+seglength,(coverage >> rbin) << rbin);
                         size_t bin = (currentbin + tulip_bins + extradepth) % tulip_bins;
                         depthmapX::insert_sorted(bins[bin], sd);
                         opencount++;
                     }
                  }
               }
            }
         }
      }
      // set the attributes for this node:
      for (int k = 0; k < radiussize; k++) {
         // note, curs_total_depth must use double as mantissa can get too long for int in large systems
         double curs_node_count = 0.0, curs_total_depth = 0.0;
         double curs_total_weight = 0.0, curs_total_weighted_depth = 0.0;
         size_t j;
         for (j = 0; j < m_connectors.size(); j++) {
            // find dir according
            bool m0 = ((uncovered[j][0] >> k) & 0x1) == 0;
            bool m1 = ((uncovered[j][1] >> k) & 0x1) == 0;
            if ((m0 | m1) != 0) {
               int dir;
               if (m0 & m1) {
                  // dir is the one with the lowest depth:
                  if (audittrail[j][k][0].depth < audittrail[j][k][1].depth)
                     dir = 0;
                  else
                     dir = 1;
               }
               else {
                  // dir is simply the one that's filled in:
                  dir = m0 ? 0 : 1;
               }
               curs_node_count++;
               curs_total_depth += audittrail[j][k][dir].depth;
               curs_total_weight += weights[j];
               curs_total_weighted_depth += audittrail[j][k][dir].depth * weights[j];
               //
               if (choice && audittrail[j][k][dir].leaf) {
                  // note, graph may be directed (e.g., for one way streets), so both ways must be included from now on:
                  SegmentRef here = SegmentRef(dir == 0 ? 1 : -1,j);
                  if (here.ref != rowid) {
                     int choicecount = 0;
                     double choiceweight = 0.0;
                            //EFEF*
                            double choiceweight2 = 0.0;
                            //*EFEF
                     while (here.ref != rowid) { // not rowid means not the current root for the path
                        int heredir = (here.dir == 1) ? 0 : 1;
                        // each node has the existing choicecount and choiceweight from previously encountered nodes added to it
                        audittrail[here.ref][k][heredir].choice += choicecount;
                        // nb, weighted values calculated anyway to save time on 'if'
                        audittrail[here.ref][k][heredir].weighted_choice += choiceweight;
                        //EFEF*
                                audittrail[here.ref][k][heredir].weighted_choice2 += choiceweight2;
                                //*EFEF
                                // if the node hasn't been encountered before, the choicecount and choiceweight is
                        // incremented for all remaining nodes to be encountered on the backwards route from it
                        if (!audittrail[here.ref][k][heredir].choicecovered) {
                           // this node has not been encountered before: this adds the choicecount and weight for this
                           // node, and flags it as visited
                           choicecount++;
                           choiceweight += weights[here.ref] * rootweight;
                                    //EFEF*
                                    choiceweight2 += weights2[here.ref] * rootweight;//rootweight!
                                    //*EFEF

                           audittrail[here.ref][k][heredir].choicecovered = true;
                           // note, for weighted choice, the start and end points have choice added to them:
                           if (weighting_col != -1) {
                              audittrail[here.ref][k][heredir].weighted_choice += (weights[here.ref] * rootweight) / 2.0;
                                        //EFEF*
                                        if (weighting_col2 != -1) {
                                            audittrail[here.ref][k][heredir].weighted_choice2 += (weights2[here.ref] * rootweight) / 2.0;  //rootweight!
                                        }
                                        //*EFEF
                                    }
                        }
                        here = audittrail[here.ref][k][heredir].previous;
                     }
                     // note, for weighted choice, the start and end points have choice added to them:
                     // (this is the summed weight for all starting nodes encountered in this path)
                     if (weighting_col != -1) {
                        audittrail[here.ref][k][(here.dir == 1) ? 0 : 1].weighted_choice += choiceweight / 2.0;
                                //EFEF*
                                if (weighting_col2 != -1) {
                                    audittrail[here.ref][k][(here.dir == 1) ? 0 : 1].weighted_choice2 += choiceweight2 / 2.0;
                                }
                                //*EFEF
                            }
                  }
               }
            }
         }
         double total_depth_conv = curs_total_depth / ((tulip_bins - 1.0f) * 0.5f);
         double total_weighted_depth_conv = curs_total_weighted_depth / ((tulip_bins - 1.0f) * 0.5f);
         //
         m_attributes.setValue(rowid,count_col[k],float(curs_node_count));
         if (curs_node_count > 1) {
            // for dmap 8 and above, mean depth simply isn't calculated as for radius measures it is meaningless
            m_attributes.setValue(rowid,td_col[k],total_depth_conv);
            if (weighting_col != -1) {
               m_attributes.setValue(rowid,total_weight_col[k],float(curs_total_weight));
               m_attributes.setValue(rowid,w_td_col[k],float(total_weighted_depth_conv));
            }
         }
         else {
            m_attributes.setValue(rowid,td_col[k],-1);
            if (weighting_col != -1) {
               m_attributes.setValue(rowid,total_weight_col[k],-1.0f);
               m_attributes.setValue(rowid,w_td_col[k],-1.0f);
            }
         }
         // for dmap 10 an above, integration is included!
         if (total_depth_conv > 1e-9) {
            m_attributes.setValue(rowid,integ_col[k],(float)(curs_node_count*curs_node_count/total_depth_conv));
            if (weighting_col != -1) {
               m_attributes.setValue(rowid,w_integ_col[k],(float)(curs_total_weight*curs_total_weight/total_weighted_depth_conv));
            }
         }
         else {
            m_attributes.setValue(rowid,integ_col[k],-1);
            if (weighting_col != -1) {
               m_attributes.setValue(rowid,w_integ_col[k],-1.0f);
            }
         }
      }
      //
      processed_rows++;
      //
      if (comm) {
         if (qtimer( atime, 500 )) {
            if (comm->IsCancelled()) {
                // interactive is usual Depthmap: throw an exception if cancelled
                if (interactive) {
                  for (size_t i = 0; i < m_connectors.size(); i++) {
                     for (size_t j = 0; j < size_t(radiussize); j++) {
                        delete [] audittrail[i][j];
                     }
                     delete [] audittrail [i];
                     delete [] uncovered [i];
                  }
                  delete [] audittrail;
                  delete [] uncovered;
                  throw Communicator::CancelledException();
               }
               else {
                  // in non-interactive mode, retain what's been processed already
                  break;
               }
            }
            comm->CommPostMessage( Communicator::CURRENT_RECORD, rowid );
         }
      }
   }
   if (choice) {
      for (size_t rowid = 0; rowid < m_connectors.size(); rowid++) {
         for (size_t r = 0; r < radius.size(); r++) {
            // according to Eva's correction, total choice and total weighted choice
            // should already have been accumulated by radius at this stage
            double total_choice = audittrail[rowid][r][0].choice + audittrail[rowid][r][1].choice;
            double total_weighted_choice = audittrail[rowid][r][0].weighted_choice + audittrail[rowid][r][1].weighted_choice;
            //EFEF*
                double total_weighted_choice2 = audittrail[rowid][r][0].weighted_choice2 + audittrail[rowid][r][1].weighted_choice2;
                //*EFEF

                // normalised choice now excluded for two reasons:
            // a) not useful measure, b) in parallel calculations, cannot be calculated at this stage
            // n.b., it is possible through the front end: the new choice takes into account bidirectional routes,
            // so it should be normalised according to (n-1)(n-2) (maximum possible through routes) not (n-1)(n-2)/2
            // the relativised segment length weighted choice equation was (total_seg_length*total_seg_length-seg_length*seg_length)/2
            // again, drop the divide by 2 for the new implementation
            //
            //
            m_attributes.setValue(rowid,choice_col[r],float(total_choice));
            if (weighting_col != -1) {
               m_attributes.setValue(rowid,w_choice_col[r],float(total_weighted_choice));
                    //EFEF*
                    if (weighting_col2 != -1) {
                        m_attributes.setValue(rowid,w_choice_col2[r],float(total_weighted_choice2));
                    }
                    //*EFEF
                }
         }
      }
   }
   for (size_t i = 0; i < m_connectors.size(); i++) {
      for (int j = 0; j < radiussize; j++) {
         delete [] audittrail[i][j];
      }
      delete [] audittrail [i];
      delete [] uncovered [i];
   }
   delete [] audittrail;
   delete [] uncovered;

   m_displayed_attribute = -2; // <- override if it's already showing
   if (choice) {
      setDisplayedAttribute(choice_col.tail());
   }
   else {
      setDisplayedAttribute(td_col.tail());
   }
   return processed_rows;
}

// revised to use tulip bins for faster analysis of large spaces

bool ShapeGraph::angularstepdepth(Communicator *comm)
{
   std::string stepdepth_col_text = "Angular Step Depth";
   int stepdepth_col = m_attributes.insertColumn(stepdepth_col_text.c_str());

   int tulip_bins = 1024;
   // calc so duplicate code above
   tulip_bins /= 2;  // <- actually use semicircle of tulip bins
   tulip_bins += 1;

   bool *covered = new bool [m_connectors.size()];
   for (size_t i = 0; i < m_connectors.size(); i++) {
      covered[i] = false;
   }
   pqvector<SegmentData> *bins = new pqvector<SegmentData>[tulip_bins];

   int opencount = 0;
   for (auto& sel: m_selection_set) {
      int row = m_attributes.getRowid(sel);
      if (row != -1) {
         bins[0].push_back(SegmentData(0,row,SegmentRef(),0,0.0,0));
         opencount++;
      }
   }
   int depthlevel = 0;
   int currentbin = 0;
   while (opencount) {
      while (!bins[currentbin].size()) {
         depthlevel++;
         currentbin++;
         if (currentbin == tulip_bins) {
            currentbin = 0;
         }
      }
      SegmentData lineindex;
      if (bins[currentbin].size() > 1) {
         // it is slightly slower to delete from an arbitrary place in the bin,
         // but it is necessary to use random paths to even out the number of times through equal paths
         int curr = pafrand() % bins[currentbin].size();
         lineindex = bins[currentbin][curr];
         bins[currentbin].remove_at(curr);
         // note: do not clear choice values here!
      }
      else {
         lineindex = bins[currentbin][0];
         bins[currentbin].pop_back();
      }
      opencount--;
      if (!covered[lineindex.ref]) {
         covered[lineindex.ref] = true;
         Connector& line = m_connectors[lineindex.ref];
         // convert depth from tulip_bins normalised to standard angle
         // (note the -1)
         double depth_to_line = depthlevel / ((tulip_bins - 1) * 0.5);
         m_attributes.setValue(lineindex.ref,stepdepth_col,depth_to_line);
         register int extradepth;
         if (lineindex.dir != -1) {
            for (auto& segconn: line.m_forward_segconns) {
               if (!covered[segconn.first.ref]) {
                  extradepth = (int) floor(segconn.second * tulip_bins * 0.5);
                  bins[(currentbin + tulip_bins + extradepth) % tulip_bins].push_back(
                      SegmentData(segconn.first,lineindex.ref,lineindex.segdepth+1,0.0,0));
                  opencount++;
               }
            }
         }
         if (lineindex.dir != 1) {
            for (auto& segconn: line.m_back_segconns) {
               if (!covered[segconn.first.ref]) {
                  extradepth = (int) floor(segconn.second * tulip_bins * 0.5);
                  bins[(currentbin + tulip_bins + extradepth) % tulip_bins].push_back(
                      SegmentData(segconn.first,lineindex.ref,lineindex.segdepth+1,0.0,0));
                  opencount++;
                }
            }
         }
      }
   }
   delete [] covered;
   delete [] bins;

   m_displayed_attribute = -2; // <- override if it's already showing
   setDisplayedAttribute(stepdepth_col);

   return true;
}
