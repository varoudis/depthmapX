#include "catch.hpp"
#include "argumentholder.h"



TEST_CASE("Test ArgumentHolder", "Constructor")
{
    ArgumentHolder ah{"foo", "bar"};
    REQUIRE(ah.argc() == 2);
    REQUIRE(strcmp(ah.argv()[0], "foo") == 0 );
    REQUIRE(strcmp(ah.argv()[1], "bar") == 0 );
}

