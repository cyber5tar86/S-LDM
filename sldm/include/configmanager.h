/**
 * @file configmanager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <memory>

class ConfigManager;
using ConfigManagerPtr = std::shared_ptr<ConfigManager>;

class ConfigManager
{
public:
    ConfigManager();
    
    static ConfigManagerPtr
    getInstance();

    AreaFilterConfiguration
    getAreaFilterConfiguration() const;

    ManeuveringServiceConfiguration
    getManeuveringServiceConfiguration() const;

    VehicleVisualizerConfiguration
    getVehicleVisualizerConfiguration() const;

    AmqpConfiguration
    getClientConfiguration() const;

    AmqpClientConfiguration
    getAmqpClientConfiguration() const;

private:
}