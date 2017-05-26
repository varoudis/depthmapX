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


#include "axialparser.h"
#include "parsingutils.h"
#include "exceptions.h"

using namespace depthmapX;

AxialParser::AxialParser() : m_mode((int) AxialParser::NONE)
{

}


std::string AxialParser::getModeName() const
{
    return "AXIAL";
}

std::string AxialParser::getHelp() const
{
    return  "Mode options for Axial Analysis:\n"\
            "-am <axial mode> one of all, fewest, unlink, analysis\n"\
            "    all - create all axial lines map\n"\
            "    fewest - reduce to fewest axial lines\n"\
            "    unlink - perform line unlinks (requires extra information)\n"\
            "    analysis - run axial analysis (requies extra information)\n"\
            "    Any mode expects input in the in graph\n"\
            "    -am can be used several times, the operations will be run in the above order\n"\
            "";
}

void AxialParser::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-am") == 0)
        {
            ENFORCE_ARGUMENT("-am", i)
            std::string arg(argv[i]);
            if ( arg == "all")
            {
                m_mode |= ALLLINES;
            }
            else if ( arg == "fewest")
            {
                m_mode |= FEWEST;
            }
            else if ( arg == "unlink")
            {
                m_mode |= UNLINK;
            }
            else if (arg == "analysis")
            {
                m_mode |= ANALYSIS;
            }
        }
    }

    if (m_mode == (int)NONE)
    {
        throw CommandLineException("No axial analysis mode present");
    }
}

void AxialParser::run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const
{
}
