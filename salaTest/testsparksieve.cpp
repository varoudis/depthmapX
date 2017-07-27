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
#include "salalib/sparksieve2.h"


TEST_CASE("One block garbage")
{
    Point2f centre(1,1);
    sparkSieve2 sieve(centre);
    pqmap<int, Line> lines;
    // these lines get turned into "blocks" based by a tanify function based on q and the centre given
    // above. Given q=4 and centre 1,1 this line will be from 0.625 to something bigger than 1
    lines.add(1, Line(Point2f(0.5, 0.2), Point2f(0.5, 0.7)));
    sieve.block(lines,4);
    sieve.collectgarbage();
    sieve.m_gaps.first();
    REQUIRE((*sieve.m_gaps).start == 0);
    REQUIRE((*sieve.m_gaps).end == Approx(0.625));
}


TEST_CASE("Shift start and end")
{
    Point2f centre(1,1);
    sparkSieve2 sieve(centre);
    pqmap<int, Line> lines;
    // .625 -> > 1
    lines.add(1, Line(Point2f(0.5, 0.2), Point2f(0.5, 0.7)));
    // < 0 -> 0.55555557
    lines.add(2, Line(Point2f(0.5,0.1),Point2f(1.1,0.9)));
    sieve.block(lines,4);
    sieve.collectgarbage();
    sieve.m_gaps.first();
    REQUIRE((*sieve.m_gaps).start == Approx(0.55555555555));
    REQUIRE((*sieve.m_gaps).end == Approx(0.625));
}

TEST_CASE("delete gap")
{
    Point2f centre(1,1);
    sparkSieve2 sieve(centre);
    pqmap<int, Line> lines;
    // < 0 -> > 1 the block covers the whole gap
    lines.add(1, Line(Point2f(1.1, 0.2), Point2f(0.5, 0.7)));
    sieve.block(lines,4);
    sieve.collectgarbage();
    sieve.m_gaps.first();
    REQUIRE(sieve.m_gaps.is_tail());
}

TEST_CASE("add gap")
{
    Point2f centre(1,1);
    sparkSieve2 sieve(centre);
    pqmap<int, Line> lines;
    // 0.55555 -> .625 the block splits the gap
    lines.add(1, Line(Point2f(0.5, 0.2), Point2f(0.5, 0.1)));
    //  0.71428571 -> > 1
    lines.add(2, Line(Point2f(0.5,0.3), Point2f(0.5,0.7)));
    sieve.block(lines,4);
    sieve.collectgarbage();
    sieve.m_gaps.first();
    REQUIRE_FALSE(sieve.m_gaps.is_tail());
    REQUIRE((*sieve.m_gaps).start == 0);
    REQUIRE((*sieve.m_gaps).end == Approx(0.55555555555));
    sieve.m_gaps++;
    REQUIRE_FALSE(sieve.m_gaps.is_tail());
    REQUIRE((*sieve.m_gaps).start == Approx(0.625));
    REQUIRE((*sieve.m_gaps).end == Approx( 0.71428571));
    sieve.m_gaps++;
    REQUIRE(sieve.m_gaps.is_tail());
}
