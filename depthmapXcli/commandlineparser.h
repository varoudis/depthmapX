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
