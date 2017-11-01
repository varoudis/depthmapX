#pragma once

#include "genlib/paftl.h"
#include "p2dpoly.h"

// Binary Space Partition

struct BSPNode
{
public:
   enum { BSPLEFT, BSPRIGHT };
   BSPNode *left;
   BSPNode *right;
   BSPNode *parent;
   Line line;
   int m_tag;
   int m_count;
   //
public:
   BSPNode()
   { left = NULL; right = NULL; parent = NULL; m_count = 0; m_tag = -1; }
   virtual ~BSPNode()
   { if (left) delete left; left = NULL; if (right) delete right; right = NULL; }
   //
   bool isLeaf() {
      return left == NULL && right == NULL;
   }
   void make(Communicator *communicator, time_t atime, const prefvec<TaggedLine>& lines, BSPNode *par);
   int classify(const Point2f& p);
   const Line& getLine() const { return line; }
   const int getTag() const { return m_tag; }
};
