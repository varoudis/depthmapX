#pragma once

#include "mgraph.h"
#include <vector>

namespace depthmapX {
    std::vector<PixelRefPair> getLinksFromMergeLines(const std::vector<Line>& mergeLines, PointMap& currentMap);
}
