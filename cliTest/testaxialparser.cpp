// Copyright (C) 2017 Christian Sailer

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

#include "../depthmapXcli/axialparser.h"
#include "argumentholder.h"

TEST_CASE("Test mode and help")
{
    AxialParser parser;
    REQUIRE(parser.getModeName() == "AXIAL");
    REQUIRE(parser.getHelp() == "Mode options for Axial Analysis:\n"\
                                "  -xl <x>,<y> Calculate all lines map from this seed point (can be used more than once)\n"
                                "  -xf Calculate fewest lines map from all lines map\n"\
                                "  -xa <radius/list of radii> run axial anlysis with specified radii\n"\
                                " All modes expect to find the required input in the in graph\n"\
                                " Any combination of flags above can be specified, they will always be run in the order -aa -af -au -ax\n"\
                                " Further flags for axial analysis are:\n"\
                                "   -xac Include choice (betweenness)\n"\
                                "   -xal Include local measures\n"\
                                "   -xar Include RA, RRA and total depth\n"\
                                "   -xaw <map attribute name> perform weighted analysis using this attribute\n"\
                                "\n");

}

TEST_CASE("Test Parsing Exceptions","")
{
    AxialParser parser;
    SECTION("No axial mode")
    {
        ArgumentHolder ah{"prog"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), "No axial analysis mode present" );
    }

    SECTION("Argument missing")
    {
        ArgumentHolder ah{"prog", "-xl"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), "-xl requires an argument" );
    }
}

TEST_CASE("Test mode parsing", "")
{
    AxialParser parser;
    SECTION("All lines")
    {
        ArgumentHolder ah{"prog", "-xl", "1.2,1.5"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE(parser.runAllLines());
        REQUIRE(parser.getAllAxesRoots().size() == 1);
        REQUIRE(parser.getAllAxesRoots()[0].x == Approx(1.2));
        REQUIRE(parser.getAllAxesRoots()[0].y == Approx(1.5));
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }
    SECTION("Fewest lines")
    {
        ArgumentHolder ah{"prog", "-xf"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }
    SECTION("Analysis")
    {
        ArgumentHolder ah{"prog", "-xa", "n"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
        REQUIRE_FALSE(parser.calculateRRA());
        REQUIRE_FALSE(parser.useChoice());
        REQUIRE_FALSE(parser.useLocal());
    }
    SECTION("Analysis -rra")
    {
        ArgumentHolder ah{"prog", "-xa", "n", "-xar"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
        REQUIRE(parser.calculateRRA());
        REQUIRE_FALSE(parser.useChoice());
        REQUIRE_FALSE(parser.useLocal());
    }
    SECTION("Analysis + choice")
    {
        ArgumentHolder ah{"prog", "-xa", "n", "-xac"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
        REQUIRE_FALSE(parser.calculateRRA());
        REQUIRE(parser.useChoice());
        REQUIRE_FALSE(parser.useLocal());
    }
    SECTION("Analysis + local")
    {
        ArgumentHolder ah{"prog", "-xa", "n", "-xal"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
        REQUIRE_FALSE(parser.calculateRRA());
        REQUIRE_FALSE(parser.useChoice());
        REQUIRE(parser.useLocal());
    }

    SECTION("Multiple")
    {
        ArgumentHolder ah{"prog", "-xl", "1.2,1.5", "-xa", "n", "-xl", "2.4,1.0"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE(parser.runAllLines());
        REQUIRE(parser.getAllAxesRoots().size() == 2);
        REQUIRE(parser.getAllAxesRoots()[0].x == Approx(1.2));
        REQUIRE(parser.getAllAxesRoots()[0].y == Approx(1.5));
        REQUIRE(parser.getAllAxesRoots()[1].x == Approx(2.4));
        REQUIRE(parser.getAllAxesRoots()[1].y == Approx(1.0));
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
    }

}
