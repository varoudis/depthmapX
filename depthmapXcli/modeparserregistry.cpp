#include "modeparserregistry.h"
#include "linkparser.h"
#include "vgaparser.h"


std::vector<std::unique_ptr<IModeParser>> ModeParserRegistry::populateParsers()
{
    std::vector<std::unique_ptr<IModeParser>> availableParsers;
    // Register any mode parsers here
    REGISTER_PARSER(VgaParser);
    REGISTER_PARSER(LinkParser);
    // *********
    return availableParsers;
}
