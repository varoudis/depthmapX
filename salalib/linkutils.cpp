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

#include "linkutils.h"
#include "genlib/exceptions.h"
#include <sstream>

namespace depthmapX {
    std::vector<PixelRefPair> getLinksFromMergeLines(const std::vector<Line>& mergeLines, PointMap& currentMap)
    {
        vector<PixelRefPair> mergePixelPairs;
        for (size_t i = 0; i < mergeLines.size(); i++)
        {
            const Line & mergeLine = mergeLines[i];
            const PixelRef & a = currentMap.pixelate(mergeLine.start());
            const PixelRef & b = currentMap.pixelate(mergeLine.end());

            // check in limits:
            if (!currentMap.includes(a) || !currentMap.getPoint(a).filled()
                    || !currentMap.includes(b) || !currentMap.getPoint(b).filled())
            {
                std::stringstream message;
                message << "Line ends not both on painted analysis space "
                        << i << " ("
                        << mergeLine.start().x << ", "
                        << mergeLine.start().y << " -> "
                        << mergeLine.end().x << ", "
                        << mergeLine.end().y << ")" << flush;
                throw depthmapX::RuntimeException(message.str().c_str());
            }

            // we probably need to check if we were given coordinates that
            // fall on a previously given cell, in which case the newest given
            // will replace the oldest and effectively delete the whole link
            for (size_t j = 0; j < mergePixelPairs.size(); j++)
            {
                // PixelRefPair internal == operator only checks a with a and b with b
                // but we also need to check the inverse
                if(a == mergePixelPairs[j].a
                        || b == mergePixelPairs[j].b
                        || a == mergePixelPairs[j].b
                        || b == mergePixelPairs[j].a)
                {
                    // one of the cells has already been seen.
                    std::stringstream message;
                    message << "Overlapping link found at line "
                            << i << " ("
                            << mergeLine.start().x << ", "
                            << mergeLine.start().y << " -> "
                            << mergeLine.end().x << ", "
                            << mergeLine.end().y << ")" << flush;
                    throw depthmapX::RuntimeException(message.str().c_str());
                }
            }

            // TODO: the merge function will replace any links that already exist
            // on the two locations, so we need to warn the user if this is the case

            mergePixelPairs.push_back(PixelRefPair(a, b));
        }
        return mergePixelPairs;
    }
}
