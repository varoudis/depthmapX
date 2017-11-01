#include "bspnode.h"

// Binary Space Partition

void BSPNode::make(Communicator *communicator, time_t atime, const prefvec<TaggedLine>& lines, BSPNode *par)
{
   m_count++;

   if (communicator)
   {
      if (communicator->IsCancelled()) {
         throw Communicator::CancelledException();
      }
      if (qtimer( atime, 500 )) {
         communicator->CommPostMessage( Communicator::CURRENT_RECORD, m_count );
      }
   }

   prefvec<TaggedLine> leftlines;
   prefvec<TaggedLine> rightlines;

   parent = par;

   // for optimization of the tree (this reduced a six-minute gen time to a 38 second gen time)
   size_t chosen = paftl::npos;
   if (lines.size() > 3) {
      size_t i;
      Point2f midpoint;
      for (i = 0; i < lines.size(); i++) {
         midpoint += lines[i].line.start() + lines[i].line.end();
      }
      midpoint /= 2.0 * lines.size();
      bool ver = true;
      if (par && par->line.height() > par->line.width()) {
         ver = false;
      }
      double chosendist = -1.0;
      for (i = 0; i < lines.size(); i++) {
         const Line& line = lines[i].line;
         if (ver) {
            if (line.height() > line.width() && (chosen == paftl::npos || dist(line.midpoint(),midpoint) < chosendist)) {
               chosen = i;
               chosendist = dist(line.midpoint(),midpoint);
            }
         }
         else {
            if (line.width() > line.height() && (chosen == paftl::npos || dist(line.midpoint(),midpoint) < chosendist)) {
               chosen = i;
               chosendist = dist(line.midpoint(),midpoint);
            }
         }
      }
      // argh... and again... there weren't any hoz / ver:
      if (chosen == paftl::npos) {
         for (size_t i = 0; i < lines.size(); i++) {
            if (chosen == paftl::npos || dist(lines[i].line.midpoint(),midpoint) < chosendist) {
               chosen = i;
               chosendist = dist(lines[i].line.midpoint(),midpoint);
            }
         }
      }
   }
   else {
      chosen = pafrand() % lines.size();
   }

   line = lines[chosen].line;
   m_tag = lines[chosen].tag;

   Point2f v0 = line.end() - line.start();
   v0.normalise();

   for (size_t i = 0; i < lines.size(); i++) {
      if (i == chosen) {
         continue;
      }
      const Line& testline = lines[i].line;
      int tag = lines[i].tag;
      Point2f v1 = testline.start()-line.start();
      v1.normalise();
      Point2f v2 = testline.end()-line.start();
      v2.normalise();
      // should use approxeq here:
      double a = testline.start() == line.start() ? 0 : det(v0,v1);
      double b = testline.end() == line.start() ? 0 : det(v0,v2);
      // note sure what to do if a == 0 and b == 0 (i.e., it's parallel... this test at least ensures on the line is one or the other side)
      if (a >= 0 && b >= 0) {
         leftlines.push_back(TaggedLine(testline,tag));
      }
      else if (a <= 0 && b <= 0) {
         rightlines.push_back(TaggedLine(testline,tag));
      }
      else {
         Point2f p = intersection_point(line,testline);
         Line x = Line(testline.start(),p);
         Line y = Line(p,testline.end());
         if (a >= 0) {
            if (x.length() > 0.0) // should use a tolerance here too
               leftlines.push_back(TaggedLine(x,tag));
            if (y.length() > 0.0) // should use a tolerance here too
               rightlines.push_back(TaggedLine(y,tag));
         }
         else {
            if (x.length() > 0.0) // should use a tolerance here too
               rightlines.push_back(TaggedLine(x,tag));
            if (y.length() > 0.0) // should use a tolerance here too
               leftlines.push_back(TaggedLine(y,tag));
         }
      }
   }

   if (leftlines.size()) {
      left = new BSPNode();
      left->make(communicator,atime,leftlines,this);
   }
   if (rightlines.size()) {
      right = new BSPNode();
      right->make(communicator,atime,rightlines,this);
   }
}

int BSPNode::classify(const Point2f& p)
{
   Point2f v0 = line.end() - line.start();
   v0.normalise();
   Point2f v1 = p - line.start();
   v1.normalise();
   if (det(v0,v1) >= 0) {
      return BSPLEFT;
   }
   else {
      return BSPRIGHT;
   }
}
