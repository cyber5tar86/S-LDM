#ifndef MANEUVERINGSERVICERESTCLIENT_H
#define MANEUVERINGSERVICERESTCLIENT_H

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <memory>
#include <nlohmann/json.hpp>
#include "ldmmap.h"

/**
 * @brief IMPORTANT_NOTE: the ManeuveringServiceRestClient objects should always be created with "new" as they include a "self-destroy"
 *        mechanism which relies on "delete"
 *        Creating a ManeuveringServiceRestClient object on the stack, without new, may result in trying to destroy an already destroyed
 *        object when the object goes out of scope
 * 
 */
class ManeuveringServiceRestClient
{
public:
    // ManeuveringServiceRestClient(double lat, double lon, uint64_t refVehStationID, const std::shared_ptr<ldmmap::LDMMap> &db) :
    //     m_range_m(m_range_m_default),
    //     m_db_ptr(db_ptr),
    //     m_lat(lat),
    //     m_lon(lon),
    //     m_refVehStationID(refVehStationID),
    //     m_notify_fcn(nullptr)
    // {
    //     m_thread_running=false;
    //     m_srv_addr="http://localhost";
    //     m_port=8000;
    // };

    /**
     * @brief Construct a new Maneuvering Service Rest Client object
     * 
     * @param refVehStationID 
     * @param db_ptr 
     */
    ManeuveringServiceRestClient(uint64_t refVehStationID, const std::shared_ptr<ldmmap::LDMMap> &db) :
        m_refVehStationID(refVehStationID),
        m_db(db)
    {};

    /**
     * @brief Construct a new Maneuvering Service Rest Client object
     * 
     * @param refVehStationID 
     * @param db_ptr 
     * @param address 
     * @param port 
     */
    ManeuveringServiceRestClient(uint64_t refVehStationID, const std::shared_ptr<ldmmap::LDMMap> &db, std::string address, long port) :
        m_refVehStationID(refVehStationID),
        m_db(db),
        m_srv_addr{address},
        m_port{port}
    {};

    /**
     * @brief Set the Notify Function object
     * 
     * @param notify_fcn 
     */
    void setNotifyFunction(const std::function<void(uint64_t)> &notify_fcn)
    {
        m_notify_fcn = notify_fcn;
    }

    /**
     * @brief 
     * 
     */
    void inline callNotifyFunction(void);

    /**
     * @brief 
     * 
     * @return int 
     */
    int startRESTthread(void);

    /**
     * @brief Set the Periodic Interval object
     * 
     * @param interval_sec 
     */
    void setPeriodicInterval(double interval_sec)
    {
        m_interval_sec = interval_sec;
    }

    /**
     * @brief Get the Periodic Interval object
     * 
     * @return double 
     */
    double getPeriodicInterval(void)
    {
        return m_interval_sec;
    }

    /**
     * @brief Set the Thread Running Status object
     *        This method is used internally by the thread callback function to signal the REST thread termination
     * 
     * @param status 
     */
    void setThreadRunningStatus(bool status)
    {
        m_thread_running = status;
    }

    /**
     * @brief Get the Running Status object
     * 
     * @return true if the REST thread is currently running and transmitting data to the Maneuvering Service
     * @return false otherwise
     */
    bool getRunningStatus(void)
    {
        return m_thread_running;
    };

    /**
     * @brief Set the Server Address object
     * 
     * @param address 
     */
    void setServerAddress(std::string address)
    {
        m_srv_addr = address;
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
     * @brief Set the Server Address Port object
     * 
     * @param address 
     * @param port 
     */
    void setServerAddressPort(std::string address, long port)
    {
        m_srv_addr = address;
        m_port = port;
    }

    /**
     * @brief 
     * 
     * @param range_m 
     */
    void changeContextRange(double range_m)
    {
        m_range_m = range_m;
    }

    /**
     * @brief Get the Context Range object
     * 
     * @return double 
     */
    double getContextRange(void)
    {
        return m_range_m;
    }

    /**
     * @brief Get the Server Full Address object
     * 
     * @return std::string 
     */
    std::string inline getServerFullAddress(void);

    /**
     * @brief 
     * 
     * @param eventID 
     * @return nlohmann::json 
     */
    nlohmann::json make_SLDM_json(int eventID);

private:
    /**
     * @brief 
     * 
     * @param stationID 
     * @param lat 
     * @param lon 
     * @param turnindicator 
     * @param CAM_tstamp 
     * @param GN_tstamp 
     * @param car_length_mm 
     * @param car_width_mm 
     * @param speed_ms 
     * @param path_history 
     * @param src_quadk 
     * @param relative_dist_m 
     * @param stationType 
     * @param diff_ref_tstamp 
     * @param diff_rec_tstamp 
     * @param cam_rec_tstamp 
     * @param db_up_tstamp 
     * @param heading 
     * @return nlohmann::json 
     */
    nlohmann::json make_vehicle(uint64_t stationID,
                                  double lat,
                                  double lon,
                                  std::string turnindicator,
                                  uint16_t CAM_tstamp,
                                  uint64_t GN_tstamp,
                                  ldmmap::OptionalDataItem<long> car_length_mm,
                                  ldmmap::OptionalDataItem<long> car_width_mm,
                                  double speed_ms,
                                  std::shared_ptr<ldmmap::PHpoints> &path_history,
                                  std::string &src_quadk,
                                  double relative_dist_m,
                                  ldmmap::e_StationTypeLDM stationType,
                                  uint64_t diff_ref_tstamp,
                                  uint64_t diff_rec_tstamp,
                                  uint64_t cam_rec_tstamp,
                                  uint64_t db_up_tstamp,
                                  double heading);

private:
    /**
     * @brief 
     * 
     */
    class DefaultValues
    {
    public:
        /** @brief */
        static constexpr char SRV_ADDR[] = "http://localhost\0";

        /** @brief */
        static constexpr long PORT{8000};

        /** @brief */
        static constexpr double RANGE_M{100.0};

        /** @brief */
        static constexpr bool THREAD_RUNNING{false};

        /** @brief */
        static constexpr double INTERVAL_SEC{1.0};
    };

    /** @brief */
    double m_range_m;

    /** @brief */
    std::shared_ptr<ldmmap::LDMMap> m_db{nullptr};

    /** @brief */
    std::string m_srv_addr{DefaultValues::SRV_ADDR};

    /** @brief */
    long m_port{DefaultValues::PORT};

    /** @brief */
    double m_interval_sec{DefaultValues::INTERVAL_SEC};

    // Not needed, as the rangeSelect() on the database is performed based on the stationID and not on a fixed point
    // double m_lat = 0.0;
    // double m_lon = 0.0;

    /** @brief */
    uint64_t m_refVehStationID;

    /** @brief */
    std::atomic<bool> m_thread_running{DefaultValues::THREAD_RUNNING};

    /** @brief */
    std::function<void(uint64_t)> m_notify_fcn{nullptr};
};

#endif // MANEUVERINGSERVICERESTCLIENT_H
