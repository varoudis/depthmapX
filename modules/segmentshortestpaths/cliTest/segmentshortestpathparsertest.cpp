// Copyright (C) 2026 Tasos Varoudis
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

#include "cliTest/argumentholder.h"
#include "cliTest/selfcleaningfile.h"
#include "modules/segmentshortestpaths/cli/segmentshortestpathparser.h"
#include <catch.hpp>

#include <sstream>

TEST_CASE("SegmentShortestPathParser", "Error cases") {
    SECTION("Missing argument to -sspo") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspd", "0,0", "-sspo"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), Catch::Contains("-sspo requires an argument"));
    }

    SECTION("Missing argument to -sspd") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspo", "0,0", "-sspd"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), Catch::Contains("-sspd requires an argument"));
    }

    SECTION("Missing argument to -sspt") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspo", "0,0", "-sspd", "0,0", "-sspt"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), Catch::Contains("-sspt requires an argument"));
    }

    SECTION("rubbish input to -sspo") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspd", "0,0", "-sspo", "foo"};
        REQUIRE_THROWS_WITH(
            parser.parse(ah.argc(), ah.argv()),
            Catch::Contains("Invalid origin point provided (foo). Should only contain digits dots and commas"));
    }

    SECTION("rubbish input to -sspd") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspo", "0,0", "-sspd", "foo"};
        REQUIRE_THROWS_WITH(
            parser.parse(ah.argc(), ah.argv()),
            Catch::Contains("Invalid destination point provided (foo). Should only contain digits dots and commas"));
    }

    SECTION("rubbish input to -sspt") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog", "-sspo", "0,0", "-sspd", "0,0", "-sspt", "foo"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), Catch::Contains("Invalid step type: foo"));
    }

    SECTION("Neiter points nor point file provided") {
        SegmentShortestPathParser parser;
        ArgumentHolder ah{"prog"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()),
                            Catch::Contains("Both -sspo and -sspd must be provided"));
    }
}

TEST_CASE("Successful SegmentShortestPathParser", "Read successfully") {
    SegmentShortestPathParser parser;
    double originX = 1.0;
    double originY = 2.0;
    double destinationX = 1.1;
    double destinationY = 1.2;

    SECTION("Read from commandline") {
        std::stringstream originStream;
        originStream << originX << "," << originY << std::flush;
        std::stringstream destinationStream;
        destinationStream << destinationX << "," << destinationY << std::flush;

        ArgumentHolder ah{"prog",  "-sspo",      originStream.str(), "-sspd", destinationStream.str(),
                          "-sspt", "topological"};
        parser.parse(ah.argc(), ah.argv());
    }

    auto originPoint = parser.getShortestPathOrigin();
    auto destinationPoint = parser.getShortestPathDestination();
    REQUIRE(originPoint.x == Approx(originX));
    REQUIRE(originPoint.y == Approx(originY));
    REQUIRE(destinationPoint.x == Approx(destinationX));
    REQUIRE(destinationPoint.y == Approx(destinationY));
}
