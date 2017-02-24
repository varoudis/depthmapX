#ifndef RUNMETHODS_H
#define RUNMETHODS_H
#include <string>
#include "commandlineparser.h"
#include "radiusconverter.h"

namespace dm_runmethods{
    class RuntimeException : public std::exception
    {

    };

    void runVga(const CommandLineParser &cmdP, const IRadiusConverter &converter);
}
#endif // RUNMETHODS_H
