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
    std::string _linksFile;
    std::vector<std::string> _manualLinks;

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
                throw CommandLineException(std::string("-lf can not be used in conjunction with -lnk"));
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException(std::string("-lnk requires an argument"));
            }
            if (!has_only_digits_dots_commas(argv[i]))
            {
                throw CommandLineException(std::string("Invalid link provided (") + argv[i]
                                           + std::string("). Should only contain digits dots and commas"));
            }
            _manualLinks.push_back(argv[i]);
        }
        ++i;
    }
    if ( _manualLinks.size() == 0 && _linksFile.empty())
    {
        throw CommandLineException(std::string("one of -lf or -lnk must be provided"));
    }

    ShapeMap temp_shape_map = ShapeMap("temp_map", ShapeMap::LINEMAP);

    if(!_linksFile.empty())
    {
        std::ifstream links_stream(_linksFile);
        if (!links_stream)
        {
            std::stringstream message;
            message << "Failed to load file " << _linksFile << ", error " << flush;
            throw depthmapX::RuntimeException(message.str().c_str());
        }
        temp_shape_map.importTxt(links_stream, false);
    }
    else if(!_manualLinks.empty())
    {
        std::stringstream links_stream;
        links_stream << "x1\ty1\tx2\ty2";
        for(size_t i = 0; i < _manualLinks.size(); ++i)
        {
            links_stream << "\n";
            std::string s = _manualLinks.at(i);
            std::replace( s.begin(), s.end(), ',', '\t');
            links_stream << s;
        }
        temp_shape_map.importTxt(links_stream, false);
    }
    pqmap<int,SalaShape>& shapes = temp_shape_map.getAllShapes();
    for (size_t i = 0; i < shapes.size(); i++)
    {
        _mergeLines.push_back(shapes.value(i).getLine());
    }
}
