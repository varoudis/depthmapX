#ifndef VGAPARSER_H
#define VGAPARSER_H

#include <string>

class VgaParser
{
public:
    VgaParser(size_t argc, char *argv[]);

    enum VgaMode{
        NONE,
        ISOVIST,
        VISBILITY,
        METRIC,
        ANGULAR
    };

    // vga options
    VgaMode getVgaMode() const { return _vgaMode; }
    bool localMeasures() const { return _localMeasures; }
    bool globalMeasures() const { return _globalMeasures; }
    const std::string & getRadius() const { return _radius; }

private:
    // vga options
    VgaMode _vgaMode;
    bool _localMeasures;
    bool _globalMeasures;
    std::string _radius;
};

#endif // VGAPARSER_H
