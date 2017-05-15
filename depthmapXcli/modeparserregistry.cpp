#include "modeparserregistry.h"
#include "linkparser.h"
#include "vgaparser.h"


void ModeParserRegistry::populateParsers()
{
    // Register any mode parsers here
    REGISTER_PARSER(VgaParser);
    REGISTER_PARSER(LinkParser);
    // *********
}
