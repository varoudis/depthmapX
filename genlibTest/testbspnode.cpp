// Copyright (C) 2017 Petros Koutsolampros

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
#include "genlib/comm.h"
#include "genlib/p2dpoly.h"
#include "genlib/bspnode.h"

TEST_CASE("BSPNode::pickMidpointLine")
{
    std::vector<TaggedLine> lines;
    lines.push_back(TaggedLine(Line(Point2f(1, 2), Point2f(2, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(2, 2), Point2f(3, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(3, 2), Point2f(4, 2)), 0));

    BSPNode node;

    REQUIRE(node.pickMidpointLine(lines, 0) == 1);

    SECTION("Additional lines") {
        lines.push_back(TaggedLine(Line(Point2f(4, 2), Point2f(5, 2)), 0));
        REQUIRE(node.pickMidpointLine(lines, 0) == 1);

        lines.push_back(TaggedLine(Line(Point2f(5, 1), Point2f(6, 1)), 0));
        REQUIRE(node.pickMidpointLine(lines, 0) == 2);

        // the only line with height > width becomes chosen
        lines.push_back(TaggedLine(Line(Point2f(15, 4), Point2f(15, 0)), 0));
        REQUIRE(node.pickMidpointLine(lines, 0) == 5);
    }
    SECTION("rotated middle") {

        // height > width, rotated, close to midpoint
        lines.push_back(TaggedLine(Line(Point2f(4.5, 1), Point2f(4.5, 3)), 0));

        lines.push_back(TaggedLine(Line(Point2f(5, 2), Point2f(6, 2)), 0));
        lines.push_back(TaggedLine(Line(Point2f(6, 2), Point2f(7, 2)), 0));

        // height > width, rotated, not close to midpoint
        lines.push_back(TaggedLine(Line(Point2f(6.5, 1), Point2f(6.5, 3)), 0));

        REQUIRE(node.pickMidpointLine(lines, 0) == 3);
    }
}

void compareLines(Line l1, Line l2, float EPSILON) {
    REQUIRE(l1.start().x == Approx(l2.start().x).epsilon(EPSILON));
    REQUIRE(l1.start().y == Approx(l2.start().y).epsilon(EPSILON));
    REQUIRE(l1.end().x == Approx(l2.end().x).epsilon(EPSILON));
    REQUIRE(l1.end().y == Approx(l2.end().y).epsilon(EPSILON));
}

TEST_CASE("BSPNode::makeLines")
{
    const float EPSILON = 0.001;
    typedef std::pair<std::vector<TaggedLine>, std::vector<TaggedLine> > TagLineVecPair;

    std::vector<TaggedLine> lines;
    lines.push_back(TaggedLine(Line(Point2f(1, 2), Point2f(2, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(2, 2), Point2f(3, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(3, 2), Point2f(4, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(4, 2), Point2f(5, 2)), 0));

    BSPNode node;

    TagLineVecPair result = node.makeLines(0, 0, lines, 0);

    REQUIRE(result.first.size() == 3);
    REQUIRE(result.second.size() == 0);

    compareLines(result.first[0].line, lines[0].line, EPSILON);
    compareLines(result.first[1].line, lines[2].line, EPSILON);
    compareLines(result.first[2].line, lines[3].line, EPSILON);


    SECTION("One on the right")
    {
        lines.push_back(TaggedLine(Line(Point2f(5, 1), Point2f(6, 1)), 0));

        result = node.makeLines(0, 0, lines, 0);

        REQUIRE(result.first.size() == 3);
        REQUIRE(result.second.size() == 1);

        compareLines(result.second[0].line, lines[4].line, EPSILON);
    }
    SECTION("One line with height > width becomes chosen")
    {
        // height > width, rotated, not close to midpoint
        lines.push_back(TaggedLine(Line(Point2f(5.5, 1), Point2f(5.5, 3)), 0));

        lines.push_back(TaggedLine(Line(Point2f(6, 2), Point2f(7, 2)), 0));

        result = node.makeLines(0, 0, lines, 0);

        REQUIRE(result.first.size() == 4);
        REQUIRE(result.second.size() == 1);

        compareLines(result.first[0].line, lines[0].line, EPSILON);
        compareLines(result.first[1].line, lines[1].line, EPSILON);
        compareLines(result.first[2].line, lines[2].line, EPSILON);
        compareLines(result.first[3].line, lines[3].line, EPSILON);
        compareLines(result.second[0].line, lines[5].line, EPSILON);
    }

    SECTION("One broken between")
    {
        // height > width, rotated, close to midpoint
        lines.push_back(TaggedLine(Line(Point2f(5.5, 1), Point2f(5.5, 3)), 0));

        lines.push_back(TaggedLine(Line(Point2f(6, 2), Point2f(7, 2)), 0));
        lines.push_back(TaggedLine(Line(Point2f(7, 2), Point2f(8, 2)), 0));
        lines.push_back(TaggedLine(Line(Point2f(8, 2), Point2f(9, 2)), 0));
        lines.push_back(TaggedLine(Line(Point2f(9, 2), Point2f(10, 2)), 0));

        // line with two points at different sides of chosen
        lines.push_back(TaggedLine(Line(Point2f(3, -2), Point2f(6, -2)), 0));

        result = node.makeLines(0, 0, lines, 0);

        // adds one on each side
        REQUIRE(result.first.size() == 5);
        REQUIRE(result.second.size() == 5);

        compareLines(result.first[0].line, lines[0].line, EPSILON);
        compareLines(result.first[1].line, lines[1].line, EPSILON);
        compareLines(result.first[2].line, lines[2].line, EPSILON);
        compareLines(result.first[3].line, lines[3].line, EPSILON);

        compareLines(result.second[0].line, lines[5].line, EPSILON);
        compareLines(result.second[1].line, lines[6].line, EPSILON);
        compareLines(result.second[2].line, lines[7].line, EPSILON);
        compareLines(result.second[3].line, lines[8].line, EPSILON);

        compareLines(result.first[4].line, Line(Point2f(3, -2), Point2f(5.5, -2)), EPSILON);
        compareLines(result.second[4].line, Line(Point2f(5.5, -2), Point2f(6, -2)), EPSILON);
    }
}

TEST_CASE("BSPNode::make (all horizontal lines)", "all-left tree")
{
    const float EPSILON = 0.001;

    std::vector<TaggedLine> lines;
    lines.push_back(TaggedLine(Line(Point2f(1, 2), Point2f(2, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(2, 2), Point2f(3, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(3, 2), Point2f(4, 2)), 0));
    lines.push_back(TaggedLine(Line(Point2f(4, 2), Point2f(5, 2)), 0));

    std::unique_ptr<BSPNode> node(new BSPNode());

    BSPTree::make(0, 0, lines, node.get());

    compareLines(node->getLine(), lines[1].line, EPSILON);

    REQUIRE(node->left != NULL);
    REQUIRE(node->right == NULL);

    compareLines(node->left->getLine(), lines[2].line, EPSILON);

    REQUIRE(node->left->left != NULL);
    REQUIRE(node->left->right == NULL);

    compareLines(node->left->left->getLine(), lines[3].line, EPSILON);

    REQUIRE(node->left->left->left != NULL);
    REQUIRE(node->left->left->right == NULL);

    compareLines(node->left->left->left->getLine(), lines[0].line, EPSILON);

    REQUIRE(node->left->left->left->left == NULL);
    REQUIRE(node->left->left->left->right == NULL);
}

TEST_CASE("BSPNode::make (all vertical lines)", "split tree")
{
    const float EPSILON = 0.001;

    std::vector<TaggedLine> lines;
    lines.push_back(TaggedLine(Line(Point2f(1.5, 1), Point2f(1.5, 3)), 0));
    lines.push_back(TaggedLine(Line(Point2f(2.5, 1), Point2f(2.5, 3)), 0));
    lines.push_back(TaggedLine(Line(Point2f(3.5, 1), Point2f(3.5, 3)), 0));
    lines.push_back(TaggedLine(Line(Point2f(4.5, 1), Point2f(4.5, 3)), 0));

    std::unique_ptr<BSPNode> node(new BSPNode());

    BSPTree::make(0, 0, lines, node.get());

    compareLines(node->getLine(), lines[1].line, EPSILON);

    REQUIRE(node->left != NULL);
    REQUIRE(node->right != NULL);

    compareLines(node->left->getLine(), lines[0].line, EPSILON);
    compareLines(node->right->getLine(), lines[3].line, EPSILON);

    REQUIRE(node->left->left == NULL);
    REQUIRE(node->left->right == NULL);

    REQUIRE(node->right->left != NULL);
    REQUIRE(node->right->right == NULL);

    compareLines(node->right->left->getLine(), lines[2].line, EPSILON);

    REQUIRE(node->right->left->left == NULL);
    REQUIRE(node->right->left->right == NULL);
}
