#include <cstring>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <pthread.h>
#include <set>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "jsonserver.h"
#include "utils.h"

static inline double haversineDist(double lat_a, double lon_a, double lat_b, double lon_b)
{
    // 12742000 is the mean Earth radius (6371 km) * 2 * 1000 (to convert from km to m)
    return 12742000.0 * asin(sqrt(sin(DEG_2_RAD(lat_b - lat_a) / 2) * sin(DEG_2_RAD(lat_b - lat_a) / 2) + cos(DEG_2_RAD(lat_a)) * cos(DEG_2_RAD(lat_b)) * sin(DEG_2_RAD(lon_b - lon_a) / 2) * sin(DEG_2_RAD(lon_b - lon_a) / 2)));
}

void *JSONthread_callback(void *arg)
{
    fd_set sockdescrs, rd_sockdescrs;
    std::set<int> sockdescrs_set = {};
    char rx_buff[1024];
    std::string rx_str;

    JSONserver *srvObj = static_cast<JSONserver *>(arg);

    int sockd = srvObj->getSock();

    // Zero out the set of descriptors to be monitored with select()
    FD_ZERO(&sockdescrs);
    // Add the main TCP socket to the FD set
    FD_SET(sockd, &sockdescrs);

    fprintf(stdout, "[INFO] The JSON server is ready to provide data to the clients. Main TCP socket descriptor: %d\n", sockd);

    while (true)
    {
        int connectiond;

        rd_sockdescrs = sockdescrs;

        // "+1" to consider the main TCP socket
        if (select(FD_SETSIZE, &rd_sockdescrs, nullptr, nullptr, nullptr) < 0 && errno != EINTR)
        {
            perror("[ERROR] JSON server died on select(). Reason");
            srvObj->setJSONThreatRunningStatus(false);
            pthread_exit(0);
        }

        // Main socket generated an event
        if (FD_ISSET(sockd, &rd_sockdescrs))
        {
            fprintf(stdout, "[INFO] Received connection request.\n");

            // Accept the new connection
            if ((connectiond = accept(sockd, nullptr, nullptr)) < 0)
            {
                perror("[ERROR] JSON server cannot accept a client connection. Reason");
            }
            else
            {
                FD_SET(connectiond, &sockdescrs);
                sockdescrs_set.insert(connectiond);

                // Send a confirmation to the client
                char confirm_msg[] = "Connection: confirmed";
                if (send(connectiond, confirm_msg, strlen(confirm_msg), 0) != static_cast<ssize_t>(strlen(confirm_msg)))
                {
                    perror("[ERROR] Could not send confirmation to connected client. The connection will still remain active. Reason");
                }

                fprintf(stdout, "[INFO] Connection from client accepted. Connected socket descriptor: %d\n", connectiond);
            }
            // Other socket generated an event
        }
        else
        {
            fprintf(stdout, "[INFO] Received message from client.\n");
            ssize_t readbytes = -1;

            for (auto currd_it = sockdescrs_set.begin(); currd_it != sockdescrs_set.end();)
            {
                int currd = *currd_it;

                if (FD_ISSET(currd, &rd_sockdescrs))
                {
                    // Read from the connection
                    readbytes = recv(currd, rx_buff, sizeof(rx_buff), 0);

                    // Connection is closing
                    if (readbytes == 0)
                    {
                        fprintf(stdout, "[INFO] Client with descriptor %d disconnected.\n", currd);
                        close(currd);
                        sockdescrs_set.erase(currd_it++);
                        FD_CLR(currd, &sockdescrs);
                    }
                    else if (readbytes > 0)
                    {
                        // Send the information the client requested
                        nlohmann::json db_data;
                        std::string err = "";

                        rx_str = std::string(rx_buff);
                        nlohmann::json request = nlohmann::json::parse(rx_str);

                        if (err == "")
                        {
                            double lat = request.at("lat");
                            double lon = request.at("lon");
                            double range = request.at("range");

                            db_data = srvObj->make_SLDM_json(lat, lon, range);

                            std::string strjson = db_data.dump();

                            if (send(currd, strjson.c_str(), strjson.length(), 0) != static_cast<ssize_t>(strjson.length()))
                            {
                                perror("[ERROR] Could not send data to connected client. The connection will still remain active. Reason");
                            }
                        }
                        else
                        {
                            fprintf(stdout, "[ERROR] Cannot parse a client request. Descriptor: %d. Error details: %s\n", currd, err.c_str());
                        }

                        ++currd_it;
                    }
                    else
                    {
                        perror("[ERROR] Cannot read a data request from a client. Reason");

                        ++currd_it;
                    }
                }
            }
        }
    }

    perror("[INFO] JSON server terminated. Last errno value");
    srvObj->setJSONThreatRunningStatus(false);
    pthread_exit(NULL);
}

nlohmann::json JSONserver::make_SLDM_json(double lat, double lon, double range)
{
    // Variable to store the relative distance of vehicles w.r.t. the reference vehicle
    double refRelDist = -1.0;

    // Create a new JSON structure
    nlohmann::json AIM_json;

    // Returned vehicle data vector
    std::vector<ldmmap::LDMMap::returnedVehicleData_t> returnedvehs;

    // "now" timestamp (i.e., the timestamp at which this JSON request is being generated: "generation_tstamp")
    uint64_t now_us = get_timestamp_us();

    AIM_json["generation_tstamp"] = now_us;
    AIM_json["reference_lat"] = lat;
    AIM_json["reference_lon"] = lon;

    // If the range was not specified or an invalid value was requested, use the default value
    if (range <= 0)
    {
        range = m_range_m;
        fprintf(stdout, "[INFO] No range specified in JSON-over-TCP request. Using default value of %.2lf m.\n", range);
    }

    if (m_db->rangeSelect(range, lat, lon, returnedvehs) != ldmmap::LDMMap::LDMMAP_OK)
    {
        AIM_json["return_code"] = "error";
        return AIM_json;
    }
    else
    {
        AIM_json["error"] = "ok";
    }

    nlohmann::json vehicles;

    for (ldmmap::LDMMap::returnedVehicleData_t vehdata : returnedvehs)
    {
        // Compute the relative distance w.r.t. the reference lat and lon
        refRelDist = haversineDist(lat, lon, vehdata.vehData.lat, vehdata.vehData.lon);

        VehicleData vehicleData;
        vehicleData.stationID = vehdata.vehData.stationID;
        vehicleData.lat = vehdata.vehData.lat;
        vehicleData.lon = vehdata.vehData.lon;
        vehicleData.turnindicator = vehdata.vehData.exteriorLights.isAvailable() ? exteriorLights_bit_to_string(vehdata.vehData.exteriorLights.getData()) : "unavailable";
        vehicleData.CAM_tstamp = vehdata.vehData.camTimestamp;
        vehicleData.GN_tstamp = vehdata.vehData.gnTimestamp;
        vehicleData.car_length_mm = vehdata.vehData.vehicleLength;
        vehicleData.car_width_mm = vehdata.vehData.vehicleWidth;
        vehicleData.speed_ms = vehdata.vehData.speed_ms;
        vehicleData.path_history = vehdata.phData;
        vehicleData.relative_dist_m = refRelDist;
        vehicleData.stationType = vehdata.vehData.stationType;
        vehicleData.diff_ref_tstamp = now_us - vehdata.vehData.timestamp_us;
        vehicleData.heading = vehdata.vehData.heading;
        vehicles.push_back(make_vehicle(vehicleData));
    }

    AIM_json["vehicles"] = vehicles;

    return AIM_json;
}

nlohmann::json JSONserver::make_vehicle(VehicleData &vehicleData)
{
    nlohmann::json vehicle;

    vehicle["stationID"] = vehicleData.stationID;
    vehicle["lat"] = vehicleData.lat;
    vehicle["lon"] = vehicleData.lon;
    vehicle["speed_ms"] = vehicleData.speed_ms;
    vehicle["turnindicator"] = vehicleData.turnindicator;
    vehicle["CAM_tstamp"] = vehicleData.CAM_tstamp;
    vehicle["GN_tstamp"] = vehicleData.GN_tstamp;
    vehicle["relative_dist_to_reference_m"] = vehicleData.relative_dist_m;
    vehicle["stationType"] = vehicleData.stationType;
    vehicle["heading"] = vehicleData.heading;

    // This value represents the difference between when the database is being read for this vehicle (i.e., now) and when the data for that vehicle was last stored
    vehicle["time_since_generation_tstamp"] = vehicleData.diff_ref_tstamp;

    if (vehicleData.car_length_mm.isAvailable())
    {
        vehicle["car_len_mm"] = vehicleData.car_length_mm.getData();
    }
    else
    {
        vehicle["car_len_mm"] = "unavailable";
    }

    if (vehicleData.car_width_mm.isAvailable())
    {
        vehicle["car_wid_mm"] = vehicleData.car_width_mm.getData();
    }
    else
    {
        vehicle["car_wid_mm"] = "unavailable";
    }

    nlohmann::json PH_points_lat_json;
    nlohmann::json PH_points_lon_json;

    if (vehicleData.path_history != nullptr)
    {
        PHDATAITER_INITIALIZER(phdataiter);

        // Iterate over all the path history points, using the "iterate" method of the PHPoints object,
        // storing the Path History points for each vehicle in the LDMMap database
        while (vehicleData.path_history->iterate(phdataiter, nullptr) != ldmmap::PHpoints::PHP_TERMINATE_ITERATION)
        {
            PH_points_lat_json.push_back(std::to_string(phdataiter.data.lat));
            PH_points_lon_json.push_back(std::to_string(phdataiter.data.lon));
        }

        vehicle["PH_points_lat"] = PH_points_lat_json;
        vehicle["PH_points_lon"] = PH_points_lon_json;
    }

    return vehicle;
}

bool JSONserver::startServer(void)
{
    // Return immediately if the JSON server is already running
    if (m_thread_running == true)
    {
        fprintf(stderr, "[ERROR] The program has attempted to start an already running JSON server. Please report this bug to the developers.\n");
        return false;
    }

    // Create the TCP socket for data exchange
    m_sockd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_sockd < 0)
    {
        perror("[ERROR] JSONserver: cannot create TCP socket. Details");
        return false;
    }

    struct sockaddr_in bindaddr;
    memset(&bindaddr, 0, sizeof(bindaddr));

    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(m_port);
    bindaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port stored inside m_port
    if (bind(m_sockd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) < 0)
    {
        perror("[ERROR] JSONserver: cannot bind TCP socket. Details");
        close(m_sockd);
        m_sockd = -1;
        return false;
    }

    // Listed for incoming connections
    if (listen(m_sockd, m_backlog_size) < 0)
    {
        perror("[ERROR] JSONserver: cannot listen on TCP socket. Details");
        close(m_sockd);
        m_sockd = -1;
        return false;
    }

    // The "accept" part is then performed in the separate thread
    if (pthread_create(&m_tid, NULL, JSONthread_callback, (void *)this) == 0)
    {
        m_thread_running = true;
    }
    else
    {
        perror("[ERROR] JSONserver: cannot create receiving thread. Details");
        close(m_sockd);
        m_sockd = -1;
        m_tid = -1;
        return false;
    }

    return true;
}

bool JSONserver::stopServer(void)
{
    // Return immediately if the server is not running, or if m_tid is not valid
    if (m_thread_running == false || m_tid < 0)
    {
        return false;
    }

    if (pthread_cancel(m_tid) != 0)
    {
        return false;
    }

    if (pthread_join(m_tid, NULL) != 0)
    {
        return false;
    }

    m_tid = -1;

    return true;
}