#include "commandlineparser.h"
#include <iostream>
#include <sstream>


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


namespace {
    bool has_only_digits(const std::string &s){
      return s.find_first_not_of( "0123456789" ) == std::string::npos;
    }
}

CommandLineParser::CommandLineParser( size_t argc, char *argv[] ) : _mode(DepthmapMode::NONE), _vgaMode(VgaMode::NONE_VGA), _simpleMode(false)
{
    if (argc <= 1)
    {
        throw std::exception("No commandline parameters provided - don't know what to do");
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
                throw std::exception("-m requires an argument");
            }
            if ( strcmp(argv[i], "VGA") == 0 )
            {
                _mode = DepthmapMode::VGA_ANALYSIS;
            }
            else
            {
                throw std::exception(std::string(std::string("Invalid mode: ") + argv[i]).c_str());
            }
        }
        else if ( strcmp ("-f", argv[i]) == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw std::exception("-f requires an argument");
            }
            _fileName = argv[i];
        }
        else if ( strcmp ("-o", argv[i]) == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw std::exception("-o requires an argument");
            }
            _outputFile = argv[i];
        }
        else if ( strcmp("-s", argv[i]) == 0)
        {
            _simpleMode = true;
        }
        else if ( strcmp ("-vm", argv[i]) == 0)
        {
            if (_vgaMode != VgaMode::NONE_VGA)
            {
                throw std::exception("-vm can only be used once, modes are mutually exclusive");
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw std::exception("-vm requires an argument");
            }
            if ( strcmp(argv[i], "isovist") == 0 )
            {
                _vgaMode = VgaMode::ISOVIST;
            }
            else if ( strcmp(argv[i], "visibility") == 0 )
            {
                _vgaMode = VgaMode::VISBILITY;
            }
            else if ( strcmp(argv[i], "metric") == 0 )
            {
                _vgaMode = VgaMode::METRIC;
            }
            else if ( strcmp(argv[i], "angular") == 0 )
            {
                _vgaMode = VgaMode::ANGULAR;
            }
            else
            {
                throw std::exception(std::string(std::string("Invalid VGA mode: ") + argv[i]).c_str());
            }
        }
        else if ( strcmp(argv[i], "-vg") == 0 )
        {
            _globalMeasures = true;
        }
        else if ( strcmp(argv[i], "-vl") == 0 )
        {
            _globalMeasures = true;
        }
        else if (strcmp(argv[i], "-vr") == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw std::exception("-vr requires an argument");
            }
            _radius = argv[i];
        }
        ++i;
    }

    if (_mode == DepthmapMode::NONE)
    {
        throw std::exception("-m for mode is required");
    }
    if (_fileName.empty())
    {
        throw std::exception("-f for input file is required");
    }
    if (_outputFile.empty())
    {
        throw std::exception("-o for output file is required");
    }

    if (_mode == DepthmapMode::VGA_ANALYSIS)
    {
        if(_vgaMode == VgaMode::NONE_VGA)
        {
            _vgaMode = VgaMode::ISOVIST;
        }

        if (_vgaMode == VgaMode::VISBILITY && _globalMeasures)
        {
            if (_radius.empty())
            {
                throw std::exception("Global measures in VGA/visibility analysis require a radius, use -vr <radius>");
            }
            if (_radius != "n" && !has_only_digits(_radius))
            {
                std::stringstream message;
                message << "Radius must be a positive integer number or n, got " << _radius << std::flush;
                throw std::exception(message.str().c_str());
            }

        }
        else if (_vgaMode == VgaMode::METRIC)
        {
            if (_radius.empty())
            {
                throw std::exception("Metric vga requires a radius, use -vr <radius>");
            }
        }
    }


}
