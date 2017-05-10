#include <catch.hpp>
#include "../depthmapXcli/performancewriter.h"
#include "selfcleaningfile.h"
#include <fstream>

TEST_CASE("TestPerformanceWriting", "Simple test case")
{
    SelfCleaningFile scf("timertest.csv");
    PerformanceWriter writer(scf.Filename());

    writer.addData("test1", 100.0);
    writer.addData("test2", 200.0 );

    writer.write();

    std::ifstream f(scf.Filename());
    REQUIRE(f.good());
    char line[1000];
    std::vector<std::string> lines;
    while( !f.eof())
    {
        f.getline(line, 1000);
        lines.push_back(line);
    }
    std::vector<std::string> expected{"\"action\",\"duration\"", "\"test1\",100", "\"test2\",200", ""};
    REQUIRE(lines == expected);

}

TEST_CASE("TestPerformanceNotWriting", "No filename no writing")
{
    SelfCleaningFile scf("timertest.csv");
    PerformanceWriter writer("");

    writer.addData("test1", 100.0);
    writer.addData("test2", 200.0 );

    writer.write();

    std::ifstream f(scf.Filename());
    REQUIRE_FALSE(f.good());
}
