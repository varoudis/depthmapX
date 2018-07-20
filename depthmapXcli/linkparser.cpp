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
#include "runmethods.h"
#include "parsingutils.h"
#include <cstring>
#include <memory>
#include <sstream>

using namespace depthmapX;

void LinkParser::parse(int argc, char *argv[])
{
    for ( int i = 1; i < argc;  )
    {
        if ( std::strcmp ("-lmt", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-lmt", i)
            if ( std::strcmp(argv[i], "pointmaps") == 0 )
            {
                _mapTypeGroup = MapTypeGroup::POINTMAPS;
            }
            else if ( std::strcmp(argv[i], "shapegraphs") == 0 )
            {
                _mapTypeGroup = MapTypeGroup::SHAPEGRAPHS;
            }
            else
            {
                throw CommandLineException(std::string("Invalid LINK map type group: ") + argv[i]);
            }
        }
        else if ( std::strcmp ("-lm", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-lm", i)
            if ( std::strcmp(argv[i], "link") == 0 )
            {
                _linkMode = LinkMode::LINK;
            }
            else if ( std::strcmp(argv[i], "unlink") == 0 )
            {
                _linkMode = LinkMode::UNLINK;
            }
            else
            {
                throw CommandLineException(std::string("Invalid LINK mode: ") + argv[i]);
            }
        }
        else if ( std::strcmp ("-lt", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-lt", i)
            if ( std::strcmp(argv[i], "coords") == 0 )
            {
                _linkType = LinkType::COORDS;
            }
            else if ( std::strcmp(argv[i], "refs") == 0 )
            {
                _linkType = LinkType::REFS;
            }
            else
            {
                throw CommandLineException(std::string("Invalid LINK type: ") + argv[i]);
            }
        }
        else if ( std::strcmp ("-lf", argv[i]) == 0)
        {
            if (!_linksFile.empty())
            {
                throw CommandLineException("-lf can only be used once at the moment");
            }
            else if (_manualLinks.size() != 0)
            {
                throw CommandLineException("-lf can not be used in conjunction with -lnk");
            }
            ENFORCE_ARGUMENT("-lf", i)
            _linksFile = argv[i];
        }
        else if ( std::strcmp ("-lnk", argv[i]) == 0)
        {
            if (!_linksFile.empty())
            {
                throw CommandLineException("-lf can not be used in conjunction with -lnk");
            }
            ENFORCE_ARGUMENT("-lnk", i)
            if (!has_only_digits_dots_commas(argv[i]))
            {
                std::stringstream message;
                message << "Invalid link provided ("
                        << argv[i]
                        << "). Should only contain digits dots and commas"
                        << std::flush;
                throw CommandLineException(message.str().c_str());
            }
            _manualLinks.push_back(argv[i]);
        }
        ++i;
    }
    if ( _manualLinks.size() == 0 && _linksFile.empty())
    {
        throw CommandLineException("one of -lf or -lnk must be provided");
    }
    if (_mapTypeGroup == MapTypeGroup::POINTMAPS && _linkMode == LinkMode::UNLINK)
    {
        throw CommandLineException("unlinking is not supported for pointmaps");
    }
}

void LinkParser::run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const
{
    dm_runmethods::linkGraph(clp, *this, perfWriter);
}
