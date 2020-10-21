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
#include "salalib/mgraph.h"
#include "salalib/pointdata.h"
#include "salalib/shapemap.h"
#include "salalib/spacepixfile.h"

TEST_CASE("Push values from shapemaps to VGA", "") {

    float vgaMinX = 0.00;
    float vgaMinY = 0.00;
    float vgaMaxX = 6.00;
    float vgaMaxY = 6.00;
    float cellSize = 1.0;
    float minorOffset = cellSize * 0.05; // used to make sure that shapes don't fall exactly on the pointmap pixels

    // The testing pointmap looks like below, filled at the 'o'
    //
    //         1   2   3   4   5
    //       |   |   |   |   |   |
    //     - + - + - + - + - + - + -
    //  1    | o | o | o | o | o |
    //     - + - + - + - + - + - + -
    //  2    | o | o | o | o | o |
    //     - + - + - + - + - + - + -
    //  3    | o | o | o | o | o |
    //     - + - + - + - + - + - + -
    //  4    | o | o | o | o | o |
    //     - + - + - + - + - + - + -
    //  5    | o | o | o | o | o |
    //     - + - + - + - + - + - + -
    //       |   |   |   |   |   |

    // in fact it should not be a requirement to make all these maps through
    // the metagraph, but instead through a "map pusher" of some sorts

    std::unique_ptr<MetaGraph> mgraph(new MetaGraph);
    mgraph->m_drawingFiles.emplace_back("Drawing file");
    mgraph->m_drawingFiles.back().m_spacePixels.emplace_back("Drawing Map");
    ShapeMap &drawingMap = mgraph->m_drawingFiles.back().m_spacePixels.back();

    // rectangle containing the filled area of the pointmap offset by 0.5 to
    // make sure it falls exactly on the edge of the 1.0-sized cell
    drawingMap.makePolyShape(
        {
            Point2f(vgaMinX + cellSize * 0.5, vgaMinY + cellSize * 0.5), //
            Point2f(vgaMinX + cellSize * 0.5, vgaMaxY - cellSize * 0.5), //
            Point2f(vgaMaxX - cellSize * 0.5, vgaMaxY - cellSize * 0.5), //
            Point2f(vgaMaxX - cellSize * 0.5, vgaMinY + cellSize * 0.5)  //
        },
        false);
    Point2f bl = drawingMap.getRegion().bottom_left;
    Point2f tr = drawingMap.getRegion().top_right;
    mgraph->updateParentRegions(drawingMap);

    mgraph->addNewPointMap("VGA Map");
    PointMap &vgaMap = mgraph->getPointMaps().back();
    vgaMap.setGrid(1.0);
    vgaMap.makePoints(Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, (vgaMinY + vgaMaxY) * 0.5 + minorOffset), 0);
    vgaMap.sparkGraph2(nullptr, false, -1);
    AttributeTable &vgaTable = vgaMap.getAttributeTable();

    int minI = std::numeric_limits<int>::max();
    int minJ = std::numeric_limits<int>::max();
    int maxI = -std::numeric_limits<int>::max();
    int maxJ = -std::numeric_limits<int>::max();
    for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
        PixelRef key(vgaRowIter->getKey().value);
        if (key.x < minI)
            minI = key.x;
        if (key.y < minJ)
            minJ = key.y;
        if (key.x > maxI)
            maxI = key.x;
        if (key.y > maxJ)
            maxJ = key.y;
    }
    int midI = floor((minI + maxI) * 0.5);
    int midJ = floor((minJ + maxJ) * 0.5);

    std::string attributeName = "Shape Value";
    int vgaAttrColIdx = vgaMap.addAttribute(attributeName);

    SECTION("Data map") {
        mgraph->addShapeMap("Test ShapeMap");
        ShapeMap &sourceMap = mgraph->getDataMaps().back();

        int sourceAttrColIdx = sourceMap.addAttribute(attributeName);

        for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
            REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == -1);
        }

        SECTION("Single polygon input") {

            // main testing shape. drawn in a way so that it contains the pixels
            // that are not on the outer-edge of the pointmap
            sourceMap.makePolyShape(
                {
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMinY + cellSize * 1.5 - minorOffset)  //
                },
                false);

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);

            mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_MAX);

            // all values are 1 (like the polygon) except from those on the edges
            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = 1;
                if (key.x == minI || key.x == maxI || key.y == minJ || key.y == maxJ) {
                    expectedValue = -1;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Two overlapping polygons input", "") {

            // left polygon
            sourceMap.makePolyShape(
                {
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset),  //
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset),  //
                    Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, vgaMinY + cellSize * 1.5 - minorOffset)  //
                },
                false);

            // right polygon
            sourceMap.makePolyShape(
                {
                    Point2f((vgaMinX + vgaMaxX) * 0.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f((vgaMinX + vgaMaxX) * 0.5 - minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset),  //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMinY + cellSize * 1.5 - minorOffset)   //
                },
                false);

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);
            sourceMap.getAttributeTable().getRow(AttributeKey(1)).setValue(sourceAttrColIdx, 2);

            SECTION("Shared border max function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MAX);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = 1;
                    if (key.x == minI || key.x == maxI || key.y == minJ || key.y == maxJ) {
                        // cells on the border
                        expectedValue = -1;
                    } else if (key.x > midI) {
                        // cells on the right polygon
                        expectedValue = 2;
                    } else if (key.x == midI) {
                        // cells on the shared border get the MAXIMUM value from the two polygons
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Shared border min function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MIN);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = 1;
                    if (key.x == minI || key.x == maxI || key.y == minJ || key.y == maxJ) {
                        // cells on the border
                        expectedValue = -1;
                    } else if (key.x > midI) {
                        // cells on the right polygon
                        expectedValue = 2;
                    } else if (key.x == midI) {
                        // cells on the shared border get the MINIMUM value from the two polygons
                        expectedValue = 1;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Shared border average function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_AVG);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = 1;
                    if (key.x == minI || key.x == maxI || key.y == minJ || key.y == maxJ) {
                        // cells on the border
                        expectedValue = -1;
                    } else if (key.x > midI) {
                        // cells on the right polygon
                        expectedValue = 2;
                    } else if (key.x == midI) {
                        // cells on the shared border get the AVERAGE value of the two polygons
                        expectedValue = 1.5;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Shared border total function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_TOT);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = 1;
                    if (key.x == minI || key.x == maxI || key.y == minJ || key.y == maxJ) {
                        // cells on the border
                        expectedValue = -1;
                    } else if (key.x > midI) {
                        // cells on the right polygon
                        expectedValue = 2;
                    } else if (key.x == midI) {
                        // cells on the shared border get the TOTAL value of the two polygons
                        expectedValue = 3;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }
        }

        SECTION("Single line input", "") {

            // vertical line
            sourceMap.makeLineShape({
                Point2f((vgaMinX + vgaMaxX) * 0.5 - minorOffset, vgaMinY + minorOffset), //
                Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, vgaMaxY - minorOffset)  //
            });

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);

            mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_MAX);

            // all values are -1 except from those under the line (1)
            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == midI) {
                    expectedValue = 1;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Crossing lines input", "") {

            // vertical line
            sourceMap.makeLineShape({
                Point2f((vgaMinX + vgaMaxX) * 0.5 - minorOffset, vgaMinY + minorOffset), //
                Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, vgaMaxY - minorOffset)  //
            });

            // horizontal line
            sourceMap.makeLineShape({
                Point2f(vgaMinX + minorOffset, (vgaMinY + vgaMaxY) * 0.5 - minorOffset), //
                Point2f(vgaMaxX - minorOffset, (vgaMinY + vgaMaxY) * 0.5 + minorOffset)  //
            });

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);
            sourceMap.getAttributeTable().getRow(AttributeKey(1)).setValue(sourceAttrColIdx, 2);

            SECTION("Crossing lines max function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MAX);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if (key.x == midI && key.y == midJ) {
                        // cells on line intersection get the MAXIMUM value of the two
                        expectedValue = 2;
                    } else if (key.x == midI) {
                        // vertical line
                        expectedValue = 1;
                    } else if (key.y == midJ) {
                        // horizontal line
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing lines min function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MIN);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if (key.x == midI && key.y == midJ) {
                        // cells on line intersection get the MINIMUM value of the two
                        expectedValue = 1;
                    } else if (key.x == midI) {
                        // vertical line
                        expectedValue = 1;
                    } else if (key.y == midJ) {
                        // horizontal line
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing lines average function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_AVG);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if (key.x == midI && key.y == midJ) {
                        // cells on line intersection get the AVERAGE value of the two
                        expectedValue = 1.5;
                    } else if (key.x == midI) {
                        // vertical line
                        expectedValue = 1;
                    } else if (key.y == midJ) {
                        // horizontal line
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing lines total function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_TOT);

                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if (key.x == midI && key.y == midJ) {
                        // cells on line intersection get the TOTAL value of the two
                        expectedValue = 3;
                    } else if (key.x == midI) {
                        // vertical line
                        expectedValue = 1;
                    } else if (key.y == midJ) {
                        // horizontal line
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }
        }

        SECTION("Single open polyline input", "") {

            // L shape
            sourceMap.makePolyShape(
                {
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset)  //
                },
                true);

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);

            mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_MAX);

            // all values are -1 except from those under the line (1)
            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == minI || key.y == maxJ) {
                    expectedValue = 1;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Crossing open polylines input", "") {

            // L shape
            sourceMap.makePolyShape(
                {
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset)  //
                },
                true);

            // L shape rotated 180 degrees
            sourceMap.makePolyShape(
                {
                    Point2f(vgaMinX + cellSize * 1.5 - minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMinY + cellSize * 1.5 - minorOffset), //
                    Point2f(vgaMaxX - cellSize * 1.5 + minorOffset, vgaMaxY - cellSize * 1.5 + minorOffset)  //
                },
                true);

            sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);
            sourceMap.getAttributeTable().getRow(AttributeKey(1)).setValue(sourceAttrColIdx, 2);

            SECTION("Crossing open polylines max function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MAX);

                // all values are -1 except from those under the line (1)
                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if ((key.x == minI && key.y == minJ) || (key.x == maxI && key.y == maxJ)) {
                        // cells on line intersection get the MAXIMUM value of the two
                        expectedValue = 2;
                    } else if (key.x == minI || key.y == maxJ) {
                        // L shape
                        expectedValue = 1;
                    } else if (key.x == maxI || key.y == minJ) {
                        // L shape rotated 180 degrees
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing open polylines min function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_MIN);

                // all values are -1 except from those under the line (1)
                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if ((key.x == minI && key.y == minJ) || (key.x == maxI && key.y == maxJ)) {
                        // cells on line intersection get the MINUMUM value of the two
                        expectedValue = 1;
                    } else if (key.x == minI || key.y == maxJ) {
                        // L shape
                        expectedValue = 1;
                    } else if (key.x == maxI || key.y == minJ) {
                        // L shape rotated 180 degrees
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing open polylines average function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_AVG);

                // all values are -1 except from those under the line (1)
                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if ((key.x == minI && key.y == minJ) || (key.x == maxI && key.y == maxJ)) {
                        // cells on line intersection get the AVERAGE value of the two
                        expectedValue = 1.5;
                    } else if (key.x == minI || key.y == maxJ) {
                        // L shape
                        expectedValue = 1;
                    } else if (key.x == maxI || key.y == minJ) {
                        // L shape rotated 180 degrees
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }

            SECTION("Crossing open polylines total function") {
                mgraph->pushValuesToLayer(MetaGraph::VIEWDATA, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx,
                                          vgaAttrColIdx, MetaGraph::PUSH_FUNC_TOT);

                // all values are -1 except from those under the line (1)
                for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                    PixelRef key(vgaRowIter->getKey().value);
                    float expectedValue = -1;
                    if ((key.x == minI && key.y == minJ) || (key.x == maxI && key.y == maxJ)) {
                        // cells on line intersection get the TOTAL value of the two
                        expectedValue = 3;
                    } else if (key.x == minI || key.y == maxJ) {
                        // L shape
                        expectedValue = 1;
                    } else if (key.x == maxI || key.y == minJ) {
                        // L shape rotated 180 degrees
                        expectedValue = 2;
                    }
                    REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
                }
            }
        }
    }

    SECTION("Axial map") {
        // the pushValues function takes the base shapemap so these sections are mainly to test
        // sending ShapeGraphs and setting the source map type to MetaGraph::VIEWAXIAL

        mgraph->addShapeGraph("Test Axial Map", ShapeMap::AXIALMAP);
        ShapeGraph &sourceMap = *mgraph->getShapeGraphs().back().get();

        sourceMap.init(2, mgraph->getRegion());
        sourceMap.initialiseAttributesAxial();

        for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
            REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == -1);
        }

        // vertical line
        sourceMap.makeLineShape({
            Point2f((vgaMinX + vgaMaxX) * 0.5 - minorOffset, vgaMinY + minorOffset), //
            Point2f((vgaMinX + vgaMaxX) * 0.5 + minorOffset, vgaMaxY - minorOffset)  //
        });

        // horizontal line
        sourceMap.makeLineShape({
            Point2f(vgaMinX + minorOffset, (vgaMinY + vgaMaxY) * 0.5 - minorOffset), //
            Point2f(vgaMaxX - minorOffset, (vgaMinY + vgaMaxY) * 0.5 + minorOffset)  //
        });

        int sourceAttrColIdx = sourceMap.addAttribute(attributeName);

        sourceMap.getAttributeTable().getRow(AttributeKey(0)).setValue(sourceAttrColIdx, 1);
        sourceMap.getAttributeTable().getRow(AttributeKey(1)).setValue(sourceAttrColIdx, 2);

        sourceMap.makeConnections();

        REQUIRE(sourceMap.getAttributeTable().hasColumn("Connectivity"));

        SECTION("Crossing lines max function") {
            mgraph->pushValuesToLayer(MetaGraph::VIEWAXIAL, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_MAX);

            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == midI && key.y == midJ) {
                    // cells on line intersection get the MAXIMUM value of the two
                    expectedValue = 2;
                } else if (key.x == midI) {
                    // vertical line
                    expectedValue = 1;
                } else if (key.y == midJ) {
                    // horizontal line
                    expectedValue = 2;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Crossing lines min function") {
            mgraph->pushValuesToLayer(MetaGraph::VIEWAXIAL, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_MIN);

            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == midI && key.y == midJ) {
                    // cells on line intersection get the MINIMUM value of the two
                    expectedValue = 1;
                } else if (key.x == midI) {
                    // vertical line
                    expectedValue = 1;
                } else if (key.y == midJ) {
                    // horizontal line
                    expectedValue = 2;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Crossing lines average function") {
            mgraph->pushValuesToLayer(MetaGraph::VIEWAXIAL, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_AVG);

            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == midI && key.y == midJ) {
                    // cells on line intersection get the AVERAGE value of the two
                    expectedValue = 1.5;
                } else if (key.x == midI) {
                    // vertical line
                    expectedValue = 1;
                } else if (key.y == midJ) {
                    // horizontal line
                    expectedValue = 2;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }

        SECTION("Crossing lines total function") {
            mgraph->pushValuesToLayer(MetaGraph::VIEWAXIAL, 0, MetaGraph::VIEWVGA, 0, sourceAttrColIdx, vgaAttrColIdx,
                                      MetaGraph::PUSH_FUNC_TOT);

            for (auto vgaRowIter = vgaTable.begin(); vgaRowIter != vgaTable.end(); vgaRowIter++) {
                PixelRef key(vgaRowIter->getKey().value);
                float expectedValue = -1;
                if (key.x == midI && key.y == midJ) {
                    // cells on line intersection get the TOTAL value of the two
                    expectedValue = 3;
                } else if (key.x == midI) {
                    // vertical line
                    expectedValue = 1;
                } else if (key.y == midJ) {
                    // horizontal line
                    expectedValue = 2;
                }
                REQUIRE(vgaRowIter->getRow().getValue(vgaAttrColIdx) == expectedValue);
            }
        }
    }
}
