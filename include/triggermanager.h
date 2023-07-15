#ifndef TRIGGERMAN_H
#define TRIGGERMAN_H

#include <mutex>
#include <list>

#include "LDMmap.h"
extern "C"
{
#include "CAM.h"
#include "options.h"
}

/**
 * @brief Simple, indicator-based, trigger manager
 * 
 */
class indicatorTriggerManager
{
public:
    /**
     * @brief Construct a new indicator Trigger Manager object
     * 
     * @param db_ptr 
     * @param opts_ptr 
     */
    indicatorTriggerManager(const std::shared_ptr<ldmmap::LDMMap> &db, options_t *opts) :
        m_db(db),
        m_opts(opts)
    {}

    void setDBpointer(const std::shared_ptr<ldmmap::LDMMap> &db)
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
    std::shared_ptr<ldmmap::LDMMap> m_db{nullptr};

    /*! @brief */
    options_t *m_opts{nullptr};

    /*! @brief */
    bool m_left_indicator_enabled{false};
};

#endif // TRIGGERMAN_H