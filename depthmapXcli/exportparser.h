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

#pragma once

#include "imodeparser.h"
#include "commandlineparser.h"
#include <string>

class ExportParser : public IModeParser
{
public:
    virtual std::string getModeName() const
    {
        return "EXPORT";
    }

    virtual std::string getHelp() const
    {
        return    "Mode options for EXPORT:\n"\
                  "-em <export mode> one of:\n"\
                  "    pointmap-data-csv\n"\
                  "    pointmap-connections-csv\n"\
                  "    pointmap-links-csv\n"\
                  "    shapegraph-map-csv\n"\
                  "    shapegraph-map-mif\n"\
                  "    shapegraph-connections-csv\n";
    }

public:
    ExportParser();
    virtual void parse(int argc, char *argv[]);
    virtual void run(const CommandLineParser &clp, IPerformanceSink& perfWriter) const;

    enum ExportMode{
        NONE,
        POINTMAP_DATA_CSV,
        POINTMAP_CONNECTIONS_CSV,
        POINTMAP_LINKS_CSV,
        SHAPEGRAPH_MAP_CSV,
        SHAPEGRAPH_MAP_MIF,
        SHAPEGRAPH_CONNECTIONS_CSV
    };
    ExportMode getExportMode() const { return m_exportMode; }

private:
    ExportMode m_exportMode;
};

