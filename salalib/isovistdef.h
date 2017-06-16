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

#pragma once

#include "genlib/p2dpoly.h"

class IsovistDefinition
{
public:
    IsovistDefinition( double x, double y, double angle, double viewAngle ) : mLocation(x, y), mAngle(angle), mViewAngle(viewAngle)
    {
        if ( viewAngle >= 2 * M_PI)
        {
            mAngle = 0.0;
            mViewAngle = 0.0;
        }
    }

    IsovistDefinition(double x, double y) : mLocation(x,y), mAngle(0), mViewAngle(0)
    {}

    const Point2f &getLocation() const { return mLocation;}
    double getAngle() const { return mAngle; }
    double getViewAngle() const { return mViewAngle; }
    double getLeftAngle() const {
        double leftAngle = mAngle - 0.5 * mViewAngle;
        if (leftAngle < 0 )
        {
            leftAngle += 2 * M_PI;
        }
        return leftAngle;
    }

    double getRightAngle() const{
        double rightAngle = mAngle + 0.5 * mViewAngle;
        if (rightAngle > 2 * M_PI)
        {
            rightAngle -= 2 * M_PI;
        }
        return rightAngle;
    }

private:
    Point2f mLocation;
    double mAngle;
    double mViewAngle;
};
