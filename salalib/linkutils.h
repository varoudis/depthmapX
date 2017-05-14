// Copyright (C) 2017 Petros Koutsolampros

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

#pragma once

#include "mgraph.h"
#include <vector>

namespace depthmapX {
    std::vector<PixelRefPair> getLinksFromMergeLines(const std::vector<Line>& mergeLines, PointMap& currentMap);
    void mergePixelPairs(std::vector<PixelRefPair>& links, PointMap& currentMap);
}
