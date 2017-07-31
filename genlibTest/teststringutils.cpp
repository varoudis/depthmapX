#include "catch.hpp"
#include "../genlib/stringutils.h"

TEST_CASE("Tests for split function", "")
{
    {
        std::vector<std::string> stringParts = dXstring::split("foo,bar",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "foo");
        REQUIRE(stringParts[1] == "bar");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5,1.2",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5\t1.2",'\t');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5\t1.2\tfoo",'\t');
        REQUIRE(stringParts.size() == 3);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
        REQUIRE(stringParts[2] == "foo");
    }

    {
        // skip last blank element
        std::vector<std::string> stringParts = dXstring::split("foo,bar,",',');
        REQUIRE(stringParts.size() == 2);
    }

    {
        // do not skip middle blank element
        std::vector<std::string> stringParts = dXstring::split("foo,,bar",',');
        REQUIRE(stringParts.size() == 3);
    }
}
