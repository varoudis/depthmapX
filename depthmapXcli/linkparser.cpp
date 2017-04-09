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

#include "linkparser.h"
#include "salalib/mgraph.h"
#include "exceptions.h"
#include <cstring>
#include <memory>
#include <sstream>

using namespace depthmapX;

namespace {
    bool has_only_digits_dots_commas(const std::string &s){
      return s.find_first_not_of( "0123456789,." ) == std::string::npos;
    }
}

LinkParser::LinkParser(size_t argc, char *argv[])
{
    for ( size_t i = 1; i < argc;  )
    {
        if ( strcmp ("-lf", argv[i]) == 0)
        {
            if (!_linksFile.empty())
            {
                throw CommandLineException(std::string("-lf can only be used once at the moment"));
            }
            else if (_manualLinks.size() != 0)
            {
                throw CommandLineException(std::string("-lf can not be used in conjunction with -lnk"));
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException(std::string("-lf requires an argument"));
            }
            _linksFile = argv[i];
        }
        else if ( strcmp ("-lnk", argv[i]) == 0)
        {
            if (!_linksFile.empty())
            {
                throw CommandLineException(std::string("-lnk can not be used in conjunction with -lf"));
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException(std::string("-lnk requires an argument"));
            }
            if (!has_only_digits_dots_commas(argv[i]))
            {
                throw CommandLineException(std::string("Invalid link provided: ") + argv[i]);
            }
            _manualLinks.push_back(argv[i]);
        }
        ++i;
    }
    if ( _manualLinks.size() == 0 && _linksFile.empty())
    {
        throw CommandLineException(std::string("one of -lf or -lnk must be provided"));
    }
}
