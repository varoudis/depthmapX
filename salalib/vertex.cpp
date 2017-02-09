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

#include <math.h>
#include <float.h> // _finite support

#include <salalib/mgraph.h>  // <- sala actually includes vertex.h for us

//////////////////////////////////////////////////////////////////////////////

static unsigned int g_nicecolor[] = { 
   0x003333DD, // 0 blue
   0x003388DD, // 1
   0x0022CCDD, // 2
   0x0022CCBB, // 3
   0x0022DD88, // 4
   0x0088DD22, // 5
   0x00BBCC22, // 6
   0x00DDCC22, // 7
   0x00DD8833, // 8
   0x00DD3333, // 9 red
};

// Test a range designed to try to keep consitent saturation and brightness of g_nicecolor, and only move hue
static unsigned int g_nicecolorhsb[] = { 
   0x003333DD, // 0 blue
   0x003377DD, // 1
   0x0033BBDD, // 2
   0x0033DDBB, // 3
   0x0033DD55, // 4
   0x0055DD33, // 5
   0x00BBDD33, // 6
   0x00DDBB33, // 7
   0x00DD7733, // 8
   0x00DD3333, // 9 red
};


static unsigned int g_hsbcolor[] = { 
   0x003333DD, // 0 blue
   0x003388DD, // 1
   0x0022CCDD, // 2
   0x0022CCBB, // 3
   0x0022DD88, // 4
   0x0088DD22, // 5
   0x00BBCC22, // 6
   0x00DDCC22, // 7
   0x00DD8833, // 8
   0x00DD3333, // 9 red
};


static unsigned int g_greyscale[] = { 
   0x00000000, // 0 black
   0x00444444, // 1
   0x00777777, // 2
   0x00AAAAAA, // 3
   0x00CCCCCC, // 4
   0x00EEEEEE, // 5
   0x00FFFFFF, // 6 white
};

static unsigned int g_bluered[] = {
   0x004575B4, // 0 blue
   0x0091BFDB, 
   0x00E0F3F8, 
   0x00FFFFBF, 
   0x00FEE090, 
   0x00FC8D59, 
   0x00D73027  // 6 red 
};

static unsigned int g_purpleorange[] = {
   0x00542788, // 0 purple
   0x00998EC3, // 1
   0x00D8DAEB, // 2
   0x00F7F7F7, // 3
   0x00FEE0B6, // 4
   0x00F1A340, // 5
   0x00B35806  // 6 orange
};

// htmlByte converts a normalised number to an HTML safe byte

unsigned char htmlByte(double colorByte)
{
   // Quick mod - TV
#if defined(_WIN32)   
   return (unsigned char((colorByte + 0.0333) * 15.0) * 0x11);
#else
   return ((unsigned char)((colorByte + 0.0333) * 15.0) * 0x11);
#endif   
}

PafColor& PafColor::makeColor(double field, DisplayParams dp)
{
   // Quick mod - TV
#if defined(_WIN32)   
   if (field == -1.0 || _isnan(field)) {
#else
   if (field == -1.0 || isnan(field)) {
#endif   
      // -1.0 is (currently) a nan value, set alpha channel to 0 (transparent)
      switch (dp.colorscale) {
         case DisplayParams::MONOCHROME:
         case DisplayParams::GREYSCALE:
            m_color = 0x00000000;   // <- monochrome and greyscale, simply hide
            break;
         default:
            // if in colour, then show greyed out:
            m_color = 0x007f7f7f;   // <- grey retained for visibility on certain values
            break;
      }
      return *this;
   }
   if (dp.blue > dp.red) {
      field = 1.0 - field;
      dp.blue = 1.0f - dp.blue;
      dp.red = 1.0f - dp.red;
   }
   if (dp.colorscale == DisplayParams::DEPTHMAPCLASSIC) {
      makeDepthmapClassic(field, dp.blue, dp.red);
   }
   else {
      field = (field - dp.blue) / (dp.red - dp.blue);
      // Quick mod - TV
#if defined(_WIN32)      
      if (_isnan(field)) {
#else
      if (isnan(field)) {
#endif      
         field = 0.5;
      }
      if (field > 1.0) {
         field = 1.0;
      }
      else if (field < 0.0) {
         field = 0.0;
      }
      switch(dp.colorscale) {
      case DisplayParams::AXMANESQUE:
         makeAxmanesque(field);
         break;
      case DisplayParams::PURPLEORANGE:
         makePurpleOrange(field);
         break;
      case DisplayParams::BLUERED:
         makeBlueRed(field);
         break;
      case DisplayParams::GREYSCALE:
      case DisplayParams::MONOCHROME:
         makeGreyScale(field);
         break;
      }
   }
   return *this;
}

// this makes an Axman-like colour range

PafColor& PafColor::makeAxmanesque( double field )
{
   m_color = 0xff000000 | g_nicecolor[int((field - 1e-9) * 10.0)];
   return *this;
}

// this makes a purple-orange scheme that is red-green colour-blind safe

PafColor& PafColor::makePurpleOrange( double field )
{
   m_color = 0xff000000 | g_purpleorange[int((field - 1e-9) * 7.0)];
   return *this;
}

// this makes a blue-red scheme that is red-green colour-blind safe

PafColor& PafColor::makeBlueRed( double field )
{
   m_color = 0xff000000 | g_bluered[int((field - 1e-9) * 7.0)];
   return *this;
}

// this makes a greyscale colour range

PafColor& PafColor::makeGreyScale( double field )
{
   m_color = 0xff000000 | g_greyscale[int((field - 1e-9) * 7.0)];
   return *this;
}

// note, makeDepthmapClassic converts to a safe HTML colour

PafColor& PafColor::makeDepthmapClassic( double field, double blue, double red )
{
   m_color = 0xff000000; // set alpha to 255, solid colour
   double green = blue + (red-blue) / 10.0;
   // NB previously included colour muting: the 1.0 was originally 0.9 to mute the colours slightly
   if (field >= 0.0 && field < blue) {
      setr(htmlByte(0.5 * (blue - field)/blue * 1.0));
      // Quick mod - TV
#if defined(_WIN32)
      setb(unsigned char(0xFF));
#else
      setb((unsigned char)(0xFF));
#endif
   }
   else if (field >= blue && field < (green+blue)/2.0) {
      // Quick mod - TV
#if defined(_WIN32)      
      setb(unsigned char(0xFF));
#else
      setb((unsigned char)(0xFF));
#endif      
      setg(htmlByte((2.0*(field - blue)/(green-blue)) * 1.0));
   }
   else if (field >= (green+blue)/2.0 && field < green) {
      setb(htmlByte((2.0*(green - field)/(green-blue)) * 1.0));
      // Quick mod - TV
#if defined(_WIN32)
      setg(unsigned char(0xFF));
#else
      setg((unsigned char)(0xFF));
#endif      
   }
   else if (field >= green && field < (green+red)/2.0 ) {
      // Quick mod - TV
#if defined(_WIN32)      
      setg(unsigned char(0xFF));
#else
      setg((unsigned char)(0xFF));
#endif      
      setr(htmlByte((2.0*(field - green)/(red-green)) * 1.0));
   }
   else if (field >= (green+red)/2.0 && field < red) {
      setg(htmlByte((2.0*(red - field)/(red-green)) * 1.0));
      // Quick mod - TV
#if defined(_WIN32)      
      setr(unsigned char(0xFF));
#else
      setr((unsigned char)(0xFF));
#endif      
   }
   else if (field >= red) {
      // Quick mod - TV
#if defined(_WIN32)      
      setr(unsigned char(0xFF));
#else
      setr((unsigned char)(0xFF));
#endif      
      setb(htmlByte(0.5 * (field - red)/(1.0 - red) * 1.0));
   }
   return *this;
}


//////////////////////////////////////////////////////////////////////////////

void AttrHeader::reset(AttrVal attributes[])
{
   attributes[NEIGHBOURHOOD_SIZE].intval = -1;
   attributes[GRAPH_SIZE].intval = -1;
   attributes[KERNEL_SIZE].intval = -1;
   attributes[CLIQUE_SIZE].intval = -1;
   attributes[TOTAL_DEPTH].intval = -1;
   attributes[ENTROPY].floatval = -1.0f;
   attributes[REL_ENTROPY].floatval = -1.0f;
   attributes[CLUSTER].floatval = -1.0f;
   attributes[UNUSED].floatval = -1.0f;
   attributes[POINT_DEPTH].intval = -1;
   attributes[CONTROL_HILL].floatval = -1.0f;
   attributes[CONTROL_TURN].floatval = -1.0f;
   attributes[MEDIAN_ANGLE].floatval = -1.0f;
   attributes[FAR_NODE].intval = -1;
   attributes[FAR_DIST].floatval = -1.0f;
   attributes[TOTAL_DIST].floatval = -1.0f;
   attributes[AGENT_COUNT].intval = -1;
   attributes[AGENT_COLL_COUNT].intval = -1;
   attributes[TOTAL_METRIC_DEPTH].floatval = -1.0f;
   attributes[METRIC_GRAPH_SIZE].intval = -1;
   attributes[METRIC_POINT_DEPTH].floatval = -1.0f;
   attributes[TOTAL_EUCLID_DIST].floatval = -1.0f;
   attributes[POINT_EUCLID_DIST].floatval = -1.0f;
   attributes[TOTAL_METRIC_ANGLE].floatval = -1.0f;
   attributes[METRIC_POINT_ANGLE].floatval = -1.0f;
}

#define QUICK_MD (double(attributes[TOTAL_DEPTH].intval) / double(attributes[GRAPH_SIZE].intval - 1))
#define QUICK_METRIC_MD (double(attributes[TOTAL_METRIC_DEPTH].floatval) / double(attributes[METRIC_GRAPH_SIZE].intval - 1))
#define QUICK_METRIC_AMD (double(attributes[TOTAL_METRIC_ANGLE].floatval) / double(attributes[METRIC_GRAPH_SIZE].intval - 1))

double AttrHeader::getAttr(int attr, const AttrVal attributes[]) const
{  
   double val;
   // will eventually put the attributes in a table
   switch (attr) {
   case NEIGHBOURHOOD_SIZE:
      val = double(attributes[NEIGHBOURHOOD_SIZE].intval);
      break;
   case GRAPH_SIZE:
      val = double(attributes[GRAPH_SIZE].intval);
      break;
   case MEAN_DEPTH:
      if (attributes[GRAPH_SIZE].intval > 1) {
         val = QUICK_MD;
      }
      else {
         val = -1.0;
      }
      break;
   case INTEGRATION_RA:
      if (attributes[GRAPH_SIZE].intval > 2) {
         val = 2.0 * (QUICK_MD - 1.0) / double(attributes[GRAPH_SIZE].intval - 2);
      }
      else {
         val = -1.0;
      }
      break;
   case INTEGRATION_RRA:
      if (attributes[GRAPH_SIZE].intval > 2) {
         val = (2.0 * (QUICK_MD - 1.0)) / (double(attributes[GRAPH_SIZE].intval - 2) * dvalue(attributes[GRAPH_SIZE].intval));
      }
      else {
         val = -1.0;
      }
      break;
   case INTEGRATION_HILL:
      if (attributes[GRAPH_SIZE].intval > 2 && attributes[TOTAL_DEPTH].intval > attributes[GRAPH_SIZE].intval) {
         val = (double(attributes[GRAPH_SIZE].intval - 2) * dvalue(attributes[GRAPH_SIZE].intval)) / (2.0 * (QUICK_MD - 1.0));
      }
      else {
         val = -1.0;
      }
      break;
   case INTEGRATION_TEKL:
      if (attributes[GRAPH_SIZE].intval > 2 && attributes[TOTAL_DEPTH].intval > attributes[GRAPH_SIZE].intval) {
         val = ln(double(attributes[GRAPH_SIZE].intval - 2)/2.0) / 
               ln(double(attributes[TOTAL_DEPTH].intval - attributes[GRAPH_SIZE].intval + 1));
      }
      else {
         val = -1.0;
      }
      break;
   case POINT_DEPTH:
      val = attributes[POINT_DEPTH].intval;
      break;
   case AVG_DIST:
      if (attributes[NEIGHBOURHOOD_SIZE].intval > 0) {
         val = attributes[TOTAL_DIST].floatval / double(attributes[NEIGHBOURHOOD_SIZE].intval);
      }
      else {
         val = -1.0;
      }
      break;
   case AGENT_COUNT:
      val = attributes[attr].intval;
      break;
   case AGENT_COLL_COUNT:
      val = attributes[attr].intval;
      break;
   case METRIC_MEAN_DEPTH:
      if (attributes[METRIC_GRAPH_SIZE].intval > 1) {
         val = QUICK_METRIC_MD;
      }
      else {
         val = -1.0;
      }
      break;
   case METRIC_MEAN_ANGLE:
      if (attributes[METRIC_GRAPH_SIZE].intval > 1) {
         val = QUICK_METRIC_AMD;
      }
      else {
         val = -1.0;
      }
      break;
   case DECENTRAL_INTEG:
      if (attributes[TOTAL_METRIC_DEPTH].floatval >= 0.0 && attributes[TOTAL_DEPTH].intval > 0) {
         val = ln(attributes[TOTAL_METRIC_DEPTH].floatval+1.0) / double(attributes[TOTAL_DEPTH].intval);
      }
      else {
         val = -1.0;
      }
      break;
   case MEAN_PENN_DIST:
      if (attributes[METRIC_GRAPH_SIZE].intval > 1) {
         if (attributes[TOTAL_METRIC_DEPTH].floatval > attributes[TOTAL_EUCLID_DIST].floatval) {
            val = double(attributes[TOTAL_METRIC_DEPTH].floatval - attributes[TOTAL_EUCLID_DIST].floatval) 
                  / double(attributes[METRIC_GRAPH_SIZE].intval);
         }
         else {
            val = 0.0f;
         }
      }
      else {
         val = -1.0f;
      }
      break;
   case POINT_PENN_DIST:
      if (attributes[METRIC_POINT_DEPTH].floatval >= 0.0) {
         if (attributes[METRIC_POINT_DEPTH].floatval > attributes[POINT_EUCLID_DIST].floatval) {
            val = attributes[METRIC_POINT_DEPTH].floatval - attributes[POINT_EUCLID_DIST].floatval;
         }
         else {
            val = 0.0f;
         }
      }
      else {
         val = -1.0f;
      }
      break;
   default:
      val = attributes[attr].floatval;
      break;
   }
   return val;
}

///////////////////////////////////////////////////////////////////////////////////

AttrBody::AttrBody( streampos p, const AttrHeader& h ) 
{
   pos = p;
   ref = -1;
   header = (AttrHeader *) &h;

   color = 0.5;
   highlight = false;
   myagent = NULL;

   attributes = new AttrVal[header->m_attr_count];

   header->reset(attributes);
}

AttrBody::AttrBody(const AttrBody& attr)
{
   pos = attr.pos;
   ref = attr.ref;
   origin = attr.origin;
   header = attr.header;

   color = attr.color;
   highlight = attr.highlight;
   myagent = attr.myagent;

   if (attr.attributes) {
      attributes = new AttrVal[header->m_attr_count];
      // fill in with original attributes:
      for (int i = 0; i < header->m_attr_count; i++) {
         attributes[i] = attr.attributes[i];
      }
   }
}

AttrBody::~AttrBody()
{
   if (attributes) {
      delete [] attributes;
      attributes = NULL;
   }
}

ostream& operator << (ostream& stream, const AttrBody& attr)
{
   stream << attr.ref << "\t";
   stream << attr.origin.x << "\t" << attr.origin.y << "\t" << attr.origin.z;

   for (int i = 0; i < NUM_SUMMARISABLE_ATTRIBUTES; i++) {
      if (g_attr_summary_map[i].usable()) {
         if (g_attr_summary_map[i].intval()) {
            stream << "\t" << attr.attributes[g_attr_summary_map[i].ref].intval;
         }
         else {
            stream << "\t" << attr.attributes[g_attr_summary_map[i].ref].floatval;
         }
      }
   }

   return stream;
}

void AttrBody::write( ofstream& stream ) const 
{
   stream.write( (char *) &pos, sizeof(pos) );
   stream.write( (char *) &ref, sizeof(ref) );
   stream.write( (char *) &origin, sizeof(origin) );
   if (attributes) {
      stream.write( (char *) attributes, sizeof(AttrVal) * (header->m_attr_count) );
   }
}

void AttrBody::read( ifstream& stream, int attr_count )
{
   stream.read( (char *) &pos, sizeof(pos) );
   stream.read( (char *) &ref, sizeof(ref) );
   stream.read( (char *) &origin, sizeof(origin) );
   if (attributes) { 
      stream.read( (char *) attributes, sizeof(AttrVal) * attr_count );
   }
}

///////////////////////////////////////////////////////////////////////////////////

bool ArVertexList::openwrite( int nodes )
{
#ifdef _WIN32
   m_stream = new fstream( m_filename.c_str(), ios::binary | ios::out | ios::trunc );
#else
   m_stream = new fstream( m_filename.c_str(), ios::out | ios::trunc );
#endif

   if (m_stream->fail()) {
      if (m_stream->rdbuf()->is_open()) {
         remove();
      }
      if (m_stream) {
         delete m_stream;
         m_stream = NULL;
      }
      return false;
   }

   m_stream->write( "grf", 3 );
   int version = METAGRAPH_VERSION;
   m_stream->write( (char *) &version, sizeof(int) );
   m_stream->write( "v", 1 ); // <- signifies virtual memory section
   m_stream->write( (char *) &nodes, sizeof(nodes) );

   return true;
}

void ArVertexList::openread()
{
#ifdef _WIN32
   m_stream = new fstream( m_filename.c_str(), ios::binary | ios::in );
#else
   m_stream = new fstream( m_filename.c_str(), ios::in );
#endif
}

void ArVertexList::close()
{
   if (m_stream) {
      if (m_stream->rdbuf()->is_open()) {
         m_stream->close(); 
      }
      delete m_stream; m_stream = NULL;
   }
}

void ArVertexList::remove()
{
   close();
   if (!m_filename.empty()) {
      // Quick mod - TV
#if defined(_WIN32)      
      _unlink(m_filename.c_str());
#else
      unlink(m_filename.c_str());
#endif      
      m_filename = pstring();
   }
}

void ArVertexList::add( int ref, const ArVertex& node ) 
{
   m_cache_ref = ref;
   m_cache_data = node;
}

void ArVertexList::commit()                     // Copy
{
   if (m_cache_ref != -1) {
      streampos pos = m_stream->tellp();
      m_attributes[m_cache_ref].pos = pos;
      m_cache_data.write( (ofstream&) *m_stream );
      m_cache_data.clear();
      m_stream->flush();
   }
   m_cache_ref = -1;
}

void ArVertexList::commit(const Point2f& p, int far_node, float far_dist, float total_dist) // Add
{
   if (m_cache_ref != -1) {
      streampos pos = m_stream->tellp();
      AttrBody attr(pos, m_attr_header);
      {
         // a few values we know
         attr.ref = m_cache_ref;
         attr.origin.x = p.x;
         attr.origin.y = p.y;
         attr.intval(AttrHeader::NEIGHBOURHOOD_SIZE) = m_cache_data.size();
         attr.intval(AttrHeader::FAR_NODE) = far_node;
         attr.floatval(AttrHeader::FAR_DIST) = far_dist;
         attr.floatval(AttrHeader::TOTAL_DIST) = total_dist;
      }
      m_attributes.push_back( attr );
      m_cache_data.write( (ofstream&) *m_stream );
      m_cache_data.clear();
      m_stream->flush();
   }
   m_cache_ref = -1;
}


// NOTE: ONLY READ AND WRITE ATTRIBUTES! 
// (At the moment, complex virtual mem stuff is handled at the meta graph level)

bool ArVertexList::read( ifstream& stream, int metagraph_version )
{
   m_metagraph_version = metagraph_version;

   int attr_count;
   stream.read( (char *) &m_which_attributes, sizeof(m_which_attributes) );
   stream.read( (char *) &attr_count, sizeof(int) );

   int size;
   stream.read( (char *) &size, sizeof(int) );

   m_attributes.clear();
   for (int i = 0; i < size; i++) {
      AttrBody attr(-1, m_attr_header);
      attr.read(stream, attr_count);   // <- only write in saved attributes
      m_attributes.push_back( attr );
   }

   m_cache_ref = -1; // just in case... should really set this with virtual mem stuff...

   return true;
}

bool ArVertexList::write( ofstream& stream )
{
   stream.write( (char *) &m_which_attributes, sizeof(m_which_attributes) );
   // I'm phasing this out again for now (attribute header), 
   // I'm thinking the map should really be stored rather than the header,
   // in any case, the only important think about the header is the number of 
   // attributes...
   stream.write( (char *) &m_attr_header.m_attr_count, sizeof(int) ); 

   int size = m_attributes.size();
   stream.write( (char *) &size, sizeof(int) );

   for (size_t i = 0; i < m_attributes.size(); i++) {
      m_attributes[i].write( stream );
   }

   return true;
}
