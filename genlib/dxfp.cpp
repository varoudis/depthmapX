// genlib - a component of the depthmapX - spatial network analysis platform
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



// parser class

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

#include <genlib/paftl.h>
#include <genlib/comm.h>  // for communicator
#include <genlib/dxfp.h>

static int counter = 0;

///////////////////////////////////////////////////////////////////////////////

bool operator > (const DxfTableRow& a, const DxfTableRow& b)  // for hash table
{
   return a.m_name > b.m_name;
}

bool operator < (const DxfTableRow& a, const DxfTableRow& b)  // for hash table
{
   return a.m_name < b.m_name;
}

bool operator == (const DxfTableRow& a, const DxfTableRow& b) // for hash table
{
   return a.m_name == b.m_name;
}

///////////////////////////////////////////////////////////////////////////////

DxfParser::DxfParser(Communicator *comm /* = NULL */)
{
   m_communicator = comm;
   m_size = 0;
   m_time = 0;
}

const DxfVertex& DxfParser::getExtMin() const
{
   return m_region.getExtMin();
}

const DxfVertex& DxfParser::getExtMax() const
{
   return m_region.getExtMax();
}

const DxfLayer& DxfParser::getLayerNum( const int i ) const
{
   return m_layers[i];
}

DxfLayer *DxfParser::getLayer( const std::string& layer_name ) // const <- removed as m_layers may be changed if DXF is poor
{
   static DxfLayer layer;

   layer.m_name = layer_name;

   size_t n = m_layers.searchindex( layer );
   if (n == paftl::npos) {
      n = m_layers.add( layer, paftl::ADD_HERE );
   }
   return &m_layers[n];
}

const DxfLineType& DxfParser::getLineType( const int i ) const
{
   return m_line_types[i];
}

DxfLineType *DxfParser::getLineType( const std::string& line_type_name )  // const <- removed as m_layers may be changed if DXF is poor
{
   static DxfLineType line_type;

   line_type.m_name = line_type_name;

   size_t n = m_line_types.searchindex( line_type );
   if (n == paftl::npos) {
      n = m_line_types.add( line_type, paftl::ADD_HERE );
   }
   return &m_line_types[n];
}

int DxfParser::numLayers() const
{
   return m_layers.size();
}

int DxfParser::numLineTypes() const
{
   return m_line_types.size();
}

///////////////////////////////////////////////////////////////////////////////

istream& operator >> (istream& stream, DxfParser& dxfp)
{
   if (dxfp.m_communicator)
   {
      long size = dxfp.m_communicator->GetInfileSize();
      dxfp.m_communicator->CommPostMessage( Communicator::NUM_RECORDS, size );

      qtimer( dxfp.m_time, 0 );
   }

   return dxfp.open( stream );
}

istream& DxfParser::open( istream& stream )
{
   DxfToken token;
   int section = UNIDENTIFIED;

   while (!stream.eof() && section != _EOF) {
      switch (section)
      {
         case ZEROTOKEN:
            if (token.data == "SECTION") {
               // find out the section
               stream >> token;
               m_size += token.size;
               //
               if (token.code != 2) {
                  // oops...
                  section = UNIDENTIFIED;
               }
               else if (token.data == "HEADER") {
                  section = HEADER;
               }
               else if (token.data == "TABLES") {
                  section = TABLES;
               }
               else if (token.data == "BLOCKS") {
                  section = BLOCKS;
               }
               else if (token.data == "ENTITIES") {
                  section = ENTITIES;
               }
               else {
                  section = UNIDENTIFIED;
               }
            }
            else if (token.data == "EOF") {
               section = _EOF;
            }
            else {
               section = UNIDENTIFIED;
            }
            break;
         case HEADER:
            openHeader( stream );
            section = UNIDENTIFIED;
            break;
         case TABLES:
            openTables( stream );
            section = UNIDENTIFIED;
            break;
         case BLOCKS:
            openBlocks( stream );
            section = UNIDENTIFIED;
            break;
         case ENTITIES:
            openEntities( stream, token ); // I'm adding the token here as before the function was unsafe, but I'm not sure reuse of this token is a good idea AT 29-APR-11
            section = UNIDENTIFIED;
            break;
         default:
            stream >> token;
            m_size += token.size;
            if (token.code == 0) {
               section = ZEROTOKEN;
            }
            break;
      }
      if (m_communicator)
      {
         counter++;
         if (qtimer( m_time, 500 )) {
            if (m_communicator->IsCancelled()) {
               throw Communicator::CancelledException();
            }
            m_communicator->CommPostMessage( Communicator::CURRENT_RECORD, m_size );
         }
      }
   }
   // Get overall bounding box from layers:
   bool first = true;
   for (size_t i = 0; i < m_layers.size(); i++) {
      if (!m_layers[i].empty()) {
         m_region.merge( m_layers[i] );
      }
   }
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

void DxfParser::openHeader( istream& stream )
{
   DxfToken token;
   int subsection = UNIDENTIFIED;

   DxfVertex vertex;

   while (!stream.eof() && subsection != ENDSEC) {
      switch (subsection) {
         case ZEROTOKEN:
            if (token.data == "ENDSEC") {
               subsection = ENDSEC;
            }
            /*
            // EXTMIN and EXTMAX are deprecated: Now calculate ourselves instead...
            // although now my blocks reading is done properly, should be okay!
            else if (token.data == "$EXTMIN") {
               subsection = EXTMIN;
            }
            else if (token.data == "$EXTMAX") {
               subsection = EXTMAX;
            }
            */
            else {
               subsection = UNIDENTIFIED;
            }
            break;
            /*
            // EXTMIN and EXTMAX are deprecated: Now calculate ourselves instead...
            // although now my blocks reading is done properly, should be okay!
         case EXTMIN:
            stream >> token;
            m_size += token.size;
            if ( vertex.parse(token, this) ) {
               m_extmin = vertex;
               vertex.clear(); // reuse
               subsection = ZEROTOKEN;
            }
            break;
         case EXTMAX:
            stream >> token;
            m_size += token.size;
            if ( vertex.parse(token, this) ) {
               m_extmax = vertex;
               vertex.clear(); // reuse
               subsection = ZEROTOKEN;
            }
            break;
            */
         default:
            stream >> token;
            m_size += token.size;
            if (token.code == 0 || token.code == 9 ) {   // 9 is used as a '0' in the header
               subsection = ZEROTOKEN;
            }
            break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DxfParser::openTables( istream& stream )
{
   DxfToken token;
   int subsection = UNIDENTIFIED;

   DxfLayer    layer;
   DxfLineType line_type;

   while (!stream.eof() && subsection != ENDSEC) {
      switch (subsection) {
         case ZEROTOKEN:
            if (token.data == "TABLE") {
               // find out the table type
               stream >> token;
               m_size += token.size;
               //
               if (token.code != 2) {
                  // oops...
                  subsection = UNIDENTIFIED;
               }
               else if (token.data == "LTYPE") {
                  subsection = LTYPE_TABLE;
               }
               else if (token.data == "LAYER") {
                  subsection = LAYER_TABLE;
               }
               else {
                  subsection = UNIDENTIFIED;
               }
            }
            else if (token.data == "ENDSEC") {
               subsection = ENDSEC;
            }
            else {
               subsection = UNIDENTIFIED;
            }
            break;
         case LTYPE_TABLE:
            stream >> token;
            m_size += token.size;
            if (token.code == 0) {
               if (token.data == "LTYPE") {
                  subsection = LTYPE_ROW;
               }
               else if (token.data == "ENDTAB") {
                  subsection = ZEROTOKEN;
               }
            }
            break;
         case LTYPE_ROW:
            stream >> token;
            m_size += token.size;
            if ( line_type.parse( token, this ) ) {
               m_line_types.add( line_type );
               if (token.data == "ENDTAB") {
                  subsection = ZEROTOKEN;
               }
            }
            break;
         case LAYER_TABLE:
            stream >> token;
            m_size += token.size;
            if (token.code == 0) {
               if (token.data == "LAYER") {
                  subsection = LAYER_ROW;
               }
               else if (token.data == "ENDTAB") {
                  subsection = ZEROTOKEN;
               }
            }
            break;
         case LAYER_ROW:
            stream >> token;
            m_size += token.size;
            if ( layer.parse( token, this ) ) {
//               m_layers.add( layer );
               if (token.data == "ENDTAB") {
                  subsection = ZEROTOKEN;
               }
            }
            break;
         default:
            stream >> token;
            m_size += token.size;
            if (token.code == 0 ) {
               subsection = ZEROTOKEN;
            }
            break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DxfParser::openBlocks( istream& stream )
{
   DxfToken token;
   int subsection = UNIDENTIFIED;

   DxfBlock block;

   while (!stream.eof() && subsection != ENDSEC) {
      switch (subsection) {
         case ZEROTOKEN:
            if (token.data == "BLOCK") {
               subsection = BLOCK;
            }
            else if (token.data == "ENDSEC") {
               subsection = ENDSEC;
            }
            else {
               subsection = UNIDENTIFIED;
            }
            break;
         case BLOCK:
            stream >> token;
            m_size += token.size;
            if ( block.parse( token, this ) ) {
               int pos = m_blocks.add( block );
               if (token.data == "ENDBLK") {
                  subsection = ZEROTOKEN;
               }
               else {
                  // this drills down to the data for the block:
                  openEntities(stream, token, &m_blocks[pos] );
                  // only if the block ends should we move up:
                  if (token.data == "ENDBLK") {
                     subsection = ZEROTOKEN;
                  }
               }
            }
            break;
         default:
            stream >> token;
            m_size += token.size;
            if (token.code == 0 ) {
               subsection = ZEROTOKEN;
            }
            break;
      }
      if (m_communicator)
      {
         counter++;
         if (qtimer( m_time, 500 )) {
            if (m_communicator->IsCancelled()) {
               throw Communicator::CancelledException();
            }
            m_communicator->CommPostMessage( Communicator::CURRENT_RECORD, m_size );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DxfParser::openEntities( istream& stream, DxfToken& token, DxfBlock *block )
{
   int subsection = UNIDENTIFIED;
   if (token.code == 0) {
      // a block must always pass it's first token:
      subsection = ZEROTOKEN;
   }

   DxfVertex       point;
   DxfLine        line;
   DxfPolyLine    poly_line;
   DxfLwPolyLine  lw_poly_line;
   DxfArc         arc;
   DxfCircle      circle;
   DxfSpline      spline;
   DxfInsert      insert;

   std::string layer_name;
   std::string line_type_name;

   while (!stream.eof() && subsection != ENDSEC) {
      switch (subsection) {
         case ZEROTOKEN:
            if (token.data == "POINT") {
               subsection = POINT;
            }
            else if (token.data == "LINE") {
               subsection = LINE;
            }
            else if (token.data == "POLYLINE") {
               subsection = POLYLINE;
            }
            else if (token.data == "LWPOLYLINE") {
               subsection = LWPOLYLINE;
            }
            else if (token.data == "ARC") {
               subsection = ARC;
            }
            else if (token.data == "CIRCLE") {
               subsection = CIRCLE;
            }
            else if (token.data == "SPLINE") {
               subsection = SPLINE;
            }
            else if (token.data == "INSERT") {
               subsection = INSERT;
            }
            else if (token.data == "ENDSEC" || token.data == "ENDBLK") {
               subsection = ENDSEC;
            }
            else {
               subsection = UNIDENTIFIED;
            }
            break;
         case POINT:
            stream >> token;
            m_size += token.size;
            if ( point.parse( token, this ) ) {
               DxfLayer *layer = block;
               if (layer == NULL) {
                  layer = point.m_p_layer;
               }
               layer->m_points.push_back( point );
               layer->merge(point); // <- merge bounding box
               layer->m_total_point_count += 1;
               point.clear();
               subsection = ZEROTOKEN;
            }
            break;
         case LINE:
            stream >> token;
            m_size += token.size;
            if ( line.parse( token, this ) ) {
               if (line.m_start != line.m_end) {
                  DxfLayer *layer = block;
                  if (layer == NULL) {
                     layer = line.m_p_layer;
                  }
                  layer->m_lines.push_back( line );
                  layer->merge(line); // <- merge bounding box
                  layer->m_total_line_count += 1;
               }
               line.clear();
               subsection = ZEROTOKEN;
            }
            break;
         case POLYLINE:
            stream >> token;
            m_size += token.size;
            if ( poly_line.parse( token, this ) ) {
               if (poly_line.m_vertex_count > 0) {
                  DxfLayer *layer = block;
                  if (layer == NULL) {
                     layer = poly_line.m_p_layer;
                  }
                  layer->m_poly_lines.push_back( poly_line );
                  int line_count = (poly_line.getAttributes() & DxfPolyLine::CLOSED) ?
                     poly_line.numVertices() - 2 : poly_line.numVertices() - 1;
                  layer->merge(poly_line); // <- merge bounding box
                  layer->m_total_line_count += line_count;
                  poly_line.clear(); // (Now reuse)
               }
               poly_line.clear(); // (Now reuse)
               subsection = ZEROTOKEN;
            }
            break;
         case LWPOLYLINE:
            stream >> token;
            m_size += token.size;
            if ( lw_poly_line.parse( token, this ) ) {
               if (lw_poly_line.m_vertex_count > 0) {
                  DxfLayer *layer = block;
                  if (layer == NULL) {
                     layer = lw_poly_line.m_p_layer;
                  }
                  layer->m_poly_lines.push_back( (const DxfPolyLine&) lw_poly_line );
                  int line_count = (lw_poly_line.getAttributes() & DxfPolyLine::CLOSED) ?
                     lw_poly_line.numVertices() - 2 : lw_poly_line.numVertices() - 1;
                  layer->merge(lw_poly_line); // <- merge bounding box
                  layer->m_total_line_count += line_count;
               }
               lw_poly_line.clear(); // (Now reuse)
               subsection = ZEROTOKEN;
            }
            break;
         case ARC:
            stream >> token;
            m_size += token.size;
            if ( arc.parse( token, this ) ) {
               DxfLayer *layer = block;
               if (layer == NULL) {
                  layer = arc.m_p_layer;
               }
               layer->m_arcs.push_back( (const DxfArc&) arc );
               layer->merge(arc);
               arc.clear(); // (Now reuse)
               subsection = ZEROTOKEN;
            }
            break;
         case CIRCLE:
            stream >> token;
            m_size += token.size;
            if ( circle.parse( token, this ) ) {
               DxfLayer *layer = block;
               if (layer == NULL) {
                  layer = circle.m_p_layer;
               }
               layer->m_circles.push_back( (const DxfCircle&) circle );
               layer->merge(circle);
               circle.clear(); // (Now reuse)
               subsection = ZEROTOKEN;
            }
            break;
         case SPLINE:
            stream >> token;
            m_size += token.size;
            if ( spline.parse( token, this ) ) {
               if (spline.numVertices() > 0) {
                  DxfLayer *layer = block;
                  if (layer == NULL) {
                     layer = spline.m_p_layer;
                  }
                  layer->m_splines.push_back( (const DxfSpline&) spline );
                  int line_count = (spline.getAttributes() & DxfSpline::CLOSED) ?
                    spline.numVertices() - 2 : spline.numVertices() - 1;
                  layer->merge(spline);
                  layer->m_total_line_count += line_count;
                  spline.clear(); // (Now reuse)
               }
               subsection = ZEROTOKEN;
            }
            break;
         case INSERT:
            stream >> token;
            m_size += token.size;
            if ( insert.parse( token, this ) ) {
               if ( insert.m_block ) {
                  DxfLayer *layer = block;
                  if (layer == NULL) {
                     layer = insert.m_p_layer;
                  }
                  layer->insert( insert, this );
               }
               insert.clear();
               subsection = ZEROTOKEN;
            }
            break;
         default:
            stream >> token;
            m_size += token.size;
            if (token.code == 0 ) {
               subsection = ZEROTOKEN;
            }
            break;
      }
      if (m_communicator)
      {
         counter++;
         if (qtimer( m_time, 500 )) {
            if (m_communicator->IsCancelled()) {
               throw Communicator::CancelledException();
            }
            m_communicator->CommPostMessage( Communicator::CURRENT_RECORD, m_size );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

// Individual parsing of the types

DxfTableRow::DxfTableRow(const std::string& name)
{
   m_name = name;
}

bool DxfTableRow::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 2:
         m_name = token.data;
         break;
      case 0:
         parsed = true;
         break;
      default:
         break;
   }
   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfEntity::DxfEntity(int tag)
{
   m_tag = tag;
}

void DxfEntity::clear()
{
   m_tag = -1;
}

bool DxfEntity::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 5:
         m_tag = std::stoi(std::string("0x") + token.data);   // tag is in hex
         break;
      case 6:
         m_p_line_type = parser->getLineType( token.data );
         break;
      case 8:
         m_p_layer = parser->getLayer( token.data );
         break;
      case 0:
         parsed = true;
         break;
      default:
         break;
   }
   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfVertex::DxfVertex(int tag) : DxfEntity( tag )
{
   x = 0.0;
   y = 0.0;
   z = 0.0;
}

void DxfVertex::clear()
{
   x = 0.0;
   y = 0.0;
   z = 0.0;

   DxfEntity::clear();
}

bool operator == (const DxfVertex& a, const DxfVertex& b)
{
   return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool operator != (const DxfVertex& a, const DxfVertex& b)
{
   return (a.x != b.x || a.y != b.y || a.z != b.z);
}

bool DxfVertex::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 10:
         x = std::stod(token.data);
         break;
      case 20:
         y = std::stod(token.data);
         break;
      case 30:
         z = std::stod(token.data);
         break;
      case 0: case 9:   // 0 is standard vertex, 9 is for header section variables
         parsed = true;
         break;
      default:
         parsed = DxfEntity::parse( token, parser ); // base class parse
   }

   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfLine::DxfLine(int tag) : DxfEntity( tag )
{
}

void DxfLine::clear()
{
   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfLine::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 10:
         m_start.x = std::stod(token.data);
         break;
      case 20:
         m_start.y = std::stod(token.data);
         break;
      case 30:
         m_start.z = std::stod(token.data);
         break;
      case 11:
         m_end.x = std::stod(token.data);
         break;
      case 21:
         m_end.y = std::stod(token.data);
         break;
      case 31:
         m_end.z = std::stod(token.data);
         break;
      case 0:
         add(m_start);  // <- add to region
         add(m_end);    // <- add to region
         parsed = true;
         break;
      default:
         parsed = DxfEntity::parse( token, parser ); // base class parse
         break;
   }
   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfPolyLine::DxfPolyLine(int tag) : DxfEntity( tag )
{
   clear();
}

void DxfPolyLine::clear()
{
   m_vertex_count = 0;
   m_vertices.clear();
   m_attributes = 0;

   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfPolyLine::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   static DxfVertex vertex;

   if (m_vertex_count) {
      if ( vertex.parse( token, parser ) ) {
         add(vertex); // <- add to region
         if (m_min.x == 0) {
            cerr << "problem" << endl;
         }
         m_vertices.push_back( vertex );
         if ( token.data == "VERTEX" ) {  // Another vertex...
            m_vertex_count++;
         }
         else {   // Should be a SEQEND
            parsed = true;
         }
      }
   }
   else {   // parse the polyline header...
      switch (token.code) {
         case 0:
            if (token.data == "VERTEX") {
               m_vertex_count++;
            }
            else {
               parsed = true;
            }
            break;
         case 70:
            m_attributes = std::stoi(token.data);
         default:
            DxfEntity::parse( token, parser ); // base class parse
            break;
      }
   }
   return parsed;
}

int DxfPolyLine::numVertices() const
{
   return m_vertices.size();
}

const DxfVertex& DxfPolyLine::getVertex(int i) const
{
   return m_vertices[i];
}

int DxfPolyLine::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////

DxfLwPolyLine::DxfLwPolyLine(int tag) : DxfPolyLine( tag )
{
   clear();
}

void DxfLwPolyLine::clear()
{
   m_expected_vertex_count = 0;

   DxfPolyLine::clear();
}

bool DxfLwPolyLine::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   static DxfVertex vertex;

   switch (token.code) {
      case 0:
         // push final vertex
         if (m_vertex_count) {
            add(vertex); // <- add vertex to region
            m_vertices.push_back( vertex );
         }
         parsed = true;
         break;
      case 10:
         if (m_vertex_count) {
            // push last vertex
            add(vertex); // <- add vertex to region
            m_vertices.push_back( vertex );
         }
         m_vertex_count++;
         vertex.clear();
         vertex.parse( token, parser );
         break;
      case 20:
      case 30:
         // continue last vertex:
         vertex.parse( token, parser );
         break;
      case 70:
         m_attributes = std::stoi(token.data);
      case 90:
         m_expected_vertex_count = std::stoi(token.data);
      default:
         DxfEntity::parse( token, parser ); // base class parse
         break;
   }

   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfArc::DxfArc(int tag) : DxfEntity( tag )
{
}

void DxfArc::clear()
{
   m_start = 0.0;
   m_end = 0.0;

   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfArc::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 10:
         m_centre.x = std::stod(token.data);
         break;
      case 20:
         m_centre.y = std::stod(token.data);
         break;
      case 30:
         m_centre.z = std::stod(token.data);
         break;
      case 40:
         m_radius = std::stod(token.data);
         break;
      case 50:
         m_start = std::stod(token.data);
         break;
      case 51:
         m_end = std::stod(token.data);
         break;
      case 0:
         {
            // just loop round if m_start is bigger than m_end
            if (m_start > m_end) {
               m_end += 360;
            }
            // technically should check for arc limits for tighter bounding box,
            // but easier to give circular bounding box
            DxfVertex bounds;
            bounds.x = m_centre.x - m_radius;
            bounds.y = m_centre.y - m_radius;
            bounds.z = m_centre.z;
            add(bounds);  // <- add to region
            bounds.x = m_centre.x + m_radius;
            bounds.y = m_centre.y + m_radius;
            bounds.z = m_centre.z;
            add(bounds);  // <- add to region
            parsed = true;
         }
         break;
      default:
         parsed = DxfEntity::parse( token, parser ); // base class parse
         break;
   }

   return parsed;
}

int DxfArc::numSegments(int segments) const
{
   return ((m_start == m_end) ? segments : (int(m_end - m_start) * segments / 360));
}

DxfVertex DxfArc::getVertex(int i, int segments) const
{
   DxfVertex v = m_centre;
   double ang = 2.0 * DXF_PI * double(i)/double(segments);
   if (m_start != m_end) {
      ang += 2.0 * DXF_PI * (m_start / 360.0);
   }
   // ARCS go anticlockwise from (1 0)
   v.x = m_centre.x + m_radius * cos(ang);
   v.y = m_centre.y + m_radius * sin(ang);
   v.z = m_centre.z;
   return v;
}

void DxfArc::reflect(double x, double y)
{
   if (x < 0) {
      m_start = 180 - m_start;
      m_end = 180 - m_end;
   }
   if (y < 0) {
      m_start = 360 - m_start;
      m_end = 360 - m_end;
   }
   while (m_start < 0) {
      m_start += 360;
   }
   while (m_end < 0) {
      m_end += 360;
   }
   if (x * y < 0) {
      double temp;
      temp = m_start;
      m_start = m_end;
      m_end = temp;
   }
}

///////////////////////////////////////////////////////////////////////////

DxfCircle::DxfCircle(int tag) : DxfEntity( tag )
{
}

void DxfCircle::clear()
{
   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfCircle::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 10:
         m_centre.x = std::stod(token.data);
         break;
      case 20:
         m_centre.y = std::stod(token.data);
         break;
      case 30:
         m_centre.z = std::stod(token.data);
         break;
      case 40:
         m_radius = std::stod(token.data);
         break;
      case 0:
         {
            DxfVertex bounds;
            bounds.x = m_centre.x - m_radius;
            bounds.y = m_centre.y - m_radius;
            bounds.z = m_centre.z;
            add(bounds);  // <- add to region
            bounds.x = m_centre.x + m_radius;
            bounds.y = m_centre.y + m_radius;
            bounds.z = m_centre.z;
            add(bounds);  // <- add to region
            parsed = true;
         }
         break;
      default:
         parsed = DxfEntity::parse( token, parser ); // base class parse
         break;
   }

   return parsed;
}

DxfVertex DxfCircle::getVertex(int i, int segments) const
{
   DxfVertex v = m_centre;
   double ang = 2.0 * DXF_PI * double(i)/double(segments);
   // CIRCLES go anticlockwise from (1 0)
   v.x = m_centre.x + m_radius * cos(ang);
   v.y = m_centre.y + m_radius * sin(ang);
   v.z = m_centre.z;
   return v;
}

void DxfCircle::reflect(double x, double y)
{
   // reflect has no effect on a circle
}

///////////////////////////////////////////////////////////////////////////

// Spline
// n.b. currently just linear interpolation between control points -
// not good, but whatever method will have to make some sort of approximation at some point

///////////////////////////////////////////////////////////////////////////////

DxfSpline::DxfSpline(int tag) : DxfEntity( tag )
{
   clear();
}

void DxfSpline::clear()
{
   m_xyz = 0;
   m_ctrl_pt_count = 0;
   m_knot_count = 0;
   m_ctrl_pts.clear();
   m_knots.clear();
   m_attributes = 0;

   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfSpline::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   static DxfVertex vertex;

   switch (token.code) {
      case 0:
         parsed = true;
         break;
      case 70:
         m_attributes = std::stoi(token.data);
         break;
      case 72:
         m_knot_count = std::stoi(token.data);
         break;
      case 73:
         m_ctrl_pt_count = std::stoi(token.data);
         break;
      case 40:
         m_knots.push_back( std::stod(token.data) );
      case 10:
         vertex.x = std::stod(token.data);
         m_xyz |= 0x0001;
         break;
      case 20:
         vertex.y = std::stod(token.data);
         m_xyz |= 0x0010;
         break;
      case 30:
         vertex.z = std::stod(token.data);
         m_xyz |= 0x0100;
         break;
      default:
         DxfEntity::parse( token, parser ); // base class parse
         break;
   }

   if (m_xyz == 0x0111) {
      add(vertex); // <- add vertex to region
      m_ctrl_pts.push_back( vertex );
      m_xyz = 0;
   }

   return parsed;
}

// Note: return control points not actual points!

int DxfSpline::numVertices() const
{
   return m_ctrl_pts.size();
}

const DxfVertex& DxfSpline::getVertex(int i) const
{
   return m_ctrl_pts[i];
}

int DxfSpline::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////

// note: inserts are flattened on way through

DxfInsert::DxfInsert(int tag) : DxfEntity( tag )
{
   clear();
}

void DxfInsert::clear()
{
   m_block = NULL;
   m_translation.clear();
   m_scale.clear();

   // actually default scale is 1,1,1
   m_scale.x = 1.0;
   m_scale.y = 1.0;
   m_scale.z = 1.0;

   m_rotation = 0.0;

   DxfRegion::clear();
   DxfEntity::clear();
}

bool DxfInsert::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 0:
         parsed = true;
         break;
      case 2:
         // lookup in blocks table
         {
            size_t index = parser->m_blocks.searchindex(token.data);
            if (index != paftl::npos) {
               m_block = &(parser->m_blocks[index]);
            }
         }
         break;
      case 10:
         m_translation.x = std::stod(token.data);
         break;
      case 20:
         m_translation.y = std::stod(token.data);
         break;
      case 30:
         m_translation.z = std::stod(token.data);
         break;
      case 41:
         m_scale.x = std::stod(token.data);
         break;
      case 42:
         m_scale.y = std::stod(token.data);
         break;
      case 43:
         m_scale.z = std::stod(token.data);
         break;
      case 50:
         m_rotation = std::stod(token.data);
         break;
      default:
         DxfEntity::parse( token, parser ); // base class parse
         break;
   }

   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfLineType::DxfLineType(const std::string& name) : DxfTableRow( name )
{
}

bool DxfLineType::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 0:
         parsed = true;
         break;
      default:
         parsed = DxfTableRow::parse( token, parser ); // base class parse
   }
   return parsed;
}

DxfVertex& DxfLine::getStart() const
{
   return (DxfVertex&) m_start;
}

DxfVertex& DxfLine::getEnd() const
{
   return (DxfVertex&) m_end;
}

///////////////////////////////////////////////////////////////////////////////

DxfLayer::DxfLayer(const std::string& name) : DxfTableRow( name )
{
   m_total_line_count = 0;
}

bool DxfLayer::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 0:
         parsed = true;
         break;
      default:
         parsed = DxfTableRow::parse( token, parser ); // base class parse
   }
   return parsed;
}

const DxfVertex& DxfLayer::getPoint( int i ) const
{
   return m_points[i];
}

const DxfLine& DxfLayer::getLine( int i ) const
{
   return m_lines[i];
}

const DxfPolyLine& DxfLayer::getPolyLine( int i ) const
{
   return m_poly_lines[i];
}

const DxfArc& DxfLayer::getArc( int i ) const
{
   return m_arcs[i];
}

const DxfCircle& DxfLayer::getCircle( int i ) const
{
   return m_circles[i];
}

const DxfSpline& DxfLayer::getSpline( int i ) const
{
   return m_splines[i];
}

int DxfLayer::numPoints() const
{
   return m_points.size();
}

int DxfLayer::numLines() const
{
   return m_lines.size();
}

int DxfLayer::numPolyLines() const
{
   return m_poly_lines.size();
}

int DxfLayer::numArcs() const
{
   return m_arcs.size();
}

int DxfLayer::numCircles() const
{
   return m_circles.size();
}

int DxfLayer::numSplines() const
{
   return m_splines.size();
}

void DxfLayer::insert(DxfInsert& insert, DxfParser *parser)
{
   size_t i;

   // munge in insert...
   bool scale = (insert.m_scale.x != 1.0 || insert.m_scale.y != 1.0 || insert.m_scale.z != 1.0);
   bool rotate = (insert.m_rotation != 0.0);
   if (insert.m_rotation < 0) {
     insert.m_rotation += 360;
   }

   for (i = 0; i < insert.m_block->m_lines.size(); i++) {
      m_lines.push_back(insert.m_block->m_lines[i]);
      // rotate, translate, scale each line as specified in the insert
      if (scale)
         m_lines.tail().scale(insert.m_block->m_base_point,insert.m_scale);
      if (rotate)
         m_lines.tail().rotate(insert.m_block->m_base_point,insert.m_rotation);
      m_lines.tail().translate(insert.m_translation);
      merge(m_lines.tail()); // <- merge bounding box
   }
   for (i = 0; i < insert.m_block->m_poly_lines.size(); i++) {
      m_poly_lines.push_back(insert.m_block->m_poly_lines[i]);
      // rotate, translate, scale each line as specified in the insert
      if (scale)
         m_poly_lines.tail().scale(insert.m_block->m_base_point,insert.m_scale);
      if (rotate)
         m_poly_lines.tail().rotate(insert.m_block->m_base_point,insert.m_rotation);
      m_poly_lines.tail().translate(insert.m_translation);
      merge(m_poly_lines.tail()); // <- merge bounding box
   }
   for (i = 0; i < insert.m_block->m_arcs.size(); i++) {
      m_arcs.push_back(insert.m_block->m_arcs[i]);
      // rotate, translate, scale each line as specified in the insert
      if (scale)
         m_arcs.tail().scale(insert.m_block->m_base_point,insert.m_scale);
      if (rotate)
         m_arcs.tail().rotate(insert.m_block->m_base_point,insert.m_rotation);
      m_arcs.tail().translate(insert.m_translation);
      merge(m_arcs.tail()); // <- merge bounding box
   }
   for (i = 0; i < insert.m_block->m_circles.size(); i++) {
      m_circles.push_back(insert.m_block->m_circles[i]);
      // rotate, translate, scale each line as specified in the insert
      if (scale)
         m_circles.tail().scale(insert.m_block->m_base_point,insert.m_scale);
      // n.b., rotate does nothing with circles
      if (rotate)
         m_circles.tail().rotate(insert.m_block->m_base_point,insert.m_rotation);
      m_circles.tail().translate(insert.m_translation);
      merge(m_circles.tail()); // <- merge bounding box
   }
   for (i = 0; i < insert.m_block->m_splines.size(); i++) {
      m_splines.push_back(insert.m_block->m_splines[i]);
      // rotate, translate, scale each line as specified in the insert
      if (scale)
         m_splines.tail().scale(insert.m_block->m_base_point,insert.m_scale);
      if (rotate)
         m_splines.tail().rotate(insert.m_block->m_base_point,insert.m_rotation);
      m_splines.tail().translate(insert.m_translation);
      merge(m_splines.tail()); // <- merge bounding box
   }

   m_total_line_count += insert.m_block->m_total_line_count;
}

///////////////////////////////////////////////////////////////////////////////

DxfBlock::DxfBlock(const std::string& name) : DxfLayer( name )
{
}

bool DxfBlock::parse( const DxfToken& token, DxfParser *parser )
{
   bool parsed = false;

   switch (token.code) {
      case 0:
         parsed = true;
         break;
      default:
         parsed = DxfLayer::parse( token, parser ); // base class parse
   }
   return parsed;
}

///////////////////////////////////////////////////////////////////////////////

DxfToken::DxfToken()
{
   size = 0;
   code = -1;
}

istream& operator >> (istream& stream, DxfToken& token)
{
   static std::string inputline;
   stream >> inputline;
   token.code = std::stoi(inputline);
   stream >> token.data;
   token.size = inputline.length() + token.data.length() + 2;   // might be missing a few end line characters --- never mind
   return stream;
}

///////////////////////////////////////////////////////////////////////////////
