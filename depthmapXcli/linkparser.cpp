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
#include "salalib/textparser.h"
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
    std::string linksFile;
    std::vector<std::string> manualLinks;

    for ( size_t i = 1; i < argc;  )
    {
        if ( strcmp ("-lf", argv[i]) == 0)
        {
            if (!linksFile.empty())
            {
                throw CommandLineException(std::string("-lf can only be used once at the moment"));
            }
            else if (manualLinks.size() != 0)
            {
                throw CommandLineException(std::string("-lf can not be used in conjunction with -lnk"));
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException(std::string("-lf requires an argument"));
            }
            linksFile = argv[i];
        }
        else if ( strcmp ("-lnk", argv[i]) == 0)
        {
            if (!linksFile.empty())
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
            manualLinks.push_back(argv[i]);
        }
        ++i;
    }
    if ( manualLinks.size() == 0 && linksFile.empty())
    {
        throw CommandLineException(std::string("one of -lf or -lnk must be provided"));
    }

    if(!linksFile.empty())
    {
        std::ifstream linksStream(linksFile);
        if (!linksStream)
        {
            std::stringstream message;
            message << "Failed to load file " << linksFile << ", error " << flush;
            throw depthmapX::RuntimeException(message.str().c_str());
        }
        vector<Line> lines = text_parser::parseLines(linksStream, '\t');
        _mergeLines.insert(std::end(_mergeLines), std::begin(lines), std::end(lines));
    }
    else if(!manualLinks.empty())
    {
        std::stringstream linksStream;
        linksStream << "x1\ty1\tx2\ty2";
        std::vector<std::string>::iterator iter = manualLinks.begin(), end =
        manualLinks.end();
        for ( ; iter != end; ++iter )
        {
            linksStream << "\n";
            std::replace( iter->begin(), iter->end(), ',', '\t'),
            linksStream << *iter;
        }
        vector<Line> lines = text_parser::parseLines(linksStream, '\t');
        _mergeLines.insert(std::end(_mergeLines), std::begin(lines), std::end(lines));
    }
}
