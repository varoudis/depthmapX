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

#pragma once
#include "imodeparser.h"
#include "salalib/shapemap.h"

class MapConvertParser : public IModeParser
{
public:
    MapConvertParser():
        m_outMapType(ShapeMap::EMPTYMAP),
        m_outMapName(""),
        m_removeInputMap(false),
        m_copyAttributes(false),
        m_removeStubLengthPRC(0)
    {}

    // IModeParser interface
public:
    std::string getModeName() const
    {
        return "MAPCONVERT";
    }

    std::string getHelp() const
    {
        return  "Mode options for Map Conversion:\n"\
                "  -co Output map type (to convert to)\n"\
                "      Possible input/output map types:\n"\
                "        - drawing\n"\
                "        - axial\n"\
                "        - segment\n"\
                "        - data\n"\
                "        - convex\n"\
                "  -con Output map name\n"\
                "  -cir Remove input map\n"\
                "  -coc Copy attributes to output map (Only between DATA, AXIAL and SEGMENT)\n"\
                "  -crsl <%> Percent of line length of axial stubs to remove (Only for AXIAL -> SEGMENT)\n\n";
    }
    void parse(int argc, char **argv);
    void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const;

    int outputMapType() const { return m_outMapType; }
    std::string outputMapName() const { return m_outMapName; }
    bool removeInputMap() const { return m_removeInputMap; }
    bool copyAttributes() const { return m_copyAttributes; }
    double removeStubLength() const { return m_removeStubLengthPRC; }

private:
    int m_outMapType;
    std::string m_outMapName;
    bool m_removeInputMap;
    bool m_copyAttributes;
    double m_removeStubLengthPRC;
};
