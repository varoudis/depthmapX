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

#ifndef LINKPARSER_H
#define LINKPARSER_H

#include "salalib/mgraph.h"
#include "imodeparser.h"
#include "commandlineparser.h"
#include <string>
#include <vector>

class LinkParser : public IModeParser
{
public:
    enum MapTypeGroup { POINTMAPS, SHAPEGRAPHS };
    enum LinkMode { LINK, UNLINK };
    enum LinkType { COORDS, REFS };

    LinkParser():
        m_mapTypeGroup(POINTMAPS),
        m_linkMode(LINK),
        m_linkType(COORDS)
    {}

    virtual std::string getModeName() const
    {
        return "LINK";
    }

    virtual std::string getHelp() const
    {
        return  "Mode options for LINK:\n"\
                "  -lmt <type> Map type group to select displayed map from. One of:\n"\
                "       pointmaps (default, vga: link)\n"\
                "       shapegraphs (axial:link/unlink, segment:link, convex:link)\n"\
                "  -lm  <mode> one of:\n"\
                "       link (default)\n"\
                "       unlink\n"\
                "  -lt  <type> one of:\n"\
                "       coords (default, provided as x,y or x1,y1,x2,y2 coordinates)\n"\
                "       refs (provided as the ids (Ref) of the shapes)\n"\
                "  -lnk <single link/unlink coordinates> provided in csv (x1,y1,x2,y2)\n"\
                "       for example \"0.1,0.2,0.2,0.4\" to create a link from 0.1,0.2\n"\
                "       to 0.2,0.4. In the case of axial-map unlinks a single (x,y) set may\n"
                "       be provided. In the case of refs provide the ids in csv (reffrom,refto)"
                "       Provide multiple times for multiple links/unlinks\n"\
                "  -lf  <links file> as in -lnk\n";
    }

public:
    virtual void parse(int argc, char *argv[]);
    virtual void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const;

    //link options
    const std::string & getLinksFile() const { return m_linksFile; }
    const std::vector<std::string> & getManualLinks() const { return m_manualLinks; }
    const MapTypeGroup& getMapTypeGroup() const { return m_mapTypeGroup; }
    const LinkMode& getLinkMode() const { return m_linkMode; }
    const LinkType& getLinkType() const { return m_linkType; }
private:
    std::string m_linksFile;
    std::vector<std::string> m_manualLinks;
    MapTypeGroup m_mapTypeGroup;
    LinkMode m_linkMode;
    LinkType m_linkType;
};

#endif // LINKPARSER_H
