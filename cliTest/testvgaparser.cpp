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
#include "../depthmapXcli/vgaparser.h"
#include "argumentholder.h"

TEST_CASE("VGA args invalid", "")
{
    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("-vm requires an argument"));
    }


    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "foo"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("Invalid VGA mode: foo"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vm", "metric"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("-vm can only be used once"));
    }


    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("Global measures in VGA/visibility analysis require a radius, use -vr <radius>"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg", "-vr"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("-vr requires an argument"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg", "-vr", "foo"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("Radius must be a positive integer number or n, got foo"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "metric"};
        REQUIRE_THROWS_WITH(VgaParser(ah.argc(), ah.argv()), Catch::Contains("Metric vga requires a radius, use -vr <radius>"));
    }
}

TEST_CASE("VGA args valid", "valid")
{
    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA"};
        VgaParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.getVgaMode() == VgaParser::VgaMode::ISOVIST);
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "isovist"};
        VgaParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.getVgaMode() == VgaParser::VgaMode::ISOVIST);
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility"};
        VgaParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.getVgaMode() == VgaParser::VgaMode::VISBILITY);
        REQUIRE_FALSE(cmdP.localMeasures());
        REQUIRE_FALSE(cmdP.globalMeasures());
        REQUIRE(cmdP.getRadius().empty());
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vl", "-vg", "-vr", "4"};
        VgaParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.getVgaMode() == VgaParser::VgaMode::VISBILITY);
        REQUIRE(cmdP.globalMeasures());
        REQUIRE(cmdP.localMeasures());
        REQUIRE(cmdP.getRadius() == "4");
    }


}
