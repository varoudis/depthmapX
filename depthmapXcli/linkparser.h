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

#include <string>
#include <vector>

class LinkParser
{
public:
    LinkParser(size_t argc, char *argv[]);

    //link options
    const std::string & getLinksFile() const { return _linksFile; }
    const std::vector<std::string> & getManualLinks() const { return _manualLinks; }

private:
    std::string _linksFile;
    std::vector<std::string> _manualLinks;
};

#endif // LINKPARSER_H
