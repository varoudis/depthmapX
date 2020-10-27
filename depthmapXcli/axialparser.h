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

class AxialParser : public IModeParser
{
public:
    AxialParser();

    // IModeParser interface
public:
    std::string getModeName() const;
    std::string getHelp() const;
    void parse(int argc, char **argv);
    void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const;

    // accessors
    bool runAllLines() const
    {
        return !m_allAxesRoots.empty();
    }

    const std::vector<Point2f> & getAllAxesRoots() const{
        return m_allAxesRoots;
    }

    bool runFewestLines() const
    {
        return m_runFewestLines;
    }

    bool runUnlink() const
    {
        // not supported for now
        return false;
    }

    bool runAnalysis() const
    {
        return m_runAnalysis;
    }

    bool useChoice() const { return m_choice; }
    bool useLocal() const { return m_local; }
    bool calculateRRA() const { return m_rra; }

    const std::vector<double>& getRadii() const { return m_radii;}
    const std::string getAttribute() const { return m_attribute;}

private:
    std::vector<Point2f> m_allAxesRoots;
    bool m_runFewestLines;
    bool m_runAnalysis;
    std::vector<double> m_radii;
    bool m_choice;
    bool m_local;
    bool m_rra;
    std::string m_attribute;
};
