#pragma once

#include "salalib/spacepix.h"

#include "genlib/p2dpoly.h"

// helpers... a class to tidy up ugly maps people may give me...

class TidyLines : public SpacePixel
{
public:
   void tidy(std::vector<Line> &lines, const QtRegion& region);
   void quicktidy(std::map<int, Line> &lines, const QtRegion& region);
};
