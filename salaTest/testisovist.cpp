// Copyright (C) 2020 Petros Koutsolampros

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

#include "catch.hpp"
#include "salalib/mgraph.h"

TEST_CASE("Simple Isovist") {

    const float EPSILON = 0.001;

    // simple plan for isovist. dot as the origin
    //  _ _ _ _
    // |     . |
    // |    _ _|
    // |   |
    // |_ _|

    std::vector<Line> planLines = {
        Line(Point2f(1, 1), Point2f(1, 3)), //
        Line(Point2f(1, 3), Point2f(3, 3)), //
        Line(Point2f(3, 3), Point2f(3, 2)), //
        Line(Point2f(3, 2), Point2f(2, 2)), //
        Line(Point2f(2, 2), Point2f(2, 1)), //
        Line(Point2f(2, 1), Point2f(1, 1))  //
    };

    Point2f isovistOrigin(2.5, 2.5);

    std::unique_ptr<MetaGraph> metaGraph(new MetaGraph("Test MetaGraph"));
    metaGraph->m_drawingFiles.emplace_back("Test SpacePixelGroup");
    metaGraph->m_drawingFiles.back().m_spacePixels.emplace_back("Test ShapeMap");
    for (Line &line : planLines) {
        metaGraph->m_drawingFiles.back().m_spacePixels.back().makeLineShape(line);
    }

    SECTION("With a communicator") {
        std::unique_ptr<Communicator> comm(new ICommunicator);
        metaGraph->makeIsovist(comm.get(), isovistOrigin, 0, 0, false);
    }
    SECTION("Without a communicator") { metaGraph->makeIsovist(nullptr, isovistOrigin, 0, 0, false); }

    SalaShape &isovist = metaGraph->getDataMaps().front().getAllShapes().begin()->second;

    REQUIRE(isovist.isClosed());
    REQUIRE(isovist.isPolygon());


    // TODO: The current implementation generates a polygon of 12 points, potentially
    // because it takes them directly from the isovist gaps. This isovist only really
    // needs 5 points so it might make sense to run some sort of optimisation right
    // after generating the isovists

    REQUIRE(isovist.m_points.size() == 12);

    REQUIRE(isovist.m_points[0].x == Approx(3.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[0].y == Approx(3.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[1].x == Approx(2.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[1].y == Approx(3.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[2].x == Approx(1.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[2].y == Approx(3.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[3].x == Approx(1.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[3].y == Approx(3.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[4].x == Approx(1.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[4].y == Approx(1.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[5].x == Approx(2.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[5].y == Approx(2.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[6].x == Approx(2.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[6].y == Approx(2.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[7].x == Approx(2.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[7].y == Approx(2.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[8].x == Approx(3.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[8].y == Approx(2.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[9].x == Approx(3.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[9].y == Approx(2.0).epsilon(EPSILON));

    REQUIRE(isovist.m_points[10].x == Approx(3.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[10].y == Approx(2.5).epsilon(EPSILON));

    REQUIRE(isovist.m_points[11].x == Approx(3.0).epsilon(EPSILON));
    REQUIRE(isovist.m_points[11].y == Approx(2.5).epsilon(EPSILON));
}
