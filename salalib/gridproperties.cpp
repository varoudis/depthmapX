#include "gridproperties.h"
#include <math.h>

GridProperties::GridProperties(double maxDimension)
{
    int maxexponent = (int) floor(log10(maxDimension)) - 1;
    int minexponent = maxexponent - 2;
    int mantissa = (int) floor(maxDimension / pow(10.0,double(maxexponent+1)));

    m_default = (double) mantissa * pow(10.0, double(maxexponent - 1));
    m_max = (double) 2 * mantissa * pow(10.0, double(maxexponent));
    m_min = (double) mantissa * pow(10.0, double(minexponent));
}
