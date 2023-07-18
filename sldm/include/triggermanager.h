/**
 * @file triggermanager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef TRIGGERMANAGER_H
#define TRIGGERMANAGER_H

#include <mutex>
#include <list>

#include "ldmmap.h"
extern "C"
{
#include "CAM.h"
#include "options.h"
}

class IndicatorTriggerManager;
using IndicatorTriggerManagerPtr = std::shared_ptr<IndicatorTriggerManager>;

/**
 * @brief Simple, indicator-based, trigger manager
 * 
 */
class IndicatorTriggerManager
{
public:
    /**
     * @brief Construct a new indicator Trigger Manager object
     * 
     * @param db_ptr 
     * @param opts_ptr 
     */
    IndicatorTriggerManager(const LDMMapPtr &db, options_t *opts) :
        m_db(db),
        m_opts(opts)
    {}

    void setDBpointer(const LDMMapPtr &db)
    {
        m_db = db;
    }

    /**
     * @brief 
     * 
     * @param lat 
     * @param lon 
     * @param refVehStationID 
     * @param exteriorLightsStatus 
     * @return true 
     * @return false 
     */
    bool checkAndTrigger(double lat, double lon, uint64_t refVehStationID, uint8_t exteriorLightsStatus);

    /**
     * @brief 
     * 
     * @param stationID 
     */
    void notifyOpTermination(uint64_t stationID);

    /**
     * @brief Set the Left Turn Indicator Enable object
     *        If set to true, the REST client will consider as triggering condition also the left indicator, other then the right one (default behaviour)
     * 
     * @param m_enable 
     */
    void setLeftTurnIndicatorEnable(bool m_enable) { m_left_indicator_enabled = m_enable; }

protected:
    /*! @brief */
    std::mutex m_already_triggered_mutex;

    /*! @brief List of vehicles for which the operations after triggering are in progress */
    std::list<uint64_t> m_already_triggered; // 

private:
    /*! @brief */
    LDMMapPtr m_db{nullptr};

    /*! @brief */
    options_t *m_opts{nullptr};

    /*! @brief */
    bool m_left_indicator_enabled{false};
};

#endif // TRIGGERMANAGER_H