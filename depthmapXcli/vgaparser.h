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

#ifndef VGAPARSER_H
#define VGAPARSER_H

#include <string>

class VgaParser
{
public:
    VgaParser(size_t argc, char *argv[]);

    enum VgaMode{
        NONE,
        ISOVIST,
        VISBILITY,
        METRIC,
        ANGULAR,
        THRU_VISION
    };

    // vga options
    VgaMode getVgaMode() const { return _vgaMode; }
    bool localMeasures() const { return _localMeasures; }
    bool globalMeasures() const { return _globalMeasures; }
    const std::string & getRadius() const { return _radius; }

private:
    // vga options
    VgaMode _vgaMode;
    bool _localMeasures;
    bool _globalMeasures;
    std::string _radius;
};

#endif // VGAPARSER_H
