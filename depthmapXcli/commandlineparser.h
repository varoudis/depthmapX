#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <string>
#include "vgaparser.h"

enum DepthmapMode{
    NONE = 0,
    VGA_ANALYSIS
};



class CommandLineParser
{
public:
    CommandLineParser(size_t argc, char *argv[]);
    ~CommandLineParser();

    DepthmapMode getMode() const { return _mode; }
    const std::string &getFileName() const { return _fileName; }
    const std::string &getOuputFile() const {return _outputFile;}
    bool isValid() const { return _valid; }
    bool simpleMode() const { return _simpleMode; }
    const VgaParser& vgaOptions() const;

    static void printHelp();



private:
    DepthmapMode _mode;
    std::string _fileName;
    std::string _outputFile;
    bool _valid;
    bool _simpleMode;

    const VgaParser * _vgaParser;

};

#endif // COMMANDLINEPARSER_H
