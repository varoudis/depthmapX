#include "catch.hpp"
#include "../depthmapXcli/radiusconverter.h"

TEST_CASE("ConvertForMetric","")
{
    RadiusConverter converter;
    REQUIRE(converter.ConvertForMetric("2") == Approx(2.0));
    REQUIRE(converter.ConvertForMetric("n") == Approx(-1.0));
    REQUIRE(converter.ConvertForMetric("2.6") == Approx(2.6));
    REQUIRE(converter.ConvertForMetric("2.3e12") == Approx(2.3e12));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("-1"), Catch::Contains("Radius for metric vga must be n for the whole range or a positive number. Got -1"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("foo"), Catch::Contains("Radius for metric vga must be n for the whole range or a positive number. Got foo"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("NaN"), Catch::Contains("Radius NaN?! Really?"));
    REQUIRE_THROWS_WITH(converter.ConvertForMetric("INFINITY"), Catch::Contains("Radius inf?! Who are you kidding?"));
}
