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
#include "imodeparserfactory.h"
#include "parsingutils.h"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace depthmapX;

void CommandLineParser::printHelp(){
    std::cout << "Usage: depthmapXcli -m <mode> -f <filename> -o <output file> [-s] [mode options]\n"
              << "       depthmapXcli -h prints this help text\n"
              << "-s enables simple mode\n"
              << "-t <times.csv> enables output of runtimes as csv file\n"

              << "Possible modes are:\n";
              std::for_each(_parserFactory.getModeParsers().begin(), _parserFactory.getModeParsers().end(), [](const ModeParserVec::value_type &p)->void{ std::cout << "  " << p->getModeName() << "\n"; });
              std::cout << "\n";
              std::for_each(_parserFactory.getModeParsers().begin(), _parserFactory.getModeParsers().end(), [](const ModeParserVec::value_type &p)->void{ std::cout << p->getHelp() << "\n"; });
              std::cout << std::flush;
}



CommandLineParser::CommandLineParser(const IModeParserFactory &parserFactory)
    :  _simpleMode(false), _modeParser(0), _parserFactory(parserFactory)
{}

void CommandLineParser::parse(size_t argc, char *argv[])
{
    _valid = false;
    if (argc <= 1)
    {
        throw CommandLineException("No commandline parameters provided - don't know what to do");
    }
    for ( size_t i = 1; i < argc;  )
    {
        if ( std::strcmp("-h", argv[i])== 0)
        {
            return;
        }
        else if ( std::strcmp ("-m", argv[i]) == 0)
        {
            if ( _modeParser)
            {
                throw CommandLineException("-m can only be used once");
            }
            ENFORCE_ARGUMENT("-m", i)

            for (auto iter = _parserFactory.getModeParsers().begin(), end = _parserFactory.getModeParsers().end();
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
        else if ( std::strcmp ("-f", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-f", i)
            _fileName = argv[i];
        }
        else if ( std::strcmp ("-o", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-o", i)
            _outputFile = argv[i];
        }
        else if ( std::strcmp ("-t", argv[i]) == 0)
        {
            ENFORCE_ARGUMENT("-t", i)
            _timingFile = argv[i];
        }
        else if ( std::strcmp("-s", argv[i]) == 0)
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
    _valid = true;
}

void CommandLineParser::run(IPerformanceSink &perfWriter) const
{
    if (!_valid || !_modeParser)
    {
        throw CommandLineException("Trying to run with invalid command line parameters");
    }
    _modeParser->run(*this, perfWriter);
}

