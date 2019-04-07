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

#pragma once

#include "imodeparser.h"
#include "genlib/p2dpoly.h"
#include <vector>

class VisPrepParser : public IModeParser
{
public:
    VisPrepParser() : m_grid(-1.0), m_maxVisibility(-1.0), m_boundaryGraph(false), m_makeGraph(false), m_unmakeGraph(false), m_removeLinksWhenUnmaking(false)
    {}

    virtual std::string getModeName() const
    {
        return "VISPREP";
    }

    virtual std::string getHelp() const
    {
        return "Mode options for VISPREP (visual analysis preparation) are:\n"  \
               "  -pg <gridsize> floating point number defining the grid spacing. If this\n" \
               "      is provided it will create a new map\n" \
               "  -pp <fillpoint> point where to fill. Can be repeated\n" \
               "  -pf <fillpoint file> a file with a point per line to fill\n" \
               "  -pr <max visibility> restrict visibility (-1 is unrestricted, default)\n" \
               "  -pb Make boundary graph\n" \
               "  -pm Make graph\n" \
               "  -pu Unmake graph\n" \
               "  -pl Remove links when unmaking\n";
    }

    virtual void parse(int argc, char** argv);

    virtual void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const;

    double getGrid() const { return m_grid; }
    std::vector<Point2f> getFillPoints() const { return m_fillPoints; }
    bool getBoundaryGraph() const { return m_boundaryGraph; }
    double getMaxVisibility() const { return m_maxVisibility; }
    bool getMakeGraph() const { return m_makeGraph; }
    bool getUnmakeGraph() const { return m_unmakeGraph; }
    bool getRemoveLinksWhenUnmaking() const { return m_removeLinksWhenUnmaking; }

private:
    double m_grid;
    std::vector<Point2f> m_fillPoints;
    double m_maxVisibility;
    bool m_boundaryGraph;
    bool m_makeGraph;
    bool m_unmakeGraph;
    bool m_removeLinksWhenUnmaking;
};


