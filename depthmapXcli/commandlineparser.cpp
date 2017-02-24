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
              << "Possible modes are:\n  VGA\n"
              << "Mode options for VGA:\n"
              << "-vm <vga mode> one of isovist, visiblity, metric, angular\n"
              << "-vg turn on global measures for visibility, requires radius between 1 and 99 or n\n"
              << "-vl turn on local measures for visibility\n"
              << "-vr set visibility radius\n"
              << std::flush;
}



CommandLineParser::CommandLineParser( size_t argc, char *argv[] )
    : _mode(DepthmapMode::NONE), _simpleMode(false), _vgaParser(nullptr)
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
}

const VgaParser& CommandLineParser::vgaOptions() const
{
    if ( _vgaParser == nullptr )
    {
        throw CommandLineException("VGA options are not available when mode is not VGA");
    }
    return *_vgaParser;
}
