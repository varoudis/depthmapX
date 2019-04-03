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

#include "../mgraph440/legacyconverters.h"
#include "catch.hpp"

TEST_CASE("vector conversion") {
    std::vector<int> vec{1, 4, 5};
    mgraph440::pvector<int> result = genshim440::toPVector(vec);
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 1);
    REQUIRE(result[1] == 4);
    REQUIRE(result[2] == 5);
}
