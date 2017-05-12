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

// Interface to encapsulate handling command line and invoking the respective
// depthmapX mode

#include "performancesink.h"
#include "commandlineparser.h"

class IModeParser
{
public:
    virtual std::string getModeName() const = 0;
    virtual std::string getHelp() const = 0;
    virtual void parse( int argc, char **argv) = 0;
    virtual void run(const CommandLineParser &clp, IPerformanceSink &perfWriter) const = 0;
    virtual ~IModeParser(){}
};
