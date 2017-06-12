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

#include "radiusconverter.h"
#include "exceptions.h"
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace depthmapX;

double RadiusConverter::ConvertForVisibility(const std::string &radius) const
{
    if (radius == "n")
    {
        return -1.0;
    }
    char *end;
    long rad = std::strtol(radius.c_str(), &end, 10);
    if (rad < 1 || rad > 99)
    {
        throw SetupCheckException(std::string("Radius for visibility analysis must be n for the whole range or an integer between 1 and 99 inclusive. Got ") + radius);
    }
    return static_cast<double>(rad);
}

double RadiusConverter::ConvertForMetric(const std::string &radius) const
{
    if (radius == "n")
    {
        return -1.0;
    }
    char *end;
    double rad = strtod(radius.c_str(), &end);
    if ( rad <= 0 )
    {
        throw SetupCheckException(std::string("Radius for metric vga must be n for the whole range or a positive number. Got ") + radius);
    }
    if (std::isnan(rad))
    {
        throw SetupCheckException("Radius NaN?! Really?");
    }
    if (std::isinf(rad))
    {
        throw SetupCheckException("Radius inf?! Who are you kidding?");
    }

    return rad;
}
