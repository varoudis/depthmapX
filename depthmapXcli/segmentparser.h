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
#include "genlib/p2dpoly.h"

class SegmentParser : public IModeParser
{
public:
    SegmentParser();

    // IModeParser interface
public:
    std::string getModeName() const;
    std::string getHelp() const;
    void parse(int argc, char **argv);
    void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const;

    enum class AnalysisType {
        NONE,
        ANGULAR_TULIP,
        ANGULAR_FULL,
        TOPOLOGICAL,
        METRIC
    };

    enum class RadiusType {
        NONE,
        SEGMENT_STEPS,
        ANGULAR,
        METRIC
    };

    AnalysisType getAnalysisType() const { return m_analysisType; }

    RadiusType getRadiusType() const { return m_radiusType; }

    bool includeChoice() const { return m_includeChoice; }

    int getTulipBins() const { return m_tulipBins; }

    const std::vector<double> getRadii() const { return m_radii;}

    const std::string getAttribute() const { return m_attribute;}

private:
    AnalysisType m_analysisType;
    RadiusType m_radiusType;
    bool m_includeChoice;
    int m_tulipBins;
    std::vector<double> m_radii;
    std::string m_attribute;
};
