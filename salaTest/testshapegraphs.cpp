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

#include "../genlib/p2dpoly.h"
#include "../salalib/mgraph.h"
#include "../salalib/shapemap.h"
#include "../salalib/axialmap.h"
#include "catch.hpp"
#include <sstream>
#include <iostream>

TEST_CASE("Testing ShapeGraph::writeAxialConnections"){

    Point2f line1Start(0,0);
    Point2f line1End  (3,0);
    Point2f line2Start(1,1);
    Point2f line2End  (1,-1);
    Point2f line3Start(2,1);
    Point2f line3End  (2,-2);

    std::unique_ptr<SuperSpacePixel> spacePixel(new SuperSpacePixel("Test SuperSpacePixel"));

    spacePixel->push_back(SpacePixelFile("Test SpacePixelGroup"));
    spacePixel->tail().push_back(ShapeMap("Test ShapeMap"));

    spacePixel->tail().tail().makeLineShape(Line(line1Start, line1End));
    spacePixel->tail().tail().makeLineShape(Line(line2Start, line2End));
    spacePixel->tail().tail().makeLineShape(Line(line3Start, line3End));

    std::unique_ptr<ShapeGraphs> shapeGraphs(new ShapeGraphs());
    shapeGraphs->convertDrawingToAxial(0, "Test axial", (*spacePixel));
    ShapeGraph &displayedShapeGraph = shapeGraphs->getDisplayedMap();

    SECTION("writeAxialConnectionsAsDotGraph") {
        std::stringstream stream;
        displayedShapeGraph.writeAxialConnectionsAsDotGraph(stream);

        REQUIRE(stream.good());
        char line[1000];
        std::vector<std::string> lines;
        while( !stream.eof())
        {
            stream.getline(line, 1000);
            lines.push_back(line);
        }
        std::vector<std::string> expected{ "strict graph {", "    0 -- 1", "    0 -- 2",
                                           "    1 -- 0", "    2 -- 0", "}", "" };
        REQUIRE(lines == expected);
    }
    SECTION("writeAxialConnectionsAsPairsCSV") {
        std::stringstream stream;
        displayedShapeGraph.writeAxialConnectionsAsPairsCSV(stream);

        REQUIRE(stream.good());
        char line[1000];
        std::vector<std::string> lines;
        while( !stream.eof())
        {
            stream.getline(line, 1000);
            lines.push_back(line);
        }
        std::vector<std::string> expected{ "refA,refB", "0,1", "0,2", "1,0", "2,0" };
        REQUIRE(lines == expected);
    }
}
TEST_CASE("Testing ShapeGraph::writeSegmentConnections")
{
    // As we are converting the drawing directly to segments
    // the lines need to touch, not cross

    Point2f line1Start(1,1);
    Point2f line1End  (1,0);
    Point2f line2Start(1,0);
    Point2f line2End  (2,0);
    Point2f line3Start(2,0);
    Point2f line3End  (2,2);

    std::unique_ptr<SuperSpacePixel> spacePixel(new SuperSpacePixel("Test SuperSpacePixel"));

    spacePixel->push_back(SpacePixelFile("Test SpacePixelGroup"));
    spacePixel->tail().push_back(ShapeMap("Test ShapeMap"));

    spacePixel->tail().tail().makeLineShape(Line(line1Start, line1End));
    spacePixel->tail().tail().makeLineShape(Line(line2Start, line2End));
    spacePixel->tail().tail().makeLineShape(Line(line3Start, line3End));

    std::unique_ptr<ShapeGraphs> shapeGraphs(new ShapeGraphs());
    shapeGraphs->convertDrawingToSegment(0, "Test segment", (*spacePixel));
    ShapeGraph &displayedShapeGraph = shapeGraphs->getDisplayedMap();

    SECTION("writeSegmentConnectionsAsPairsCSV") {
        std::stringstream stream;
        displayedShapeGraph.writeSegmentConnectionsAsPairsCSV(stream);

        REQUIRE(stream.good());
        char line[1000];
        std::vector<std::string> lines;
        while( !stream.eof())
        {
            stream.getline(line, 1000);
            lines.push_back(line);
        }
        std::vector<std::string> expected{ "refA,refB,ss_weight,for_back,dir", "0,1,1,0,1", "1,2,1,0,1", "1,0,1,1,-1",
                                           "2,1,1,1,-1" };
        REQUIRE(lines == expected);
    }
}
