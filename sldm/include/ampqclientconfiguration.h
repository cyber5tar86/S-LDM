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
         * @param connectionURL_ 
         * @param address_ 
         * @param latitudeMin_ 
         * @param latitudeMax_ 
         * @param longitudeMin_ 
         * @param longitudeMax_ 
         */
        AMQPClientConfiguration(std::string connectionURL_, const std::string &address_, const double &latitudeMin_, const double &latitudeMax_, const double &longitudeMin_, const double &longitudeMax_) :
            : connectionURL(std::move(connectionURL_)),
              address(std::move(address_)),
              latitudeMin(std::move(latitudeMin_)),
              latitudeMax(std::move(latitudeMax_)),
              longitudeMin(std::move(longitudeMin_)),
              longitudeMax(std::move(longitudeMax_))
        {}

        /**
         * @brief Destroy the AMQPClientConfiguration object
         * 
         */
        ~AMQPClientConfiguration() = default;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(AMQPClientConfiguration, name, address, age)

    private:
        std::string mLogTag{"[AMQPClient] "};   

        std::string connectionURL{""};
        std::string address{""};
        double latitudeMin{0.0};
        double latitudeMax{0.0};
        double longitudeMin{0.0};
        double longitudeMax{0.0};
}

#endif /* AMQPCLIENTCONFIGURATION_H */