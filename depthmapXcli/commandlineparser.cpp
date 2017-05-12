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

#include "commandlineparser.h"
#include "exceptions.h"
#include <iostream>
//#include <sstream>
#include <cstring>
#include "vgaparser.h"
#include "linkparser.h"
#include <algorithm>

using namespace depthmapX;

std::vector<std::unique_ptr<IModeParser> > populateParsers()
{
    std::vector<std::unique_ptr<IModeParser> > result;
    result.push_back(std::move(std::unique_ptr<IModeParser>(new VgaParser)));
    result.push_back(std::move(std::unique_ptr<IModeParser>(new LinkParser)));
    return result;
}

static std::vector<std::unique_ptr<IModeParser> > AvailableParsers = populateParsers();




void CommandLineParser::printHelp(){
    std::cout << "Usage: depthmapXcli -m <mode> -f <filename> -o <output file> [-s] [mode options]\n"
              << "       depthmapXcli -h prints this help text\n"
              << "-s enables simple mode\n"
              << "-t <times.csv> enables output of runtimes as csv file\n"

              << "Possible modes are:\n  ";
              std::for_each(AvailableParsers.begin(), AvailableParsers.end(), [](auto &p)->void{ std::cout << "  " << p->getModeName() << "\n"; });
              std::for_each(AvailableParsers.begin(), AvailableParsers.end(), [](auto &p)->void{ std::cout << p->getHelp(); });
              std::cout << std::flush;
}



CommandLineParser::CommandLineParser( size_t argc, char *argv[] )
    :  _simpleMode(false), _modeParser(0)
{
    if (argc <= 1)
    {
        throw CommandLineException("No commandline parameters provided - don't know what to do");
    }
    _valid = true;
    for ( size_t i = 1; i < argc;  )
    {
        if ( strcmp("-h", argv[i])== 0)
        {
            _valid = false;
            return;
        }
        else if ( strcmp ("-m", argv[i]) == 0)
        {
            if ( _modeParser)
            {
                throw CommandLineException("-m can only be used once");
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-m requires an argument");
            }

            for (auto iter = AvailableParsers.begin(), end = AvailableParsers.end();
                 iter != end; ++iter )
            {
                if ((*iter)->getModeName() == argv[i])
                {
                    _modeParser = iter->get();
                    break;
                }
            }

            if (!_modeParser)
            {
                throw CommandLineException(std::string("Invalid mode: ") + argv[i]);
            }
        }
        else if ( strcmp ("-f", argv[i]) == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-f requires an argument");
            }
            _fileName = argv[i];
        }
        else if ( strcmp ("-o", argv[i]) == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-o requires an argument");
            }
            _outputFile = argv[i];
        }
        else if ( strcmp ("-t", argv[i]) == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-t requires an argument");
            }
            _timingFile = argv[i];
        }
        else if ( strcmp("-s", argv[i]) == 0)
        {
            _simpleMode = true;
        }
        ++i;
    }

    if (!_modeParser)
    {
        throw CommandLineException("-m for mode is required");
    }
    if (_fileName.empty())
    {
        throw CommandLineException("-f for input file is required");
    }
    if (_outputFile.empty())
    {
        throw CommandLineException("-o for output file is required");
    }
    _modeParser->parse(argc, argv);
}

void CommandLineParser::run(IPerformanceSink &perfWriter) const
{
    if (!_valid || !_modeParser)
    {
        throw CommandLineException("Trying to run with invalid command line parameters");
    }
    _modeParser->run(*this, perfWriter);
}

