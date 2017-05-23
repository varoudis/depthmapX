#include "modeparserregistry.h"
#include "linkparser.h"
#include "vgaparser.h"
#include "visprepparser.h"


void ModeParserRegistry::populateParsers()
{
    // Register any mode parsers here
    REGISTER_PARSER(VgaParser);
    REGISTER_PARSER(LinkParser);
    REGISTER_PARSER(VisPrepParser);
    // *********
}
