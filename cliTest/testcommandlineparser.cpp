#include "catch.hpp"
#include "../depthmapXcli/commandlineparser.h"
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
        ArgumentHolder ah{"prog", "-m", "VGA", "-f", "inputfile.graph", "-o", "outputfile.graph"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE_FALSE(cmdP.simpleMode());
    }
    {
        ArgumentHolder ah{"prog", "-m", "VGA", "-f", "inputfile.graph", "-o", "outputfile.graph", "-s"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.simpleMode());
    }

}

TEST_CASE("Invalid Parser Need Help", "CheckForHelp")
{
    ArgumentHolder ah{ "prog", "-h"};
    CommandLineParser cmdP(ah.argc(), ah.argv());
    REQUIRE_FALSE(cmdP.isValid());

}


