#include <iostream>
#include "commandlineparser.h"
#include "runmethods.h"

using namespace std;

int main(int argc, char *argv[])
{
    try{
        CommandLineParser args(argc, argv);
        if (!args.isValid())
        {
            CommandLineParser::printHelp();
            return 0;
        }

        if ( args.getMode() == DepthmapMode::VGA_ANALYSIS)
        {
            RadiusConverter converter;
            dm_runmethods::runVga(args, converter);
        }

    }
    catch( std::exception &e)
    {
        cout << e.what() << "\n";
        CommandLineParser::printHelp();
        return -1;
    }
    return 0;
}
