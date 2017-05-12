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
#include "../depthmapXcli/commandlineparser.h"
#include "../depthmapXcli/imodeparser.h"
#include <cstring>
#include "argumentholder.h"

TEST_CASE("Invalid Parser","Constructor"){

    {
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(0, 0), Catch::Contains("No commandline parameters provided - don't know what to do"));
    }

    {
        ArgumentHolder ah{"prog", "-m"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "-m requires an argument");
    }

    {
        ArgumentHolder ah{"prog", "-f"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "-f requires an argument");
    }

    {
        ArgumentHolder ah{"prog", "-o"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "-o requires an argument");
    }

    {
        ArgumentHolder ah{"prog", "-t", "-o"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "-t requires an argument");
    }


    {
        ArgumentHolder ah{"prog", "-m", "-f"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "-m requires an argument");
    }

    {
        ArgumentHolder ah{"prog", "-m", "LaLaLa"};
        REQUIRE_THROWS_WITH(auto cmdP = CommandLineParser(ah.argc(), ah.argv()), "Invalid mode: LaLaLa");
    }
    {
        ArgumentHolder ah{"prog", "-f", "inputfile.graph", "-o", "outputfile.graph"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-m for mode is required"));
    }
    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-m", "LINK","-f", "inputfile.graph"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-m can only be used once"));
    }

    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-o", "outputfile.graph"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-f for input file is required"));
    }
    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-f", "inputfile.graph"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-o for output file is required"));
    }

}

TEST_CASE("Valid Parser","CheckValues"){
    {
        ArgumentHolder ah{"prog", "-m", "LINK", "-f", "inputfile.graph", "-o", "outputfile.graph", "-lnk", "1.2,3.4,5.6,7.8"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.getTimingFile().empty());
    }
    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-f", "inputfile.graph", "-o", "outputfile.graph"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE_FALSE(cmdP.simpleMode());
        REQUIRE(cmdP.getTimingFile().empty());
        REQUIRE(cmdP.modeOptions().getModeName() == "VGA");
    }
    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-f", "inputfile.graph", "-o", "outputfile.graph", "-s", "-t", "timings.csv"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.simpleMode());
        REQUIRE(cmdP.getTimingFile() == "timings.csv");
    }

}

TEST_CASE("Invalid Parser Need Help", "CheckForHelp")
{
    ArgumentHolder ah{ "prog", "-h"};
    CommandLineParser cmdP(ah.argc(), ah.argv());
    REQUIRE_FALSE(cmdP.isValid());

}


