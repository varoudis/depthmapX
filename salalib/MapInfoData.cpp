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



#include <salalib/mgraph.h>
#include <salalib/attributes.h>
#include <salalib/shapemap.h>
#include <salalib/axialmap.h>
#include "MapInfoData.h"

///////////////////////////////////////////////////////////////////////

// A typical MIF

/*
Version 300
Charset "WindowsLatin1"
Delimiter ","
Index 1,2
CoordSys Earth Projection 8, 79, "m", -2, 49, 0.9996012717, 400000, -100000 Bounds (-7845061.1011, -15524202.1641) (8645061.1011, 4470074.53373)
Columns 2
  ID Integer
  Length_m Float
Data

Line 534014.29 182533.33 535008.52 182764.11
    Pen (1,2,0) 
Line 533798.68 183094.69 534365.48 183159.01
    Pen (1,2,0)
...etc...
Point 534014.29 182533.33
    Symbol (34,0,12) 
Point 533798.68 183094.69
    Symbol (34,0,12) 
Point 534365.48 183159.01
    Symbol (34,0,12) 
...etc...
*/

// A Typical MID

/*
1,1017.81
2,568.795
3,216.026
*/


///////////////////////////////////////////////////////////////////////////////////

int MapInfoData::import(istream& miffile, istream& midfile, ShapeMap& map)
{
   int retvar = MINFO_OK;

   // read the header...
   if (!readheader(miffile)) {
      return MINFO_HEADER;
   }

   pvecstring columnheads;

   AttributeTable& table = map.getAttributeTable();

   // read mif table
   if (!readcolumnheaders(miffile,midfile,columnheads)) {
      return MINFO_TABLE;
   }

   // set up a list of readable columns from the headers:
   // 
   pvecstring colnames;
   pvecint readable, colindexes;
   size_t i;
   for (i = 0; i < columnheads.size(); i++) {
      columnheads[i].ltrim();
      pvecstring tokens = columnheads[i].tokenize(' ',true);
      if (compare(tokens[1],"Integer",7) || compare(tokens[1],"Smallint",8) || compare(tokens[1],"Decimal",7) || compare(tokens[1],"Float",5)) {
         colnames.push_back(tokens[0]);
         table.insertColumn(colnames.tail());
         readable.push_back(i);
      }
   }

   for (i = 0; i < colnames.size(); i++) {
      colindexes.push_back(table.getColumnIndex(colnames[i]));
   }

   pstring textline;
   prefvec<pvecpoint> pointsets;
   pvecint duplicates;
   pvecint types;

   try {
   // now read line data into the axial map   
   while (!miffile.eof()) {
      miffile >> textline;
      textline.ltrim();
      textline.makelower();
      if (textline.empty()) {
         continue;
      }
      if (compare(textline,"point",5)) {
         pvecstring tokens = textline.tokenize(' ',true);
         pointsets.push_back(pvecpoint());
         types.push_back(SalaShape::SHAPE_POINT);
         pointsets.tail().push_back(Point2f(tokens[1].c_double(),tokens[2].c_double()));
      }
      if (compare(textline,"line",4)) {
         pvecstring tokens = textline.tokenize(' ',true);
         pointsets.push_back(pvecpoint());
         types.push_back(SalaShape::SHAPE_LINE);
         pointsets.tail().push_back(Point2f(tokens[1].c_double(),tokens[2].c_double()));
         pointsets.tail().push_back(Point2f(tokens[3].c_double(),tokens[4].c_double()));
      }
      else if (compare(textline,"pline",5) || compare(textline,"region",6)) {
         int type = compare(textline,"pline",5) ? SalaShape::SHAPE_POLY : (SalaShape::SHAPE_POLY | SalaShape::SHAPE_CLOSED);
         // note: polylines, even multiple lines, are condensed into a single line
         pvecstring tokens = textline.tokenize(' ',true);
         int multiple = 1;
         if (tokens.size() > 1) {
            if (tokens[1] == "multiple") {
               multiple = tokens[2].c_int();
            }
            else if (type & SalaShape::SHAPE_CLOSED) {
               multiple = tokens[1].c_int();
            }
            // if for some reason c_int fails:
            if (multiple == 0) {
               multiple = 1;
            }
         }
         for (int i = 0; i < multiple; i++) {
            int count = -1;
            if ((type & SalaShape::SHAPE_CLOSED) != SalaShape::SHAPE_CLOSED && tokens.size() == 2) {
               // token 2 can apparently be used for count in pline rather than a newline being used...
               count = tokens[1].c_int();
            }
            else {
               miffile >> textline;
               textline.ltrim();
               count = textline.c_int();
            }
            pointsets.push_back(pvecpoint());
            types.push_back(type);
            for (int j = 0; j < count; j++) {
               miffile >> textline;
               textline.ltrim();
               pvecstring tokens = textline.tokenize(' ',true);
               pointsets.tail().push_back(Point2f(tokens[0].c_double(),tokens[1].c_double()));
            }
            if (i != 0) {
               // warn about extraneous pline data
               retvar = MINFO_MULTIPLE;
               duplicates.push_back(pointsets.size() - 1);
            }
         }
      }
   }
   }
   catch (pexception) {
      // unhandled parsing exceptions return read error:
      return MINFO_MIFPARSE;
   }

   size_t nextduplicate = 0;
   int lastrow = -1;

   QtRegion region(pointsets[0][0],pointsets[0][0]);
   for (i = 0; i < pointsets.size(); i++) {
      for (size_t j = 0; j < pointsets[i].size(); j++) {
         region.encompass(pointsets[i][j]);
      }
   }

   try {
     // switch lines into our format
     map.init(pointsets.size(),region);
     for (size_t i = 0; i < pointsets.size(); i++) {
         bool open = false;
         if ((types[i] & SalaShape::SHAPE_CLOSED) == 0) {
            open = true;
         }
         map.makePolyShape(pointsets[i],open);
         int row = table.getRowCount() - 1;
         //
         // table data entries:
         if (nextduplicate < duplicates.size() && duplicates[nextduplicate] == i) {
            // duplicate last row:
            for (size_t i = 0; i < colindexes.size(); i++) {
               table.setValue(row,colindexes[i],table.getValue(lastrow,colindexes[i]));
            }
            nextduplicate++;
         }
         else {
            // read next row:
            pstring line;
            while (!midfile.eof() && line.empty()) {
               midfile >> line;
            }
            if (line.empty()) {
               return MINFO_OBJROWS;
            }
            bool instring = false;
            size_t here = 0, first = 0, reading = 0, nextreadable = 0;
            while (nextreadable < readable.size()) {
               char next = line[here];
               if (next == '\"') {
                  instring = !instring;
               }
               here++;
               if ((!instring && next == m_delimiter) || here >= line.length()) {
                  int length = (here < line.length()) ? here-first-1 : here-first;
                  pstring field = line.substr(first,length);
                  first = here;
                  if (reading == readable[nextreadable]) {
                     float val = (float) field.c_double();
                     table.setValue(row,colindexes[nextreadable],val);
                     nextreadable++;
                  }
                  reading++;
               }
            }
         }
         lastrow = row;
      }
   }
   catch (pexception) {
      // unhandled parsing exceptions return read error:
      return MINFO_TABLE;
   }

   return retvar;
}
/*
bool MapInfoData::exportFile(ostream& miffile, ostream& midfile, const ShapeGraph& map)
{
   // if bounds has not been filled in, fill it in
   if (m_bounds.empty()) {
      char bounds[256];
      sprintf(bounds,"Bounds (%10f, %10f) (%10f, %10f)", map.m_region.bottom_left.x, 
                                                         map.m_region.bottom_left.y,
                                                         map.m_region.top_right.x,
                                                         map.m_region.top_right.y);
      m_bounds = bounds;
   }

   // write the header...
   writeheader(miffile);

   // write the mif table
   writetable(miffile,midfile,map.m_attributes);

   miffile.precision(16);

   for (int i = 0; i < map.m_lines.size(); i++) {
      miffile << "Line " << map.m_lines[i].line.start().x << " " 
                         << map.m_lines[i].line.start().y << " " 
                         << map.m_lines[i].line.end().x << " " 
                         << map.m_lines[i].line.end().y << endl;
      miffile << "    Pen (1,2,0)" << endl; 
   }

   return true;
}
*/
bool MapInfoData::exportFile(ostream& miffile, ostream& midfile, const PointMap& points)
{
   // if bounds has not been filled in, fill it in
   if (m_bounds.empty()) {
      char bounds[256];
      sprintf(bounds,"Bounds (%10f, %10f) (%10f, %10f)", points.m_region.bottom_left.x, 
                                                         points.m_region.bottom_left.y,
                                                         points.m_region.top_right.x,
                                                         points.m_region.top_right.y);
      m_bounds = bounds;
   }

   // write the header...
   writeheader(miffile);
      
   // write the mif table
   writetable(miffile,midfile,points.m_attributes);

   miffile.precision(16);

   for (int i = 0; i < points.m_attributes.getRowCount(); i++) {
      PixelRef pix = points.m_attributes.getRowKey(i);
      Point2f p = points.depixelate(pix);
      miffile << "Point " << p.x << " " << p.y << endl;
      miffile << "    Symbol (32,0,10)" << endl;
   }

   return true;
}

bool MapInfoData::exportFile(ostream& miffile, ostream& midfile, const ShapeMap& map)
{
   // if bounds has not been filled in, fill it in
   if (m_bounds.empty()) {
      char bounds[256];
      sprintf(bounds,"Bounds (%10f, %10f) (%10f, %10f)", map.getRegion().bottom_left.x, 
                                                         map.getRegion().bottom_left.y,
                                                         map.getRegion().top_right.x,
                                                         map.getRegion().top_right.y);
      m_bounds = bounds;
   }

   // write the header...
   writeheader(miffile);

   // write the mid table
   writetable(miffile,midfile,map.m_attributes);

   miffile.precision(16);

   for (size_t i = 0; i < map.m_shapes.size(); i++) {
      // note, attributes must align for this:
      if (map.getAttributeTable().isVisible(i)) {
         const SalaShape& poly = map.m_shapes[i];
         if (poly.isPoint()) {
            miffile << "POINT " << poly.getPoint().x << " " << poly.getPoint().y << endl;
            miffile << "    SYMBOL (32,0,10)" << endl;
         }
         else if (poly.isLine()) {
            miffile << "LINE " << poly.getLine().start().x << " " 
                               << poly.getLine().start().y << " " 
                               << poly.getLine().end().x << " " 
                               << poly.getLine().end().y << endl;
            miffile << "    PEN (1,2,0)" << endl; 
         }
         else if (poly.isPolyLine()) {
            miffile << "PLINE" << endl;
            miffile << "  " << poly.size() << endl; 
            for (size_t k = 0; k < poly.size(); k++) {
               miffile << poly[k].x << " " << poly[k].y << endl;
            }
            miffile << "    PEN (1,2,0)" << endl; 
         }
         else if (poly.isPolygon()) {
            miffile << "REGION  1" << endl;
            miffile << "  " << poly.size() + 1 << endl; 
            for (size_t k = 0; k < poly.size(); k++) {
               miffile << poly[k].x << " " << poly[k].y << endl;
            }
            miffile << poly[0].x << " " << poly[0].y << endl;
            miffile << "    PEN (1,2,0)" << endl; 
            miffile << "    BRUSH (2,16777215,16777215)" << endl; 
            miffile << "    CENTER " << poly.getCentroid().x << " " << poly.getCentroid().y << endl;
         }
      }
   }

   return true;
}

bool MapInfoData::exportPolygons(ostream& miffile, ostream& midfile, const prefvec<pvecpoint>& polygons, const QtRegion& region)
{
   // if bounds has not been filled in, fill it in
   if (m_bounds.empty()) {
      char bounds[256];
      sprintf(bounds,"Bounds (%10f, %10f) (%10f, %10f)", region.bottom_left.x, 
                                                         region.bottom_left.y,
                                                         region.top_right.x,
                                                         region.top_right.y);
      m_bounds = bounds;
   }

   // write the header...
   writeheader(miffile);

   // dummy attributes table:
   AttributeTable attributes;
   for (size_t i = 0; i < polygons.size(); i++) {
      attributes.insertRow(i);
   }

   // write the mid table
   writetable(miffile,midfile,attributes);

   miffile.precision(16);
   for (size_t j = 0; j < polygons.size(); j++) {
      Point2f centre;
      miffile << "QtRegion  1" << endl;
      miffile << "  " << polygons[j].size() + 1 << endl; 
      for (size_t k = 0; k < polygons[j].size(); k++) {
         centre += polygons[j][k];
         miffile << polygons[j][k].x << " " << polygons[j][k].y << endl;
      }
      miffile << polygons[j][0].x << " " << polygons[j][0].y << endl;
      miffile << "    Pen (1,2,0)" << endl; 
      miffile << "    Brush (2,16777215,16777215)" << endl; 
      centre /= polygons[j].size();
      miffile << "    Center " << centre.x << " " << centre.y << endl;
   }

   return true;
}


///////////////////////////////////////////////////////////////////////

MapInfoData::MapInfoData()
{
   m_version = "Version 300";
   m_charset = "Charset \"WindowsLatin1\"";
   m_delimiter = ',';
   m_index = "Index 1";
   m_coordsys = "CoordSys NonEarth Units \"m\" ";
   // note: m_bounds is filled in later
}

bool MapInfoData::readheader(istream& miffile)
{
   pstring line;

   miffile >> m_version;
   miffile >> m_charset;
   m_charset.makeinitcaps();
   // this should read "Charset..." but some files have delimiter straight away...
   if (compare(m_charset,"Delimiter",9)) {
      line = m_charset;
      m_charset = "Charset \"WindowsLatin1\"";
   }
   else {
      miffile >> line;
   }
   size_t index = line.findindex("\"");
   if (index == paftl::npos) {
      return false;
   }
   m_delimiter = line[index+1];
   miffile >> line;
   line.makeinitcaps();
   while (compare(line,"Index",5) || compare(line,"Unique",6)) {
      m_index = line;
      miffile >> line;
   }

   line.ltrim();
   line.makeinitcaps();
   if (compare(line,"Coordsys",8)) {
      line[5] = 'S'; // set back to CoordSys
      // coordsys and bounds together in one line
      m_coordsys = line.splice("Bounds");
      m_bounds = pstring("Bounds") + line;
   }
   else {
      return false;
   }

   return true;
}

bool MapInfoData::readcolumnheaders(istream& miffile, istream& midfile, pvecstring& columnheads)
{
   pstring line;

   miffile >> line;
   line.makeinitcaps();
   size_t colplace = line.findindex("Columns");
   if (colplace == paftl::npos) {
      return false;
   }
   pstring temp = line.splice(' ');
   int cols = line.c_int();

   for (int i = 0; i < cols; i++) {
      miffile >> line;
      line.makeinitcaps();
      columnheads.push_back(line);
   }

   miffile >> line;
   line.makeinitcaps();
   if (line != "Data") {
      return false;
   }

   return true;
}

void MapInfoData::writeheader(ostream& miffile)
{
   miffile << m_version << endl;
   miffile << m_charset << endl;
   miffile << "Delimiter \"" << m_delimiter << "\"" << endl;
   miffile << m_index << endl;
   miffile << m_coordsys;
   miffile << m_bounds << endl;
}

// note: stopped using m_table and m_columnheads as of VERSION_MAPINFO_SHAPES
// simply hack up the table now for own purposes

void MapInfoData::writetable(ostream& miffile, ostream& midfile, const AttributeTable& attributes)
{
   miffile << "Columns " << attributes.getColumnCount() + 1 << endl;
   /*
   miffile << "Columns " << m_columnheads.size() + 1 + attributes.getColumnCount() << endl;

   for (int i = 0; i < m_columnheads.size(); i++) {
      miffile << m_columnheads[i] << endl;
   }
   */
   miffile << "  Depthmap_Ref Integer" << endl;

   for (int j = 0; j < attributes.getColumnCount(); j++) {
      pstring colname = attributes.getColumnName(j);
      miffile << "  ";
      bool lastalpha = false;
      for (size_t i = 0; i < colname.length(); i++) {
         // get rid of any character that's not alphanumeric:
         if (isalnum(colname[i])) {
            miffile << colname[i];
            lastalpha = true;
         }
         else if (lastalpha) {
            miffile << "_";
            lastalpha = false;
         }
      }
      miffile << " Float" << endl;
   }

   miffile << "Data" << endl << endl;

   for (int k = 0; k < attributes.getRowCount(); k++) {
      /*
      if (k < m_table.size()) {
         midfile << m_table[k] << m_delimiter;
      }
      */
      if (attributes.isVisible(k)) {
         midfile << attributes.getRowKey(k);
         // note: outputRow prefixes delimiter, so no delimiter necessary first
         attributes.outputRow( k, midfile, m_delimiter );
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

istream& MapInfoData::read(istream& stream, int version)
{
   m_version.read(stream);
   m_charset.read(stream);
   m_delimiter = stream.get();
   m_index.read(stream);
   m_coordsys.read(stream);
   m_bounds.read(stream);
   //
   // this is no longer used: just a dummy read:
   if (version < VERSION_MAPINFO_SHAPES) {
      int columns, rows;
      pstring temp;
      pvecstring columnheads;
      pvecstring table;
      stream.read((char *) &columns, sizeof(int));
      for (int i = 0; i < columns; i++) {
         temp.read(stream);
         columnheads.push_back(temp);
      }
      stream.read((char *) &rows, sizeof(int));
      for (int j = 0; j < rows; j++) {
         temp.read(stream);
         table.push_back(temp);
      }
   }
   
   return stream;
}

ostream& MapInfoData::write(ostream& stream)
{
   m_version.write(stream);
   m_charset.write(stream);
   stream.put(m_delimiter);
   m_index.write(stream);
   m_coordsys.write(stream);
   m_bounds.write(stream);
   /*
   // No longer used as of VERSION_MAPINFO_SHAPES
   int columns = m_columnheads.size();
   int rows = m_table.size();
   stream.write((char *)&columns, sizeof(columns));
   for (int i = 0; i < m_columnheads.size(); i++) {
      m_columnheads[i].write(stream);
   }
   stream.write((char *)&rows, sizeof(rows));
   for (int j = 0; j < m_table.size(); j++) {
      m_table[j].write(stream);
   }
   */
   return stream;
}
