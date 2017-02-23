#ifndef RADIUSCONVERTER_H
#define RADIUSCONVERTER_H
#include <string>

class IRadiusConverter
{
public:
    virtual double ConvertForVisibility(const std::string &radius) const = 0;
    virtual double ConvertForMetric(const std::string &radius) const = 0;
    virtual ~IRadiusConverter(){}
};


class RadiusConverter : public IRadiusConverter
{
public:
    virtual double ConvertForVisibility(const std::string &radius) const;
    virtual double ConvertForMetric(const std::string &radius) const;
};

#endif // RADIUSCONVERTER_H
