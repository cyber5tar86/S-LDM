/**
 * @file areafilter.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef AREAFILTER_H
#define AREAFILTER_H


#include <memory>
#include <nlohmann/json.hpp>

extern "C"
{
#include "options.h"
}

/**
 * @brief 
 * 
 */
class AreaFilterConfiguration;
class AreaFilter;

/**
 * @brief 
 * 
 */
typedef std::shared_ptr<AreaFilterConfiguration> AreaFilterConfigurationPtr;
typedef std::shared_ptr<AreaFilter> AreaFilterPtr;

/**
 * @brief 
 * 
 */
class AreaFilterConfigurationKeys
{
public:
    static constexpr char minLat[] = "minLat";              //!< 
    static constexpr char minLon[] = "minLon";              //!< 
    static constexpr char maxLat[] = "maxLat";              //!< 
    static constexpr char maxLon[] = "maxLon";              //!< 
    static constexpr char extLatFactor[] = "extLatFactor";  //!< 
    static constexpr char extLonFactor[] = "extLonFactor";  //!< 
};

/**
 * @brief 
 * 
 */
class AreaFilterConfiguration
{
public:
    /**
     * @brief Construct a new Area Filter Configuration object
     * 
     */
    AreaFilterConfiguration() = delete;

    /**
     * @brief Construct a new Area Filter Configuration object
     * 
     * @param configuration 
     */
    explicit AreaFilterConfiguration(const nlohmann::json &configuration);

    /**
     * @brief Construct a new Area Filter Configuration object
     * 
     * @param configuration 
     */
    explicit AreaFilterConfiguration(const std::string &configuration);

    /**
     * @brief Destroy the Area Filter Configuration object
     * 
     */
    ~AreaFilterConfiguration() = default;

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & minLat();

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & minLon();

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & maxLat();

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & maxLon();

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & extLatFactor();

    /**
     * @brief 
     * 
     * @return double& 
     */
    double & extLonFactor();

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string toString() const;

    /**
     * @brief 
     * 
     * @return nlohmann::json 
     */
    nlohmann::json toJson() const;

    /**
     * @brief 
     * 
     * @param configuration 
     */
    void fromString(const std::string &configuration);

    /**
     * @brief 
     * 
     * @param configuration 
     */
    void fromJson(const nlohmann::json &configuration);

private:
    double mMinLat{0.0};        //!< 
    double mMinLon{0.0};        //!< 
    double mMaxLat{0.0};        //!< 
    double mMaxLon{0.0};        //!< 

    // Extended area parameters (additive factors to the min/max lat and lon values of the internal area)
    double mExtLatFactor{0.0};  //!< 
    double mExtLonFactor{0.0};  //!< 
};

/**
 * @brief 
 * 
 */
class areaFilter
{
public:
    /**
     * @brief Construct a new area Filter object
     * 
     */
    areaFilter();

    /**
     * @brief Construct a new Area Filter object
     * 
     * @param configuration AreaFilter configuration
     */
    explicit AreaFilter(AreaFilterConfigurationPtr configuration);

    /**
     * @brief Set the AreaFilter configuration object
     *        This function expects the mConfiguration pointer to be non-nullptr, otherwise it always returns 'false'
     * 
     * @param configuration AreaFilter configuration
     */
    void setConfiguration(const AreaFilterConfigurationPtr& configuration);

    /**
     * @brief This function expects the m_opts_ptr pointer to be non-NULL, otherwise it always returns 'false'
     *        This function checks if the specified lat and lon value are within the S-LDM coverage area specified
     *        by the user and stored inside the options structure (pointed by m_opts_ptr)
     * 
     * @param lat 
     * @param lon 
     * @return true 
     * @return false 
     */
    bool isInside(double lat, double lon);

    /**
     * @brief This function works as isInside()
     * 
     * @param lat 
     * @param lon 
     * @return true  if the specified lat and lon values are located inside the internal area
     *         of the S-LDM (thus excluding the external area)
     * @return false otherwise
     */
    bool isInsideInternal(double lat, double lon);

private:
    AreaFilterConfigurationPtr mConfiguration{nullptr}; //!<
};

#endif // AREAFILTER_H