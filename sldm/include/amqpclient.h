/**
 * @file amqpclient.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SLDM_AMQP_CLIENT_H
#define SLDM_AMQP_CLIENT_H

#include <atomic>
#include <memory>
#include <proton/connection_options.hpp>
#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/message_id.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/receiver_options.hpp>
#include <proton/source_options.hpp>
#include <proton/tracker.hpp>
#include <proton/types.hpp>

#include "areafilter.h"
#include "etsidecoderfrontend.h"
#include "ldmmap.h"
#include "triggermanager.h"
#include "utils.h"

/**
 * @brief 
 * 
 */
class AMQPClient : public proton::messaging_handler
{
public:
    /**
     * @brief Construct a new AMQPClient object
     * 
     * @param u 
     * @param a 
     * @param latmin 
     * @param latmax 
     * @param lonmin 
     * @param lonmax 
     * @param opts_ptr 
     * @param db_ptr 
     * @param logfile_name 
     */
    //AMQPClient(const std::string &u, const std::string &a, const double &latmin, const double &latmax, const double &lonmin, const double &lonmax, struct options *opts_ptr, const LDMMapPtr &db, std::string logfile_name=std::string()) : 
        // conn_url_(u),
        // addr_(a),
        // max_latitude(latmax),
        // max_longitude(lonmax),
        // min_latitude(latmin),
        // min_longitude(lonmin),
        // m_configuration(opts_ptr),
        // m_db(db)
    AMQPClient(const AMPQClientConfiguration &configuration, const LDMMapPtr &db)
    {
        m_areaFilter.setOptions(m_opts_ptr);
    }

    /**
     * @brief Set the Indicator Trigger Manager object
     * 
     * @param indicatorTrgMan_ptr 
     */
    void setIndicatorTriggerManager(const IndicatorTriggerManagerPtr &indicatorTrgMan)
    {
        m_indicatorTrgMan = indicatorTrgMan;
        m_indicatorTrgMan_enabled = true;
    }

    /**
     * @brief Set the Username object
     * 
     * @param username 
     */
    void setUsername(std::string username)
    {
        m_username = username;
    }

    /**
     * @brief Set the Password object
     * 
     * @param password 
     */
    void setPassword(std::string password)
    {
        m_password = password;
    }

    /**
     * @brief Set the Connection Options object
     * 
     * @param allow_sasl 
     * @param allow_insecure 
     * @param reconnect 
     */
    void setConnectionOptions(bool allow_sasl, bool allow_insecure, bool reconnect = false)
    {
        m_allow_sasl = allow_sasl;
        m_allow_insecure = allow_insecure;
        m_reconnect = reconnect;
    }

    /**
     * @brief Set the Credentials object
     * 
     * @param username 
     * @param password 
     * @param allow_sasl 
     * @param allow_insecure 
     * @param reconnect 
     */
    void setCredentials(std::string_view const& username, std::string_view const& password, bool allow_sasl, bool allow_insecure, bool reconnect = false)
    {
        m_username = username;
        m_password = password;
        m_allow_sasl = allow_sasl;
        m_allow_insecure = allow_insecure;
        m_reconnect = reconnect;
    }

    /**
     * @brief Set the Idle Timeout object
     * 
     * @param idle_timeout_ms 
     */
    void setIdleTimeout(long idle_timeout_ms)
    {
        m_idle_timeout_ms = idle_timeout_ms;
    }

    /**
     * @brief Set the Client I D object
     * 
     * @param id 
     */
    void setClientID(std::string const& id)
    {
        m_client_id = id;
    }

    /**
     * @brief Set the Filter object
     * 
     * @param filter 
     */
    void setFilter(std::string &filter)
    {
        m_quadKey_filter = filter;
    }

    /**
     * @brief 
     * 
     * @param decoded_cam 
     * @param stationID 
     * @return ldmmap::OptionalDataItem<uint8_t> 
     */
    inline ldmmap::OptionalDataItem<uint8_t> manage_LowfreqContainer(CAM_t *decoded_cam, uint32_t stationID);

    /**
     * @brief 
     * 
     * @param c 
     */
    void on_container_start(proton::container &c) override;

    /**
     * @brief 
     * 
     * @param conn 
     */
    void on_connection_open(proton::connection &conn) override;

    /**
     * @brief 
     * 
     * @param d 
     * @param msg 
     */
    void on_message(proton::delivery &d, proton::message &msg) override;

    /**
     * @brief 
     * 
     * @param c 
     */
    void on_container_stop(proton::container &c) override;

    /**
     * @brief 
     * 
     * @param conn 
     */
    void on_connection_close(proton::connection &conn) override;

    /**
     * @brief This function can be used to force the termination of the event loop of the current client from outside the AMQPClient object
     *        "m_cont" is an atomic pointer to the proton::container which is set as soon as on_container_start() is called
     * 
     */
    void force_container_stop()
    {
        if (m_cont != nullptr)
        {
            m_cont.load()->stop();
            m_cont = nullptr;
        }
    }

    /**
     * @brief Set the Print Msg object
     * 
     * @param printMsgEnable 
     */
    void setPrintMsg(bool printMsgEnable)
    {
        m_printMsg = printMsgEnable;
    }

private:
    std::string mLogTag{"[AMQPClient] "};

    double max_latitude;
    double max_longitude;
    double min_latitude;
    double min_longitude;

    /** @brief If 'true' each received message will be printed (default: 'false' - enable only for debugging purposes) */
    bool m_printMsg{false};

    etsiDecoder::decoderFrontend m_decodeFrontend;          //!< 
    AreaFilter m_areaFilter;                                //!< 
    LDMMapPtr m_db{nullptr};                                //!< 

    IndicatorTriggerManagerPtr m_indicatorTrgMan{nullptr};  //!< 
    bool m_indicatorTrgMan_enabled{false};                  //!< 

    std::string conn_url_;                                  //!< 
    std::string addr_;                                      //!< 
    std::string m_username;                                 //!< 
    std::string m_password;                                 //!< 
    bool m_reconnect{false};                                //!< 
    bool m_allow_sasl{false};                               //!< 
    bool m_allow_insecure{false};                           //!< 
    long m_idle_timeout_ms{-1};                             //!< 
    std::string m_quadKey_filter;                           //!< 
    std::atomic<proton::container *> m_cont{nullptr};       //!< 
    std::string m_client_id{"unset"};                       //!< 
};

#endif // SLDM_AMQP_CLIENT_H
