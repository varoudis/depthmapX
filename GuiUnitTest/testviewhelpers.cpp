#include "viewhelpers.h"
#include "catch.hpp"

TEST_CASE("Calculating the new center", "[calculateCenter]"){
    auto point = QPoint(100, 100);
    auto oldCenter = QPoint(200,200);
    auto newCenter = QPoint(150,150);

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 0.5) == newCenter);

    newCenter.rx() = 300;
    newCenter.ry() = 300;

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 2.0) == newCenter);

}
