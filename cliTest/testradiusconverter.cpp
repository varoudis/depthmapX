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
#include "../depthmapXcli/radiusconverter.h"

TEST_CASE("ConvertForMetric","")
{
    RadiusConverter converter;
    REQUIRE(converter.ConvertForMetric("2") == Approx(2.0));
    REQUIRE(converter.ConvertForMetric("n") == Approx(-1.0));
    REQUIRE(converter.ConvertForMetric("2.6") == Approx(2.6));
    REQUIRE(converter.ConvertForMetric("2.3e12") == Approx(2.3e12));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("-1"), Catch::Contains("Radius for metric vga must be n for the whole range or a positive number. Got -1"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("foo"), Catch::Contains("Radius for metric vga must be n for the whole range or a positive number. Got foo"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("NaN"), Catch::Contains("Radius NaN?! Really?"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("INFINITY"), Catch::Contains("Radius inf?! Who are you kidding?"));
}
