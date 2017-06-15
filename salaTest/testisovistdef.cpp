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
#include "../salalib/isovistdef.h"

TEST_CASE("Full Isovist")
{
    IsovistDefinition isovist(1.0, 1.0);
    REQUIRE(isovist.getLocation().x == Approx(1.0));
    REQUIRE(isovist.getLocation().y == Approx(1.0));
    REQUIRE(isovist.getAngle() == 0.0);
    REQUIRE(isovist.getViewAngle() == Approx(-1.0));
    REQUIRE_FALSE(isovist.partialIsovist());
}

TEST_CASE("Partial Isovist")
{
    SECTION("No overrun")
    {
        IsovistDefinition isovist(1.0, 1.0, 2.0, 1.0);
        REQUIRE(isovist.getLocation().x == Approx(1.0));
        REQUIRE(isovist.getLocation().y == Approx(1.0));
        REQUIRE(isovist.getAngle() == Approx(2.0));
        REQUIRE(isovist.getViewAngle() == Approx(1.0));
        REQUIRE(isovist.partialIsovist());
        REQUIRE(isovist.getLeftAngle() == Approx(1.5) );
        REQUIRE(isovist.getRightAngle() == Approx(2.5) );
    }

    SECTION("Overrun left")
    {
        IsovistDefinition isovist(1.0, 1.0, 0.1, 1.0);
        REQUIRE(isovist.getLocation().x == Approx(1.0));
        REQUIRE(isovist.getLocation().y == Approx(1.0));
        REQUIRE(isovist.getAngle() == Approx(0.1));
        REQUIRE(isovist.getViewAngle() == Approx(1.0));
        REQUIRE(isovist.partialIsovist());
        REQUIRE(isovist.getLeftAngle() == Approx(5.8831853072) );
        REQUIRE(isovist.getRightAngle() == Approx(0.6) );

    }

    SECTION("Overrun right")
    {
        IsovistDefinition isovist(1.0, 1.0, 6.1, 1.0);
        REQUIRE(isovist.getLocation().x == Approx(1.0));
        REQUIRE(isovist.getLocation().y == Approx(1.0));
        REQUIRE(isovist.getAngle() == Approx(6.1));
        REQUIRE(isovist.getViewAngle() == Approx(1.0));
        REQUIRE(isovist.partialIsovist());
        REQUIRE(isovist.getLeftAngle() == Approx(5.6) );
        REQUIRE(isovist.getRightAngle() == Approx(0.31681469) );
    }

}
