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
        ArgumentHolder ah{"prog", "-am"};
        REQUIRE_THROWS_WITH(parser.parse(ah.argc(), ah.argv()), "-am requires an argument" );
    }
}

TEST_CASE("Test mode parsing", "")
{
    AxialParser parser;
    SECTION("All lines")
    {
        ArgumentHolder ah{"prog", "-am", "all"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }
    SECTION("Fewest lines")
    {
        ArgumentHolder ah{"prog", "-am", "fewest"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }
    SECTION("Unlink")
    {
        ArgumentHolder ah{"prog", "-am", "unlink"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }
    SECTION("Analysis")
    {
        ArgumentHolder ah{"prog", "-am", "analysis"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE_FALSE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE_FALSE(parser.runUnlink());
        REQUIRE(parser.runAnalysis());
    }
    SECTION("Multiple")
    {
        ArgumentHolder ah{"prog", "-am", "all", "-am", "unlink"};
        parser.parse(ah.argc(), ah.argv());
        REQUIRE(parser.runAllLines());
        REQUIRE_FALSE(parser.runFewestLines());
        REQUIRE(parser.runUnlink());
        REQUIRE_FALSE(parser.runAnalysis());
    }

}
