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

#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include "genlib/p2dpoly.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <string>

namespace textParser {
    std::vector<Line> parseLines(std::istream& stream, char delimiter);

    template<typename Out>
    void split(const std::string &s, char delim, Out result);
    std::vector<std::string> split(const std::string &s, char delim);
    double stringToDouble( const std::string& s );
}

#endif // TEXTPARSER_H
