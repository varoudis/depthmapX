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

#include <iostream>
#include "commandlineparser.h"
#include "runmethods.h"

using namespace std;

int main(int argc, char *argv[])
{
    try{
        CommandLineParser args(argc, argv);
        if (!args.isValid())
        {
            CommandLineParser::printHelp();
            return 0;
        }

        if ( args.getMode() == DepthmapMode::VGA_ANALYSIS)
        {
            RadiusConverter converter;
            dm_runmethods::runVga(args, converter);
        } else if ( args.getMode() == DepthmapMode::LINK_GRAPH)
        {
            dm_runmethods::linkGraph(args);
        }

    }
    catch( std::exception &e)
    {
        cout << e.what() << "\n";
        CommandLineParser::printHelp();
        return -1;
    }
    return 0;
}
