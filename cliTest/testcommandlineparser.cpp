#include "catch.hpp"
#include "../depthmapXcli/commandlineparser.h"
#include <cstring>

class ArgumentHolder{
public:
    ArgumentHolder(std::initializer_list<std::string> l ): mArguments(l){
        for (auto& arg : mArguments) {
               mArgv.push_back(arg.data());
        }
    }

    char** argv() const{
        return (char**) mArgv.data();
    }

    size_t argc() const{
        return mArgv.size();
    }

private:
    std::vector<std::string> mArguments;
    std::vector<const char *> mArgv;
};

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


TEST_CASE("Test ArgumentHolder", "Constructor")
{
    ArgumentHolder ah{"foo", "bar"};
    REQUIRE(ah.argc() == 2);
    REQUIRE(strcmp(ah.argv()[0], "foo") == 0 );
    REQUIRE(strcmp(ah.argv()[1], "bar") == 0 );
}

TEST_CASE("VGA args invalid", "")
{
    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-vm requires an argument"));
    }


    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "foo"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("Invalid VGA mode: foo"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vm", "metric"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-vm can only be used once"));
    }


    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("Global measures in VGA/visibility analysis require a radius, use -vr <radius>"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg", "-vr"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("-vr requires an argument"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vg", "-vr", "foo"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("Radius must be a positive integer number or n, got foo"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "metric"};
        REQUIRE_THROWS_WITH(CommandLineParser(ah.argc(), ah.argv()), Catch::Contains("Metric vga requires a radius, use -vr <radius>"));
    }
}

TEST_CASE("VGA args valid", "valid")
{
    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.getVgaMode() == VgaMode::ISOVIST);
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "isovist"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.getVgaMode() == VgaMode::ISOVIST);
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.getVgaMode() == VgaMode::VISBILITY);
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "VGA", "-vm", "visibility", "-vl", "-vg", "-vr", "4"};
        CommandLineParser cmdP(ah.argc(), ah.argv());
        REQUIRE(cmdP.isValid());
        REQUIRE(cmdP.getVgaMode() == VgaMode::VISBILITY);
        REQUIRE(cmdP.globalMeasures());
        REQUIRE(cmdP.localMeasures());
        REQUIRE(cmdP.getRadius() == "4");
    }


}


