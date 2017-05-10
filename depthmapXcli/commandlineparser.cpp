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

using namespace depthmapX;

void CommandLineParser::printHelp(){
    std::cout << "Usage: depthmapXcli -m <mode> -f <filename> -o <output file> [-s] [mode options]\n"
              << "       depthmapXcli -h prints this help text\n"
              << "-s enables simple mode\n"
              << "-t <times.csv> enables output of runtimes as csv file"

              << "Possible modes are:\n  VGA\n  LINK\n"
              << "Mode options for LINK:\n"
              << "-lf <links file>\n"
              << "-lnk <single link coordinates> provided in csv (x1,y1,x2,y2) for example \"0.1,0.2,0.2,0.4\" "
              << "to create a link from 0.1,0.2 to 0.2,0.4. Provide multiple times for multiple links\n"
              << "Mode options for VGA:\n"
              << "-vm <vga mode> one of isovist, visiblity, metric, angular, thruvision\n"
              << "-vg turn on global measures for visibility, requires radius between 1 and 99 or n\n"
              << "-vl turn on local measures for visibility\n"
              << "-vr set visibility radius\n"
              << std::flush;
}



CommandLineParser::CommandLineParser( size_t argc, char *argv[] )
    : _mode(DepthmapMode::NONE), _simpleMode(false), _linkParser(nullptr), _vgaParser(nullptr)
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
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-m requires an argument");
            }
            if ( strcmp(argv[i], "VGA") == 0 )
            {
                _mode = DepthmapMode::VGA_ANALYSIS;
            }
            else if ( strcmp(argv[i], "LINK") == 0 )
            {
                _mode = DepthmapMode::LINK_GRAPH;
            }
            else
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

    if (_mode == DepthmapMode::NONE)
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

    if (_mode == DepthmapMode::VGA_ANALYSIS)
    {
        _vgaParser = new VgaParser(argc, argv);
    }
    if (_mode == DepthmapMode::LINK_GRAPH)
    {
        _linkParser = new LinkParser(argc, argv);
    }
}

const VgaParser& CommandLineParser::vgaOptions() const
{
    if ( _vgaParser == nullptr )
    {
        throw CommandLineException("VGA options are not available when mode is not VGA");
    }
    return *_vgaParser;
}
const LinkParser& CommandLineParser::linkOptions() const
{
    if ( _linkParser == nullptr )
    {
        throw CommandLineException("LINK options are not available when mode is not LINK");
    }
    return *_linkParser;
}

CommandLineParser::~CommandLineParser()
{
    if ( _vgaParser != nullptr)
    {
        delete _vgaParser;
    }
    if ( _linkParser != nullptr)
    {
        delete _linkParser;
    }
}
