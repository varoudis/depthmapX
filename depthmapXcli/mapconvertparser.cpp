// Copyright (C) 2018 Petros Koutsolampros

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


#include "mapconvertparser.h"
#include "parsingutils.h"
#include "exceptions.h"
#include "runmethods.h"
#include <cstring>

using namespace depthmapX;

void MapConvertParser::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if ( std::strcmp ("-co", argv[i]) == 0)
        {
            if (m_outMapType != ShapeMap::EMPTYMAP)
            {
                throw CommandLineException("-co can only be used once, modes are mutually exclusive");
            }
            ENFORCE_ARGUMENT("-co", i)
            if ( std::strcmp(argv[i], "drawing") == 0 )
            {
                m_outMapType = ShapeMap::DRAWINGMAP;
            }
            else if ( std::strcmp(argv[i], "axial") == 0 )
            {
                m_outMapType = ShapeMap::AXIALMAP;
            }
            else if ( std::strcmp(argv[i], "segment") == 0 )
            {
                m_outMapType = ShapeMap::SEGMENTMAP;
            }
            else if ( std::strcmp(argv[i], "data") == 0 )
            {
                m_outMapType = ShapeMap::DATAMAP;
            }
            else if ( std::strcmp(argv[i], "convex") == 0)
            {
                m_outMapType = ShapeMap::CONVEXMAP;
            }
            else
            {
                throw CommandLineException(std::string("Invalid map output (-co) type: ") + argv[i]);
            }
        }
        else if(std::strcmp(argv[i], "-con") == 0)
        {
            ENFORCE_ARGUMENT("-con", i)
            m_outMapName = argv[i];
        }
        else if(std::strcmp(argv[i], "-cir") == 0)
        {
            m_removeInputMap = true;
        }
        else if (std::strcmp(argv[i], "-coc") == 0)
        {
            m_copyAttributes = true;
        }
        else if (std::strcmp(argv[i], "-crsl") == 0)
        {
            ENFORCE_ARGUMENT("-crsl", i)
            if (!has_only_digits_dots(argv[i]))
            {
                throw CommandLineException(std::string("-crsl must be a number >0, got ") + argv[i]);
            }
            m_removeStubLengthPRC = std::stod(argv[i]);
            if (!(m_removeStubLengthPRC > 0))
            {
                throw CommandLineException(std::string("-crsl must be a number >0, got ") + argv[i]);
            }
        }
    }

    if (m_outMapType == ShapeMap::EMPTYMAP)
    {
        throw CommandLineException("A valid output map type (-co) is required");
    }

    if (m_outMapName == "")
    {
        throw CommandLineException("A valid output map name (-con) is required");
    }
}

void MapConvertParser::run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const
{
    dm_runmethods::runMapConversion(clp, *this, perfWriter);
}
