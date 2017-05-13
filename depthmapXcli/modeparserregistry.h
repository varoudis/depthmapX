#ifndef MODEPARSERREGISTRY_H
#define MODEPARSERREGISTRY_H

#include "imodeparser.h"
#include <vector>
#include <memory>

namespace ModeParserRegistry
{
    std::vector<std::unique_ptr<IModeParser>> populateParsers();
}

#define REGISTER_PARSER(parser)\
    availableParsers.push_back(std::unique_ptr<IModeParser>(new parser));

#endif // MODEPARSERREGISTRY_H
