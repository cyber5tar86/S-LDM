/**
 * @file vehicle-visualizer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef VEHICLE_VISUALIZER_H
#define VEHICLE_VISUALIZER_H

#include <string>

/**
 * @brief 
 * 
 */
#define VIS_HEADING_INVALID 361

class VehicleVisualizerDefault
{
public:
    static constexpr char ADDRESS[] = "127.0.0.1";
    static constexpr int PORT{48110};
    static constexpr int HTTP_PORT{8080};
    static constexpr char DEFAULT_NODEJS_SERVER_PATH[] = "./js/server.js";
};

/**
 * @brief 
 * 
 */
class VehicleVisualizer
{
public:
    /**
     * @brief Create a new vehicleVisualizer object with the default IP and port for the connection to the Node.js server
     * 
     */
    VehicleVisualizer();

    /**
     * @brief Create a new vehicleVisualizer object with the default IP for the connection to the Node.js server
     *
     */
    explicit VehicleVisualizer(int port);

    /**
     * @brief Create a new vehicleVisualizer object for the connection to the Node.js server
     * 
     * @param port 
     * @param ipv4 
     */
    VehicleVisualizer(int port, std::string ipv4);

    /**
     * @brief Object destructor, taking also care of sending the "terminate" message to the Node.js server to gracefully terminate it
     * 
     */
    virtual ~VehicleVisualizer();

    // 
    /**
     * @brief Setters to set an IPv4 different than the default ones, for the UDP connection to the Node.js server
     * 
     * @param ipv4 
     */
    void
    setAddress(std::string address);

    /**
     * @brief Setters to set a port, different than the default ones, for the UDP connection to the Node.js server
     * 
     * @param port 
     */
    void
    setPort(int port);

    /**
     * @brief Setter to set a different HTTP port at which the web visualizer will be available
     * 
     * @param port 
     */
    void
    setHTTPPort(int port);

    /**
     * @brief This function must be called just after creating a new vehicleVisualizer object
     *        It will start a Node.js server receiving data from ms-van3t via UDP and making a web GUI available 
     *        to the user on loopback, port "m_httpport"
     *        This function will return only when the Node.js server is ready
     * 
     * @return int 
     */
    int
    startServer();

    /**
     * @brief This function must be called after calling startServer()
     *        It will set up the UDP socket for the connection between ms-van3t and the server
     * 
     * @return int 
     */
    int
    connectToServer();

    // These functions should be automatically called by the TraCI client o by the GPS-tc client
    // The user, however, can also call sendObjectUpdate() to add and move an external object on the map, not managed by SUMO or GPS-tc

    // Send a "map draw" message to tell the server where the client (i.e. the web browser) should center the map at
    // This function should always be called once before calling sendObjectUpdate()
    int
    sendMapDraw(double lat, double lon);

    int
    sendMapDraw(double lat, double lon, double minlat, double minlon, double maxlat, double maxlon, double lat_ext_factor, double lon_ext_factor);

    // These functions will update the position on the map of the object with unique id "objID" (the vehicle ID can be used here, for instance)
    // If the object does not exist yet on the map, it will be first added
    // If no "heading" is specified, VIS_HEADING_INVALID (i.e. no heading available) will be sent to the server
    int
    sendObjectUpdate(std::string objID, double lat, double lon, int stationType);
    int
    sendObjectUpdate(std::string objID, double lat, double lon, int stationType, double heading);

    // This function will remove an object from the map, given its unique "objID"
    int
    sendObjectClean(std::string objID);

    // This function should be called to terminate the execution of the Node.js server
    // Normally, the user should not call it, as it is automatically called by the destructor of the vehicleVisualizer object
    int
    terminateServer();

    // This setter can be used to set a different path for the Node.js server.js file location
    void
    setServerPath(std::string srvpath) { m_serverpath = srvpath; }

    // These methods allows the user to check whether the UDP socket creation was successfully executed or whether the "map draw" message was successfully sent via UDP
    bool isConnected() { return m_is_connected; }
    bool mapDrawSent() { return m_is_map_sent; }

private:
    int m_port{VehicleVisualizerDefault::PORT};
    int m_httpPort{VehicleVisualizerDefault::HTTP_PORT};                            //!< HTTP Node.js server port 
    std::string m_address{VehicleVisualizerDefault::ADDRESS};
    std::string m_serverpath{VehicleVisualizerDefault::DEFAULT_NODEJS_SERVER_PATH};
    int m_sockfd{-1};
    bool m_is_connected{false};
    bool m_is_map_sent{false};
    bool m_is_server_active{false};

    // Internal (private) function to open the UDP socket for the communication with the Node.js server
    int socketOpen(void);
};

#endif /* VEHICLE_VISUALIZER_H */