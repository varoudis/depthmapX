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
        return (m_mode & ALLLINES) != 0;
    }

    bool runFewestLines() const
    {
        return (m_mode & FEWEST) != 0;
    }

    bool runUnlink() const
    {
        return (m_mode & UNLINK) != 0;
    }

    bool runAnalysis() const
    {
        return (m_mode & ANALYSIS) != 0;
    }


private:
    enum AXIAL_MODE
    {
        NONE = 0,
        ALLLINES = 1,
        FEWEST = 2,
        UNLINK = 4,
        ANALYSIS = 8
    };

    int m_mode;
};
