// Copyright (C) 2018 Petros Koutsolampros

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

#include <catch.hpp>
#include "depthmapXcli/mapconvertparser.h"
#include "argumentholder.h"
#include "selfcleaningfile.h"

TEST_CASE("MapConvertParserFail", "Error cases")
{
    SECTION("Missing argument to co")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-co"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-co requires an argument"));
    }
    SECTION("Missing argument to con")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-con"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-con requires an argument"));
    }
    SECTION("Missing argument to crsl")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-crsl"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-crsl requires an argument"));
    }

    SECTION("Non-numeric input to -crsl")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-crsl", "foo"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-crsl must be a number >0, got foo"));
    }

    SECTION("Under-zero input to -crsl")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-crsl", "-1"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-crsl must be a number >0, got -1"));
    }

    SECTION("Zero input to -crsl")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-crsl", "-1"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-crsl must be a number >0, got -1"));
    }

    SECTION("rubbish input to -co")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-co", "foo"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("Invalid map output (-co) type: foo"));
    }

    SECTION("output type (-co) provided twice")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-co", "axial", "-co", "drawing"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("-co can only be used once, modes are mutually exclusive"));
    }

    SECTION("Don't provide output type")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("A valid output map type (-co) is required"));
    }

    SECTION("Don't provide output name")
    {
        MapConvertParser parser;
        ArgumentHolder ah{"prog", "-co", "axial"};
        REQUIRE_THROWS_WITH(parser.parse(int(ah.argc()), ah.argv()), Catch::Contains("A valid output map name (-con) is required"));
    }
}

TEST_CASE("MapConvertParserSuccess", "Read successfully")
{
    MapConvertParser parser;

    SECTION("Plain axial")
    {
        ArgumentHolder ah{"prog", "-co", "axial", "-con", "new_axial"};
        parser.parse(int(ah.argc()), ah.argv());
        REQUIRE(parser.outputMapName() == "new_axial");
        REQUIRE(parser.outputMapType() == ShapeMap::AXIALMAP);
    }
}
