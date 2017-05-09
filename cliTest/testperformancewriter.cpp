#include <catch.hpp>
#include "../depthmapXcli/performancewriter.h"
#include "selfcleaningfile.h"
#include <fstream>

TEST_CASE("TestPerformanceWriting", "Simple test case")
{
    SelfCleaningFile scf("timertest.csv");
    PerformanceWriter writer(scf.Filename());

    writer.AddData("test1", 100.0);
    writer.AddData("test2", 200.0 );

    writer.Write();

    std::ifstream f(scf.Filename());
    REQUIRE(f.good());
    char buffer[1000];
    f.read(buffer, 999);
    REQUIRE_THAT(buffer, Catch::Matchers::Equals("\"action\",\"duration\"\n\"test1\",100\n\"test2\",200\n"));

}
