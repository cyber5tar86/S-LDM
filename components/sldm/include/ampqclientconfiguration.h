/**
 * @file ampqclientconfiguration.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef AMQPCLIENTCONFIGURATION_H
#define AMQPCLIENTCONFIGURATION_H

#include <memory>
#include <nlohmann/json.hpp>
#include "ldmmap.h"

class AMQPClientConfiguration;
using AMQPClientConfigurationPtr = std::shared_ptr<AMQPClientConfiguration>;

/**
 * @brief 
 * 
 */
class AMQPClientConfiguration
{
    public:

        /**
         * @brief Construct a new AMQPClientConfiguration object
         * 
         */
        AMQPClientConfiguration() = default;

        /**
         * @brief Construct a new AMQPClientConfiguration object
         * 
         * @param connectionURL
         * @param address
         * @param latitudeMin
         * @param latitudeMax
         * @param longitudeMin
         * @param longitudeMax
         * @param enableLogInfo
         */
        AMQPClientConfiguration(std::string connectionURL, const std::string &address,
                                const double &latitudeMin, const double &latitudeMax,
                                const double &longitudeMin, const double &longitudeMax,
                                bool enableLogInfo)
            : mConnectionURL(std::move(connectionURL)),
              mAddress(std::move(address)),
              mLatitudeMin(std::move(latitudeMin)),
              mLatitudeMax(std::move(latitudeMax)),
              mLongitudeMin(std::move(longitudeMin)),
              mLongitudeMax(std::move(longitudeMax)),
              mEnableLogInfo(enableLogInfo)
        {}

        /**
         * @brief Destroy the AMQPClientConfiguration object
         * 
         */
        ~AMQPClientConfiguration() = default;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(AMQPClientConfiguration, mConnectionURL, mAddress,
                                                    mLatitudeMin, mLatitudeMax,
                                                    mLongitudeMin, mLongitudeMax,
                                                    mEnableLogInfo)

        std::string
        connectionURL() const
        {
            return mConnectionURL;
        }

        std::string
        address() const
        {
            return mAddress;
        }

        double
        latitudeMin() const
        {
            return mLatitudeMin;
        }

        double
        latitudeMax() const
        {
            return mLatitudeMax;
        }

        double
        longitudeMin() const
        {
            return mLongitudeMin;
        }

        double
        longitudeMax() const
        {
            return mLongitudeMax;
        }

        bool
        enableLogInfo() const
        {
            return mEnableLogInfo;
        }

    private:
        std::string mConnectionURL{""};
        std::string mAddress{""};
        double mLatitudeMin{0.0};
        double mLatitudeMax{0.0};
        double mLongitudeMin{0.0};
        double mLongitudeMax{0.0};
        bool mEnableLogInfo{false};
};

#endif /* AMQPCLIENTCONFIGURATION_H */