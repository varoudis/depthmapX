#include "viewhelpers.h"
#include "catch.hpp"
#include <time.h>
#include <sstream>
#include <iomanip>

TEST_CASE("Calculating the new center", "[calculateCenter]"){
    auto point = QPoint(100, 100);
    auto oldCenter = QPoint(200,200);
    auto newCenter = QPoint(150,150);

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 0.5) == newCenter);

    newCenter.rx() = 300;
    newCenter.ry() = 300;

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 2.0) == newCenter);

}

TEST_CASE("Date string format", "[getCurrentDate]"){
    auto now = time(NULL);
    const tm* ltime = localtime(&now);
    std::stringstream sstream;
    sstream << ltime->tm_year + 1900 << "/" <<
            setfill('0') << setw(2) << ltime->tm_mon + 1 << "/" <<
            setfill('0') << setw(2) << ltime->tm_mday << std::flush;

    REQUIRE(ViewHelpers::getCurrentDate() ==  pstring(sstream.str().c_str()));
}

