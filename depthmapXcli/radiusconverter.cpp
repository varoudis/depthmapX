#include "radiusconverter.h"
#include <sstream>
#include <cstdlib>
#include <cmath>

double RadiusConverter::ConvertForVisibility(const std::string &radius) const
{
    if (radius == "n")
    {
        return -1.0;
    }
    char *end;
    long rad = std::strtol(radius.c_str(), &end, 10);
    if (rad < 1 || rad > 99)
    {
        std::stringstream message;
        message << "Radius for visibility analysis must be n for the whole range or an integer between 1 and 99 inclusive. Got " << radius << std::flush;
        throw std::exception(message.str().c_str());
    }
    return static_cast<double>(rad);
}

double RadiusConverter::ConvertForMetric(const std::string &radius) const
{
    if (radius == "n")
    {
        return -1.0;
    }
    char *end;
    double rad = strtod(radius.c_str(), &end);
    if ( rad <= 0 )
    {
        std::stringstream message;
        message << "Radius for metric vga must be n for the whole range or a positive number. Got " << radius << std::flush;
        throw std::exception(message.str().c_str());
    }
    if (isnan(rad))
    {
        throw std::exception("Radius NaN?! Really?");
    }
    if (isinf(rad))
    {
        throw std::exception("Radius inf?! Who are you kidding?");
    }

    return rad;
}
