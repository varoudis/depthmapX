// Copyright (C) 2017 Petros Koutsolampros

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
#include "salalib/textparser.h"
#include "genlib/p2dpoly.h"
#include <sstream>
#include <vector>

TEST_CASE("Failing line parser", "")
{
    const float EPSILON = 0.001;
    {
        // header only has 3 elements
        std::stringstream stream;
        stream << "x1,y1,x2" << std::endl;
        REQUIRE_THROWS_WITH(textParser::parseLines(stream,','), Catch::Contains(""));
    }

    {
        // header has y1 twice instead of y2
        std::stringstream stream;
        stream << "x1,y1,x2,y1" << std::endl;
        REQUIRE_THROWS_WITH(textParser::parseLines(stream,','), Catch::Contains(""));
    }

    {
        // error parsing line
        std::stringstream stream;
        stream << "x1,y1,x2,y2" << std::endl;
        stream << "1.2,3.4,5.6" << std::endl;
        REQUIRE_THROWS_WITH(textParser::parseLines(stream,','), Catch::Contains(""));
    }
}
TEST_CASE("Successful line parser", "")
{
    const float EPSILON = 0.001;
    {
        std::stringstream stream;
        stream << "x1,y1,x2,y2" << std::endl;
        stream << "1.2,3.4,5.6,7.8" << std::endl;
        std::vector<Line> lines = textParser::parseLines(stream,',');
        REQUIRE(lines.size() == 1);
        REQUIRE(lines[0].start().x == Approx(1.2).epsilon(EPSILON));
        REQUIRE(lines[0].start().y == Approx(3.4).epsilon(EPSILON));
        REQUIRE(lines[0].end().x == Approx(5.6).epsilon(EPSILON));
        REQUIRE(lines[0].end().y == Approx(7.8).epsilon(EPSILON));
    }

    {
        std::stringstream stream;
        stream << "x1\ty1\tx2\ty2" << std::endl;
        stream << "1.2\t3.4\t5.6\t7.8" << std::endl;
        std::vector<Line> lines = textParser::parseLines(stream,'\t');
        REQUIRE(lines.size() == 1);
        REQUIRE(lines[0].start().x == Approx(1.2).epsilon(EPSILON));
        REQUIRE(lines[0].start().y == Approx(3.4).epsilon(EPSILON));
        REQUIRE(lines[0].end().x == Approx(5.6).epsilon(EPSILON));
        REQUIRE(lines[0].end().y == Approx(7.8).epsilon(EPSILON));
    }

    {
        std::stringstream stream;
        stream << "x1\ty1\tx2\ty2" << std::endl;
        stream << "1.2\t3.4\t5.6\t7.8" << std::endl;
        stream << "0.1\t0.2\t0.3\t0.4" << std::endl;
        std::vector<Line> lines = textParser::parseLines(stream,'\t');
        REQUIRE(lines.size() == 2);
        REQUIRE(lines[0].start().x == Approx(1.2).epsilon(EPSILON));
        REQUIRE(lines[0].start().y == Approx(3.4).epsilon(EPSILON));
        REQUIRE(lines[0].end().x == Approx(5.6).epsilon(EPSILON));
        REQUIRE(lines[0].end().y == Approx(7.8).epsilon(EPSILON));
        REQUIRE(lines[1].start().x == Approx(0.1).epsilon(EPSILON));
        REQUIRE(lines[1].start().y == Approx(0.2).epsilon(EPSILON));
        REQUIRE(lines[1].end().x == Approx(0.3).epsilon(EPSILON));
        REQUIRE(lines[1].end().y == Approx(0.4).epsilon(EPSILON));
    }
}

TEST_CASE("Tests for split function", "")
{
    {
        std::vector<std::string> stringParts = textParser::split("foo,bar",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "foo");
        REQUIRE(stringParts[1] == "bar");
    }

    {
        std::vector<std::string> stringParts = textParser::split("0.5,1.2",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = textParser::split("0.5\t1.2",'\t');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = textParser::split("0.5\t1.2\tfoo",'\t');
        REQUIRE(stringParts.size() == 3);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
        REQUIRE(stringParts[2] == "foo");
    }

    {
        // skip last blank element
        std::vector<std::string> stringParts = textParser::split("foo,bar,",',');
        REQUIRE(stringParts.size() == 2);
    }

    {
        // do not skip middle blank element
        std::vector<std::string> stringParts = textParser::split("foo,,bar",',');
        REQUIRE(stringParts.size() == 3);
    }
}
