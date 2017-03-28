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

TEST_CASE("TestSimpleTimerReset", "")
{
    SimpleTimer timer1;
    SimpleTimer timer2;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    REQUIRE(timer1.getTimeInSeconds() == Approx(0.5).epsilon(0.2));
    REQUIRE(timer2.getTimeInSeconds() == Approx(0.5).epsilon(0.2));
    timer2.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    REQUIRE(timer1.getTimeInSeconds() == Approx(1.0).epsilon(0.2));
    REQUIRE(timer2.getTimeInSeconds() == Approx(0.5).epsilon(0.2));
}

