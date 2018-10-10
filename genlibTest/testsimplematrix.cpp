// Copyright (C) 2018 Christian Sailer

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
#include "../genlib/simplematrix.h"
#include <vector>
#include <algorithm>

TEST_CASE("First row matrix test"){
    depthmapX::RowMatrix<int> matrix(2, 3);
    matrix(0,0) = 1;
    matrix(1,2) = -1;
    matrix(0,1) = 2;
    matrix(0,2) = 3;
    matrix(1,0) = -23;
    matrix(1,1) = 0;

    REQUIRE(matrix(1,2) == -1);

    std::vector<int> result(6);
    std::vector<int> expected = { 1, 2, 3, -23, 0, -1};

    std::copy(matrix.begin(), matrix.end(), result.begin() );
    REQUIRE(result == expected);
}
