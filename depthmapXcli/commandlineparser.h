#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <string>


enum DepthmapMode{
    NONE = 0,
    VGA_ANALYSIS
};

enum VgaMode{
    NONE_VGA,
    ISOVIST,
    VISBILITY,
    METRIC,
    ANGULAR
};

class CommandLineParser
{
public:
    CommandLineParser(size_t argc, char *argv[]);
    DepthmapMode getMode() const { return _mode; }
    const std::string &getFileName() const { return _fileName; }
    const std::string &getOuputFile() const {return _outputFile;}
    bool isValid() const { return _valid; }
    bool simpleMode() const { return _simpleMode; }

    // vga options
    VgaMode getVgaMode() const { return _vgaMode; }
    bool localMeasures() const { return _localMeasures; }
    bool globalMeasures() const { return _globalMeasures; }
    const std::string & getRadius() const { return _radius; }

    static void printHelp();

private:
    DepthmapMode _mode;
    std::string _fileName;
    std::string _outputFile;
    bool _valid;
    bool _simpleMode;

    // vga options
    VgaMode _vgaMode;
    bool _localMeasures;
    bool _globalMeasures;
    std::string _radius;
};

#endif // COMMANDLINEPARSER_H
