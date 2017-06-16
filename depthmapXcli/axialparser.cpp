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
#include "salalib/entityparsing.h"
#include "runmethods.h"

using namespace depthmapX;

AxialParser::AxialParser() :  m_runFewestLines(false), m_runAnalysis(false)
{

}


std::string AxialParser::getModeName() const
{
    return "AXIAL";
}

std::string AxialParser::getHelp() const
{
    return  "Mode options for Axial Analysis:\n"\
            "  -xl <x>,<y> Calculate all lines map from this seed point (can be used more than once)\n"
            "  -xf Calculate fewest lines map from all lines map\n"\
            "  -xu Process unlink data (not yet supported)\n"\
            "  -xa run axial anlysis\n"\
            " All modes expect to find the required input in the in graph\n"\
            " Any combination of flags above can be specified, they will always be run in the order -aa -af -au -ax\n";
}

void AxialParser::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-xl") == 0)
        {
            ENFORCE_ARGUMENT("-xl", i)
            m_allAxesRoots.push_back(EntityParsing::parsePoint(argv[i]));
        }
        else if(strcmp(argv[i], "-xf") == 0)
        {
            m_runFewestLines = true;
        }
        else if(strcmp(argv[i], "-xu") == 0)
        {
            ENFORCE_ARGUMENT("-xu", i)
        }
        else if (strcmp(argv[i], "-xa") == 0)
        {
            ENFORCE_ARGUMENT("-xa", i)
            if (m_runAnalysis)
            {
                throw CommandLineException("-xa can only be used once");
            }
            m_radii = depthmapX::parseAxialRadiusList(argv[i]);
            m_runAnalysis = true;
        }
    }

    if (!runAllLines() && !runFewestLines() && !runUnlink() && !runAnalysis())
    {
        throw CommandLineException("No axial analysis mode present");
    }
}

void AxialParser::run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const
{
    dm_runmethods::runAxialAnalysis(clp, *this, perfWriter);
}
