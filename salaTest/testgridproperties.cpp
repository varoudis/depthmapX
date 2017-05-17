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
#include "salalib/gridproperties.h"

TEST_CASE("TestGridProperties", "Test the calculations of grid properties")
{
    double maxDimension = 4.583;
    GridProperties gp(maxDimension);
    REQUIRE(gp.getDefault() == Approx(0.04));
    REQUIRE(gp.getMax() == Approx(0.8));
    REQUIRE(gp.getMin() == Approx(0.004));
}
