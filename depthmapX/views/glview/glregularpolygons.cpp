// depthmapX - spatial network analysis platform
// Copyright (C) 2018, Petros Koutsolampros

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

#include "glregularpolygons.h"
#include <math.h>

void GLRegularPolygons::loadPolygonData(const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                                        const int sides, const float radius) {
    init(colouredPoints.size() * sides * 3);
    std::vector<Point2f> points(sides * 3);
    float angle = 2 * M_PI / sides;
    for (int i = 0; i < sides; i++) {
        points[i * 3] = Point2f(cos((i)*angle) * radius, sin((i)*angle) * radius);
        points[i * 3 + 1] = Point2f(cos((i + 1) * angle) * radius, sin((i + 1) * angle) * radius);
        points[i * 3 + 2] = Point2f(0, 0);
    }

    Point2f prevCentre(0, 0);
    for (const auto &colouredPoint : colouredPoints) {
        const Point2f &centre = colouredPoint.first;
        float r = colouredPoint.second.redf();
        float g = colouredPoint.second.greenf();
        float b = colouredPoint.second.bluef();

        for (Point2f &point : points) {
            point.x -= prevCentre.x;
            point.y -= prevCentre.y;
            point.x += centre.x;
            point.y += centre.y;
            add(QVector3D(point.x, point.y, 0.0f), QVector3D(r, g, b));
        }
        prevCentre = centre;
    }
}
