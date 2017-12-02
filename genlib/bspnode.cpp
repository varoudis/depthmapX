// genlib - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2000-2010 University College London, Alasdair Turner
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

#include "bspnode.h"
#include <stack>

// Binary Space Partition

void BSPNode::make(Communicator *communicator, time_t atime, const std::vector<TaggedLine>& lines, BSPNode *par)
{

    std::stack<BSPNode *> nodeStack;
    std::stack<std::vector<TaggedLine> > leftLinesStack;
    std::stack<std::vector<TaggedLine> > rightLinesStack;

    typedef std::pair<std::vector<TaggedLine>, std::vector<TaggedLine> > TagLineVecPair;
    TagLineVecPair leftRightLines = makeLines(communicator, atime, lines, par);

    parent = par;
    nodeStack.push(this);
    leftLinesStack.push(leftRightLines.first);
    rightLinesStack.push(leftRightLines.second);

    while(nodeStack.size() > 0) {
        BSPNode *currNode = nodeStack.top();
        nodeStack.pop();
        std::vector<TaggedLine> currLeftLines = leftLinesStack.top();
        leftLinesStack.pop();
        std::vector<TaggedLine> currRightLines = rightLinesStack.top();
        rightLinesStack.pop();

        if (currLeftLines.size()) {
           currNode->left = new BSPNode();
           TagLineVecPair leftRightLinesFromLeft = currNode->left->makeLines(communicator,atime,currLeftLines,currNode);
           nodeStack.push(currNode->left);
           leftLinesStack.push(leftRightLinesFromLeft.first);
           rightLinesStack.push(leftRightLinesFromLeft.second);
        }
        if (currRightLines.size()) {
           currNode->right = new BSPNode();
           TagLineVecPair leftRightLinesFromRight = currNode->right->makeLines(communicator,atime,currRightLines,currNode);
           nodeStack.push(currNode->right);
           leftLinesStack.push(leftRightLinesFromRight.first);
           rightLinesStack.push(leftRightLinesFromRight.second);
        }
    }
}

std::pair<std::vector<TaggedLine>, std::vector<TaggedLine> > BSPNode::makeLines(Communicator *communicator,
                                                                        time_t atime,
                                                                        const std::vector<TaggedLine> &lines,
                                                                        BSPNode *par)
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

   std::vector<TaggedLine> leftlines;
   std::vector<TaggedLine> rightlines;

   parent = par;

   // for optimization of the tree (this reduced a six-minute gen time to a 38 second gen time)
   int chosen = -1;
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
            if (line.height() > line.width() && (chosen == -1 || dist(line.midpoint(),midpoint) < chosendist)) {
               chosen = i;
               chosendist = dist(line.midpoint(),midpoint);
            }
         }
         else {
            if (line.width() > line.height() && (chosen == -1 || dist(line.midpoint(),midpoint) < chosendist)) {
               chosen = i;
               chosendist = dist(line.midpoint(),midpoint);
            }
         }
      }
      // argh... and again... there weren't any hoz / ver:
      if (chosen == -1) {
         for (size_t i = 0; i < lines.size(); i++) {
            if (chosen == -1 || dist(lines[i].line.midpoint(),midpoint) < chosendist) {
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

   return std::make_pair(leftlines, rightlines);
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
