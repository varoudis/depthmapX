#include "vgaparser.h"
#include "exceptions.h"

using namespace depthmapX;

namespace {
    bool has_only_digits(const std::string &s){
      return s.find_first_not_of( "0123456789" ) == std::string::npos;
    }
}


VgaParser::VgaParser(size_t argc, char *argv[]) : _vgaMode(VgaMode::NONE), _globalMeasures(false), _localMeasures(false)
{
    for ( size_t i = 1; i < argc;  )
    {

        if ( strcmp ("-vm", argv[i]) == 0)
        {
            if (_vgaMode != VgaMode::NONE)
            {
                throw CommandLineException("-vm can only be used once, modes are mutually exclusive");
            }
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-vm requires an argument");
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
                throw CommandLineException(std::string("Invalid VGA mode: ") + argv[i]);
            }
        }
        else if ( strcmp(argv[i], "-vg") == 0 )
        {
            _globalMeasures = true;
        }
        else if ( strcmp(argv[i], "-vl") == 0 )
        {
            _localMeasures = true;
        }
        else if (strcmp(argv[i], "-vr") == 0)
        {
            if ( ++i >= argc || argv[i][0] == '-'  )
            {
                throw CommandLineException("-vr requires an argument");
            }
            _radius = argv[i];
        }
        ++i;
    }

    if(_vgaMode == VgaMode::NONE)
    {
        _vgaMode = VgaMode::ISOVIST;
    }

    if (_vgaMode == VgaMode::VISBILITY && _globalMeasures)
    {
        if (_radius.empty())
        {
            throw CommandLineException("Global measures in VGA/visibility analysis require a radius, use -vr <radius>");
        }
        if (_radius != "n" && !has_only_digits(_radius))
        {
            throw CommandLineException(std::string("Radius must be a positive integer number or n, got ") + _radius);
        }

    }
    else if (_vgaMode == VgaMode::METRIC)
    {
        if (_radius.empty())
        {
            throw CommandLineException("Metric vga requires a radius, use -vr <radius>");
        }
    }

}
