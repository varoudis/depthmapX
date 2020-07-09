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
#include "salalib/mgraph.h"

TEST_CASE("Failing empty drawing map conversion", "") {
    std::vector<SpacePixelFile> drawingFiles;
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToAxial(nullptr, "Axial map", drawingFiles),
                        Catch::Contains("Failed to convert lines"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles),
                        Catch::Contains("No lines found in drawing"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToConvex(nullptr, "Convex map", drawingFiles),
                        Catch::Contains("No polygons found in drawing"));

    drawingFiles.push_back(SpacePixelFile("Drawing file"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToAxial(nullptr, "Axial map", drawingFiles),
                        Catch::Contains("Failed to convert lines"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles),
                        Catch::Contains("No lines found in drawing"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToConvex(nullptr, "Convex map", drawingFiles),
                        Catch::Contains("No polygons found in drawing"));

    drawingFiles.back().m_spacePixels.push_back(ShapeMap("Drawing layer", ShapeMap::DRAWINGMAP));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToAxial(nullptr, "Axial map", drawingFiles),
                        Catch::Contains("Failed to convert lines"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles),
                        Catch::Contains("No lines found in drawing"));
    REQUIRE_THROWS_WITH(MapConverter::convertDrawingToConvex(nullptr, "Convex map", drawingFiles),
                        Catch::Contains("No polygons found in drawing"));
}

TEST_CASE("Failing empty axial to segment map conversion", "") {
    ShapeGraph segmentMap("Axial map", ShapeMap::AXIALMAP);
    // TODO: Does not throw an exception but maybe it should as the axial map is empty?
    // REQUIRE_THROWS_WITH(MapConverter::convertAxialToSegment(nullptr, segmentMap, "Segment map", false, false, 0),
    // Catch::Contains("No lines found in drawing"));
}

TEST_CASE("Failing empty data map conversion", "") {
    ShapeMap dataMap("Data map", ShapeMap::DATAMAP);
    REQUIRE_THROWS_WITH(MapConverter::convertDataToAxial(nullptr, "Axial map", dataMap),
                        Catch::Contains("No lines found in data map"));
    REQUIRE_THROWS_WITH(MapConverter::convertDataToSegment(nullptr, "Segment map", dataMap),
                        Catch::Contains("No lines found in data map"));
    REQUIRE_THROWS_WITH(MapConverter::convertDataToConvex(nullptr, "Convex map", dataMap),
                        Catch::Contains("No polygons found in data map"));
}

TEST_CASE("Test drawing to segment conversion", "") {
    const float EPSILON = 0.001;

    Line line1(Point2f(0, 0), Point2f(0, 1));
    Line line2(Point2f(0, 1), Point2f(1, 1));
    Line line3(Point2f(1, 1), Point2f(1, 0));

    std::vector<SpacePixelFile> drawingFiles;
    drawingFiles.push_back(SpacePixelFile("Drawing file"));
    drawingFiles.back().m_spacePixels.push_back(ShapeMap("Drawing layer", ShapeMap::DRAWINGMAP));
    ShapeMap &drawingLayer = drawingFiles.back().m_spacePixels.back();

    SECTION("Single line") {
        drawingLayer.makeLineShape(line1);

        // TODO: This fails with std::bad_alloc because there's only 1 line in the drawing
        REQUIRE_THROWS_WITH(MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles),
                            Catch::Contains("std::bad_alloc"));
    }

    SECTION("Two lines") {
        drawingLayer.makeLineShape(line1);
        drawingLayer.makeLineShape(line2);

        // TODO: This fails with std::bad_alloc because there's only 2 lines in the drawing
        REQUIRE_THROWS_WITH(MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles),
                            Catch::Contains("std::bad_alloc"));
    }

    SECTION("Three lines") {
        drawingLayer.makeLineShape(line1);
        drawingLayer.makeLineShape(line2);
        drawingLayer.makeLineShape(line3);
        std::unique_ptr<ShapeGraph> segmentMap =
            MapConverter::convertDrawingToSegment(nullptr, "Segment map", drawingFiles);
        std::map<int, SalaShape> &shapes = segmentMap->getAllShapes();
        REQUIRE(shapes.size() == 3);
        auto shapeIter = shapes.begin();
        REQUIRE(shapeIter->first == 0);
        const Line &segmentLine1 = shapeIter->second.getLine();
        REQUIRE(segmentLine1.ax() == Approx(line1.ax()).epsilon(EPSILON));
        REQUIRE(segmentLine1.ay() == Approx(line1.ay()).epsilon(EPSILON));
        REQUIRE(segmentLine1.bx() == Approx(line1.bx()).epsilon(EPSILON));
        REQUIRE(segmentLine1.by() == Approx(line1.by()).epsilon(EPSILON));
        shapeIter++;
        REQUIRE(shapeIter->first == 1);
        const Line &segmentLine2 = shapeIter->second.getLine();
        REQUIRE(segmentLine2.ax() == Approx(line2.ax()).epsilon(EPSILON));
        REQUIRE(segmentLine2.ay() == Approx(line2.ay()).epsilon(EPSILON));
        REQUIRE(segmentLine2.bx() == Approx(line2.bx()).epsilon(EPSILON));
        REQUIRE(segmentLine2.by() == Approx(line2.by()).epsilon(EPSILON));
        shapeIter++;
        REQUIRE(shapeIter->first == 2);
        const Line &segmentLine3 = shapeIter->second.getLine();
        REQUIRE(segmentLine3.ax() == Approx(line3.ax()).epsilon(EPSILON));
        REQUIRE(segmentLine3.ay() == Approx(line3.ay()).epsilon(EPSILON));
        REQUIRE(segmentLine3.bx() == Approx(line3.bx()).epsilon(EPSILON));
        REQUIRE(segmentLine3.by() == Approx(line3.by()).epsilon(EPSILON));
    }
}

TEST_CASE("Test data to segment conversion", "") {
    const float EPSILON = 0.001;

    std::string newAttributeName = "testID";
    ShapeMap dataMap("Data map", ShapeMap::DATAMAP);
    int newAttributeID = dataMap.addAttribute(newAttributeName);

    std::vector<Line> lines;
    std::vector<std::map<int, float>> extraAttributes;

    lines.push_back(Line(Point2f(0, 0), Point2f(0, 1)));
    lines.push_back(Line(Point2f(0, 1), Point2f(1, 1)));
    lines.push_back(Line(Point2f(1, 1), Point2f(1, 0)));

    for (int i = 0; i < lines.size(); i++) {
        extraAttributes.push_back(std::map<int, float>());
        extraAttributes.back()[newAttributeID] = extraAttributes.size();
    }

    SECTION("Single line with extra attributes") {
        dataMap.makeLineShape(lines[0], false, false, extraAttributes[0]);

        // TODO: This fails with std::bad_alloc because there's only 1 line in the data map
        REQUIRE_THROWS_WITH(MapConverter::convertDataToSegment(nullptr, "Segment map", dataMap, true),
                            Catch::Contains("std::bad_alloc"));
    }

    SECTION("Two lines with extra attributes") {
        dataMap.makeLineShape(lines[0], false, false, extraAttributes[0]);
        dataMap.makeLineShape(lines[1], false, false, extraAttributes[1]);

        // TODO: This fails with std::bad_alloc because there's only 2 lines in the data map
        REQUIRE_THROWS_WITH(MapConverter::convertDataToSegment(nullptr, "Segment map", dataMap, true),
                            Catch::Contains("std::bad_alloc"));
    }

    SECTION("Three lines") {
        dataMap.makeLineShape(lines[0], false, false, extraAttributes[0]);
        dataMap.makeLineShape(lines[1], false, false, extraAttributes[1]);
        dataMap.makeLineShape(lines[2], false, false, extraAttributes[2]);
        std::unique_ptr<ShapeGraph> segmentMap =
            MapConverter::convertDataToSegment(nullptr, "Segment map", dataMap, true);
        int segmentNewAttributeID = segmentMap->getAttributeTable().getColumnIndex(newAttributeName);
        std::map<int, SalaShape> &shapes = segmentMap->getAllShapes();
        REQUIRE(shapes.size() == 3);
        auto shapeIter = shapes.begin();
        for (int i = 0; i < lines.size(); i++) {
            REQUIRE(shapeIter->first == i);
            AttributeRow &row = segmentMap->getAttributeRowFromShapeIndex(shapeIter->first);
            REQUIRE(row.getValue(segmentNewAttributeID) == extraAttributes[i][newAttributeID]);
            const Line &segmentLine = shapeIter->second.getLine();
            REQUIRE(segmentLine.ax() == Approx(lines[i].ax()).epsilon(EPSILON));
            REQUIRE(segmentLine.ay() == Approx(lines[i].ay()).epsilon(EPSILON));
            REQUIRE(segmentLine.bx() == Approx(lines[i].bx()).epsilon(EPSILON));
            REQUIRE(segmentLine.by() == Approx(lines[i].by()).epsilon(EPSILON));
            shapeIter++;
        }
    }

    SECTION("Four lines, second line twice") {
        dataMap.makeLineShape(lines[0], false, false, extraAttributes[0]);
        dataMap.makeLineShape(lines[1], false, false, extraAttributes[1]);
        dataMap.makeLineShape(lines[1], false, false, extraAttributes[1]); // this one should be removed by tidylines
        dataMap.makeLineShape(lines[2], false, false, extraAttributes[2]);
        std::unique_ptr<ShapeGraph> segmentMap =
            MapConverter::convertDataToSegment(nullptr, "Segment map", dataMap, true);
        int segmentNewAttributeID = segmentMap->getAttributeTable().getColumnIndex(newAttributeName);
        std::map<int, SalaShape> &shapes = segmentMap->getAllShapes();
        REQUIRE(shapes.size() == 3);
        auto shapeIter = shapes.begin();
        for (int i = 0; i < lines.size(); i++) {
            REQUIRE(shapeIter->first == i);
            AttributeRow &row = segmentMap->getAttributeRowFromShapeIndex(shapeIter->first);
            REQUIRE(row.getValue(segmentNewAttributeID) == extraAttributes[i][newAttributeID]);
            const Line &segmentLine = shapeIter->second.getLine();
            REQUIRE(segmentLine.ax() == Approx(lines[i].ax()).epsilon(EPSILON));
            REQUIRE(segmentLine.ay() == Approx(lines[i].ay()).epsilon(EPSILON));
            REQUIRE(segmentLine.bx() == Approx(lines[i].bx()).epsilon(EPSILON));
            REQUIRE(segmentLine.by() == Approx(lines[i].by()).epsilon(EPSILON));
            shapeIter++;
        }
    }
}
