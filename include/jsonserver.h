/**
 * @file jsonserver.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef JSONSERVER_H
#define JSONSERVER_H

#include <atomic>
#include <nlohmann/json.hpp>
#include "ldmmap.h"

/**
 * @brief 
 * 
 */
struct VehicleData
{
public:
    /** @brief */
    uint64_t stationID;

    /** @brief */
    double lat;

    /** @brief */
    double lon;

    /** @brief */
    std::string/ turnindicator;

    /** @brief */
    uint16_t CAM_tstamp;

    /** @brief */
    uint64_t GN_tstamp;

    /** @brief */
    ldmmap::OptionalDataItem<long> car_length_mm;

    /** @brief */
    ldmmap::OptionalDataItem<long> car_width_mm;

    /** @brief */
    double speed_ms;

    /** @brief */
    std::shared_ptr<ldmmap::PHpoints> path_history;

    /** @brief */
    double relative_dist_m;

    /** @brief */
    ldmmap::e_StationTypeLDM stationType;

    /** @brief */
    uint64_t diff_ref_tstamp;

    /** @brief */
    double heading;
}

/**
 * @brief 
 * 
 */
class JSONserver
{
public:
    /**
     * @brief Construct a new JSONserver object
     * 
     * @param db_ptr 
     */
    JSONserver(const std::shared_ptr<ldmmap::LDMMap> &db) :
        m_db(db)
    {};

    /**
     * @brief Construct a new JSONserver object
     * 
     * @param db_ptr 
     * @param port 
     */
    JSONserver(const std::shared_ptr<ldmmap::LDMMap> &db_ptr, long port) :
        m_db(db),
        m_port(port)
    {};

    /**
     * @brief Destroy the JSONserver object
     * 
     */
    ~JSONserver()
    {
        stopServer();
    }

    /**
     * @brief Set the Server Port object
     * 
     * @param port 
     */
    void setServerPort(long port)
    {
        m_port = port;
    }

    /**
     * @brief These methods can be used to set and retrieve the default range radius in case it is not specified in the JSON-over-TCP requests
     * 
     * @param range_m 
     */
    void changeDefaultContextRange(double range_m)
    {
        m_range_m = range_m;
    }

    /**
     * @brief Get the Default Context Range object
     * 
     * @return double 
     */
    double getDefaultContextRange(void)
    {
        return m_range_m;
    }

    /**
     * @brief 
     * 
     * @param lat 
     * @param lon 
     * @param range 
     * @return nlohmann::json 
     */
    nlohmann::json make_SLDM_json(double lat, double lon, double range);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool startServer(void);

    /**
     * @brief 'true' is returned if the server was successfully stopped, 'false' is returned if the server termination attempt failed
     * 
     * @return true 
     * @return false 
     */
    bool stopServer();

    /**
     * @brief Advanced option: change the backlog queue size for the TCP socket
     *        Specifying an invalid size (<= 0) will have no effect
     * 
     * @param size 
     */
    void changeBacklogQueueSize(int size)
    {
        if (size > 0)
        {
            m_backlog_size = size;
        }
    };

    /**
     * @brief Get the Sock object
     *        The user is not expected to call these functions, which are used internally
     *        (they must be public due to the usage of pthread; future work will switch to C++ threads)
     * 
     * @return int 
     */
    int getSock(void)
    {
        return m_sockd;
    }

    /**
     * @brief 
     * 
     * @param status 
     */
    void setJSONThreatRunningStatus(bool status)
    {
        m_thread_running = status;
    }

private:
    /** @brief */
    nlohmann::json make_vehicle(const VehicleData &VehicleData);

private:
    /**
     * @brief 
     * 
     */
    class DefaultValues
    {
    public:
        /** @brief */
        static constexpr long PORT{49000};

        /** @brief */
        static constexpr double RANGE_M{300.0};

        /** @brief */
        static constexpr bool THREAD_RUNNING{false};

        /** @brief */
        static constexpr int BACKLOG_SIZE{5};

        /** @brief */
        static constexpr int SOCKD{1};
    }

    /** @brief */
    double m_range_m{DefaultValues::RANGE_M};

    /** @brief */
    std::shared_ptr<ldmmap::LDMMap> m_db{nullptr};

    /** @brief */
    long m_port{DefaultValues::PORT};

    /** @brief */
    std::atomic<bool> m_thread_running{DefaultValues::THREAD_RUNNING};

    /** @brief */
    int m_backlog_size{DefaultValues::BACKLOG_SIZE};

    /** @brief */
    int m_sockd{DefaultValues::SOCKD};

    /** @brief */
    std::thread m_thread;
};

#endif // JSONSERVER_H