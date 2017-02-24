#include "catch.hpp"
#include "../depthmapXcli/simpletimer.h"
#include <thread>
#include <chrono>

TEST_CASE("TestSimpleTimer", "")
{
    SimpleTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    REQUIRE(timer.getTimeInSeconds() == Approx(0.5).epsilon(0.2));
}
