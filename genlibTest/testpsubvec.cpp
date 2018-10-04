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


#include "catch.hpp"

#include "genlib/psubvec.h"


TEST_CASE("psubvec creation")
{
    psubvec<short> psv;
    psv.push_back(0);
    psv.push_back(1);
    psv.push_back(4);
    psv.push_back(-1);
    psv.push_back(128);

    REQUIRE(psv.size() == 5);
    REQUIRE(!psv.isEmpty());
    REQUIRE(psv[0] == 0);
    REQUIRE(psv[1] == 1);
    REQUIRE(psv[2] == 4);
    REQUIRE(psv[3] == -1);
    REQUIRE(psv[4] == 128);

    psv.clear();

    REQUIRE(psv.isEmpty());
    REQUIRE(psv.size() == 0);

    psubvec<short> psv1;
    psv1.push_back(512);
    psv1.push_back(2215);
    psv1.push_back(-122);

    psv = psv1;

    REQUIRE(psv.size() == 3);
    REQUIRE(!psv.isEmpty());
    REQUIRE(psv[0] == 512);
    REQUIRE(psv[1] == 2215);
    REQUIRE(psv[2] == -122);

    psubvec<char> psvc;
    psvc.push_back('1');
    psvc.push_back('a');
    psvc.push_back('\t');

    REQUIRE(psvc.size() == 3);
    REQUIRE(!psvc.isEmpty());
    REQUIRE(psvc[0] == '1');
    REQUIRE(psvc[1] == 'a');
    REQUIRE(psvc[2] == '\t');
}
