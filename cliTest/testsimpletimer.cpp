// Copyright (C) 2026 Tasos Varoudis
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
#include "../depthmapXcli/simpletimer.h"
#include <thread>
#include <chrono>

// sleep_for only guarantees a lower bound on how long it sleeps, so these tests
// assert the timer's actual invariants rather than a tolerance around the sleep.
// A loaded machine can overshoot by any amount - a shared CI runner routinely
// does - and a tolerance tight enough to be meaningful is also tight enough to
// fail at random. The generous upper bound is there to catch a timer reporting
// the wrong unit, which is the failure mode that would really matter.

static const double SLEEP_SECONDS = 0.5;
static const double ABSURDLY_LONG = 60.0;

TEST_CASE("TestSimpleTimer", "")
{
    SimpleTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    const double elapsed = timer.getTimeInSeconds();
    REQUIRE(elapsed >= SLEEP_SECONDS);
    REQUIRE(elapsed < ABSURDLY_LONG);
}

TEST_CASE("TestSimpleTimerReset", "")
{
    SimpleTimer timer1;
    SimpleTimer timer2;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    REQUIRE(timer1.getTimeInSeconds() >= SLEEP_SECONDS);
    REQUIRE(timer2.getTimeInSeconds() >= SLEEP_SECONDS);

    timer2.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    const double unreset = timer1.getTimeInSeconds();
    const double reset = timer2.getTimeInSeconds();

    // timer1 has been running across both sleeps, timer2 only across the second
    REQUIRE(unreset >= 2 * SLEEP_SECONDS);
    REQUIRE(reset >= SLEEP_SECONDS);
    REQUIRE(reset < unreset);
    REQUIRE(unreset < ABSURDLY_LONG);
}

