// Copyright (C) 2020 Petros Koutsolampros

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
#include "salalib/mapconverter.h"
#include "salalib/shapemap.h"

TEST_CASE("Test point in polygon in shapemap", "") {
    // The problem this test was made to demostrate was that shapemaps with
    // extended bounds tended to make the point-in-polygon more inaccurate.

    std::unique_ptr<ShapeMap> shapeMap(new ShapeMap("Test ShapeMap"));

    // main testing shape
    shapeMap->makePolyShape(
        {
            Point2f(4.50, 5.75), //
            Point2f(5.75, 5.50), //
            Point2f(5.25, 4.75), //
            Point2f(4.75, 4.50)  //
        },
        false);

    // points inside the polygon
    std::vector<Point2f> pointsInsidePoly = {
        Point2f(5.1250, 5.1250), //
        Point2f(5.6345, 5.4522), //
        Point2f(4.5884, 5.6616), //
        Point2f(4.8049, 4.6123), //
        Point2f(5.1673, 4.8437), //
        Point2f(4.9441, 4.7368), //
        Point2f(4.7476, 5.1495), //
        Point2f(5.1005, 5.5024), //
        Point2f(5.3960, 5.1943)  //
    };

    // points outside (but very close to) the polygon
    std::vector<Point2f> pointsOutsidePoly = {
        Point2f(4.6951, 4.3877), //
        Point2f(4.4116, 5.8384), //
        Point2f(5.8655, 5.5478), //
        Point2f(5.3327, 4.6563), //
        Point2f(5.0559, 4.5132), //
        Point2f(4.5024, 5.1005), //
        Point2f(5.1495, 5.7476), //
        Point2f(5.6040, 5.0557)  //
    };

    for (Point2f point : pointsInsidePoly) {
        REQUIRE(shapeMap->pointInPolyList(point)[0] == 0);
    }

    for (Point2f point : pointsOutsidePoly) {
        REQUIRE(shapeMap->pointInPolyList(point).size() == 0);
    }

    // now extend the bounds

    // first little extra rectangle to extend the map region to 0.25, 0.25
    shapeMap->makePolyShape(
        {
            Point2f(0.25, 0.25), //
            Point2f(0.50, 0.25), //
            Point2f(0.50, 0.50), //
            Point2f(0.25, 0.50)  //
        },
        false);

    // second little extra rectangle to extend the map region to 10.0, 10.0
    shapeMap->makePolyShape(
        {
            Point2f(9.75, 9.75), //
            Point2f(10.0, 9.75), //
            Point2f(10.0, 10.0), //
            Point2f(9.75, 10.0)  //
        },
        false);


    for (Point2f point : pointsInsidePoly) {
        REQUIRE(shapeMap->pointInPolyList(point)[0] == 0);
    }

    for (Point2f point : pointsOutsidePoly) {
        REQUIRE(shapeMap->pointInPolyList(point).size() == 0);
    }
}
