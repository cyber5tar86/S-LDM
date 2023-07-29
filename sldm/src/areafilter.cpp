#include "areafilter.h"

#include <utility>

AreaFilterConfiguration::AreaFilterConfiguration(const nlohmann::json &configuration)
{
    fromJson(configuration);
}

AreaFilterConfiguration::AreaFilterConfiguration(const std::string &configuration)
{
    fromString(configuration);
}

double &
AreaFilterConfiguration::minLat()
{
    return mMinLat;
}

double &
AreaFilterConfiguration::minLon()
{
    return mMinLon;
}

double &
AreaFilterConfiguration::maxLat()
{
    return mMaxLat;
}

double &
AreaFilterConfiguration::maxLon()
{
    return mMaxLon;
}

double &
AreaFilterConfiguration::extLatFactor()
{
    return mExtLatFactor;
}

double &
AreaFilterConfiguration::extLonFactor()
{
    return mExtLatFactor;
}

std::string
AreaFilterConfiguration::toString() const
{
    return to_string(toJson());
}

nlohmann::json
AreaFilterConfiguration::toJson() const
{
    return nlohmann::json{
        {AreaFilterConfigurationKeys::minLat, mMinLat},
        {AreaFilterConfigurationKeys::minLon, mMinLon},
        {AreaFilterConfigurationKeys::maxLat, mMaxLat},
        {AreaFilterConfigurationKeys::maxLon, mMaxLon},
        {AreaFilterConfigurationKeys::extLatFactor, mExtLatFactor},
        {AreaFilterConfigurationKeys::extLonFactor, mExtLonFactor}
    };
}

void
AreaFilterConfiguration::fromString(const std::string &configuration)
{
    fromJson(nlohmann::json::parse(configuration));
}

void
AreaFilterConfiguration::fromJson(const nlohmann::json &configuration)
{
    mMinLat = configuration.at(AreaFilterConfigurationKeys::minLat);
    mMinLon = configuration.at(AreaFilterConfigurationKeys::minLon);
    mMaxLat = configuration.at(AreaFilterConfigurationKeys::maxLat);
    mMaxLon = configuration.at(AreaFilterConfigurationKeys::maxLon);
    mExtLatFactor = configuration.at(AreaFilterConfigurationKeys::extLatFactor);
    mExtLonFactor = configuration.at(AreaFilterConfigurationKeys::extLonFactor);
}

AreaFilter::AreaFilter(AreaFilterConfigurationPtr configuration)
    : mConfiguration(std::move(configuration))
{}

AreaFilter::AreaFilter() = default;

bool
AreaFilter::setConfiguration(const AreaFilterConfigurationPtr &configuration)
{
    bool res{false};

    if (configuration != nullptr)
    {
        mConfiguration = configuration;
        res = true;
    }

    return res;
}

bool
AreaFilter::isInside(double lat, double lon)
{
    bool res{false};

    if (mConfiguration != nullptr)
    {
        if (lat >= mConfiguration->minLat() - mConfiguration->extLatFactor() && lat <= mConfiguration->maxLat() + mConfiguration->extLatFactor() &&
            lon >= mConfiguration->minLon() - mConfiguration->extLonFactor() && lon <= mConfiguration->maxLon() + mConfiguration->extLonFactor())
        {
            res = true;
        }
    }

    return res;
}

bool
AreaFilter::isInsideInternal(double lat, double lon)
{
    bool res{false};

    if (mConfiguration != nullptr)
    {
        if (lat >= mConfiguration->minLat() && lat <= mConfiguration->maxLat() &&
            lon >= mConfiguration->minLon() && lon <= mConfiguration->maxLon())
        {
            res = true;
        }
    }

    return false;
}