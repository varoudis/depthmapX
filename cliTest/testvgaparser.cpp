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
