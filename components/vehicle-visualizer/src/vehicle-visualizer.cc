/**
 * @file vehicle-visualizer.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <arpa/inet.h>
#include <common/log.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "vehicle-visualizer.h"

VehicleVisualizer::VehicleVisualizer()
{
    log4cplus::NDCContextCreator context(LOG4CPLUS_TEXT("VehicleVisualizer"));
}

VehicleVisualizer::VehicleVisualizer(int port) : VehicleVisualizer()
{
    m_port = port;
}

VehicleVisualizer::VehicleVisualizer(int port, std::string address) : VehicleVisualizer()
{
    m_port = port;
    m_address = std::move(address);
}

VehicleVisualizer::~VehicleVisualizer()
{
    // The destructor will attempt to send a termination message to the Node.js server only if a server
    // was successfully started with startServer()
    if (m_is_server_active)
    {
        terminateServer();

        // Close the UDP socket, if it was opened before
        if (m_sockfd != -1)
        {
            close(m_sockfd);
            m_sockfd = -1;
        }
    }
}

int
VehicleVisualizer::connectToServer()
{
    m_sockfd = socketOpen();

    if (m_sockfd < 0)
    {
        return -1;
    }

    m_is_connected = true;

    return 0;
}

int
VehicleVisualizer::sendMapDraw(double lat, double lon)
{
    if (m_is_connected == false)
    {
        LogError("Error: attempted to use a non-connected vehicle visualizer client.")
        exit(EXIT_FAILURE);
    }

    if (m_is_map_sent == true)
    {
        LogError("Error in vehicle visualizer client: attempted to send twice a map draw message. This is not allowed.")
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    int send_rval = -1;

    oss.precision(7);
    oss << "map," << lat << "," << lon;

    std::string msg_string = oss.str();
    char *msg_buffer = new char[msg_string.length() + 1];
    std::copy(msg_string.c_str(), msg_string.c_str() + msg_string.length() + 1, msg_buffer);

    send_rval = send(m_sockfd, msg_buffer, msg_string.length() + 1, 0);

    delete[] msg_buffer;

    m_is_map_sent = true;

    return send_rval;
}

int
VehicleVisualizer::sendMapDraw(double lat, double lon, double minlat, double minlon, double maxlat, double maxlon, double lat_ext_factor, double lon_ext_factor)
{
    if (m_is_connected == false)
    {
        LogError("Error: attempted to use a non-connected vehicle visualizer client.")
        exit(EXIT_FAILURE);
    }

    if (m_is_map_sent == true)
    {
        LogError("Error in vehicle visualizer client: attempted to send twice a map draw message. This is not allowed.")
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    int send_rval = -1;

    oss.precision(7);
    oss << "map_areas," << lat << "," << lon << "," << minlat << "," << minlon << "," << maxlat << "," << maxlon << "," << lat_ext_factor << "," << lon_ext_factor;

    std::string msg_string = oss.str();
    char *msg_buffer = new char[msg_string.length() + 1];
    std::copy(msg_string.c_str(), msg_string.c_str() + msg_string.length() + 1, msg_buffer);

    send_rval = send(m_sockfd, msg_buffer, msg_string.length() + 1, 0);

    delete[] msg_buffer;

    m_is_map_sent = true;

    return send_rval;
}

int
VehicleVisualizer::sendObjectUpdate(std::string objID, double lat, double lon, int stationType, double heading)
{
    if (m_is_connected == false)
    {
        LogError("Error: attempted to use a non-connected vehicle visualizer client.")
        exit(EXIT_FAILURE);
    }

    if (m_is_map_sent == false)
    {
        LogError("Error in vehicle visualizer client: attempted to send an object update before sending the map draw message.")
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    int send_rval = -1;

    oss.precision(7);
    oss << "object," << objID << "," << lat << "," << lon << "," << stationType << ",";
    oss.precision(3);
    oss << heading;

    std::string msg_string = oss.str();
    char *msg_buffer = new char[msg_string.length() + 1];
    std::copy(msg_string.c_str(), msg_string.c_str() + msg_string.length() + 1, msg_buffer);

    send_rval = send(m_sockfd, msg_buffer, msg_string.length() + 1, 0);

    delete[] msg_buffer;

    return send_rval;
}

int
VehicleVisualizer::sendObjectUpdate(std::string objID, double lat, double lon, int stationType)
{
    return sendObjectUpdate(objID, lat, lon, stationType, VIS_HEADING_INVALID);
}

int
VehicleVisualizer::sendObjectClean(std::string objID)
{
    if (m_is_connected == false)
    {
        LogError("Error: attempted to use a non-connected vehicle visualizer client.")
        exit(EXIT_FAILURE);
    }

    if (m_is_map_sent == false)
    {
        LogError("Error in vehicle visualizer client: attempted to send an object clean message before sending the map draw message.")
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    int send_rval = -1;

    oss << "objclean," << objID;

    std::string msg_string = oss.str();
    char *msg_buffer = new char[msg_string.length() + 1];
    std::copy(msg_string.c_str(), msg_string.c_str() + msg_string.length() + 1, msg_buffer);

    send_rval = send(m_sockfd, msg_buffer, msg_string.length() + 1, 0);

    delete[] msg_buffer;

    return send_rval;
}

int
VehicleVisualizer::startServer()
{
    std::string servercmd;

    // Create a FIFO special file (see https://linux.die.net/man/3/mkfifo) for the "startup communication"
    // between the S-LDM and the node.js server we are going to start
    // Basically this file will be used as a pipe to tell the S-LDM when the node.js server UDP socket
    // creation is complete, so that the S-LDM can start sending information to the server
    // The special FIFO file will be named "vehvizfifo<PID of this S-LDM instance>"
    std::string fifofile = "/tmp/vehvizfifo" + std::to_string(getpid());
    int fifofd = -1;

    // Print an error and exit if mkfifo() failed due to any reason different than an already existing file (errno==EEXIST)
    // If the FIFO special file already exists, for any reason, we should be able to simply re-use it
    if (mkfifo(fifofile.c_str(), 0660) < 0)
    {
        if (errno != EEXIST)
        {
            LogError("Cannot start the Node.js server. mkfifo() communication pipe creation error.")
            perror("Error details:");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        LogInfo("VehicleVisualizer startup: using temporary FIFO special file: " << fifofile)
    }

    // Open the FIFO file (O_RDWR is needed, instead of O_RDONLY, to avoid blocking on this open() due to the absence of a writer.
    // Indeed, the writer will be the node.js server, which hasn't started yet)
    fifofd = open(fifofile.c_str(), O_RDWR);

    if (fifofd < 0)
    {
        LogError("Cannot start the Node.js server. mkfifo() communication pipe error.")
        exit(EXIT_FAILURE);
    }

    // Start the node.js server managing the web-based vehicle visualizer S-LDM GUI
    int nodeCheckRval = std::system("command -v node > /dev/null");

    if (nodeCheckRval != 0)
    {
        LogError("Error. Node.js does not seem to be installed. Please install it before using the S-LDM vehicle visualizer.")
        exit(EXIT_FAILURE);
    }

    // servercmd = "node " + m_serverpath + " " + std::to_string(m_httpPort) + " &";

    // node.js server (server.js) command line parameters: HTTP web interface port, UDP socket bind address, UDP socket port, PID of this S-LDM instance
    servercmd = "node " + m_serverpath + " " + std::to_string(m_httpPort) + " " + m_address + " " + std::to_string(m_port) + " " + std::to_string(getpid()) + " &";

    int startCmdRval = std::system(servercmd.c_str());

    // If the result is 0, system() was able to successfully launch the command (which may fail afterwards, though)
    if (startCmdRval == 0)
    {
        LogInfo("Used the following command to start up the vehicle visualizer Node.js server: " << servercmd)
        m_is_server_active = true;
    }
    else
    {
        LogError("Cannot send the command for starting the Node.js server.")
        exit(EXIT_FAILURE);
    }

    LogInfo("VehicleVisualizer startup: Waiting for the VehicleVisualizer node.js server to come up...")

    // Wait 1 second for the server to come up (no more needed since the FIFO special file mechanism is in place)
    // sleep(1);

    // Blocking read on the FIFO special file
    // This read() will block until the node.js server writes a string on the same file, to signal that its internal UDP socket is ready to receive
    // data from the S-LDM
    // The expected string from the node.js server, which will be stored inside "buf", is "STARTED" (so, we need 7 characters + '\0')
    char buf[8] = {0};
    if (read(fifofd, buf, 8) <= 0)
    {
        LogInfo("Error. It seems that the node.js server could not come up.")
    }
    else
    {
        LogInfo("VehicleVisualizer node.js server succesfully started. Confirmation message content: " << std::string(buf))
    }

    // Close and delete (with unlink(), as there should be no other process referring to the same file) the FIFO special file which was used as
    // a "synchronization mean" between the S-LDM and the node.js server
    close(fifofd);
    unlink(fifofile.c_str());

    return startCmdRval;
}

int
VehicleVisualizer::terminateServer()
{
    if (m_is_connected == false)
    {
        LogError("Error: attempted to use a non-connected vehicle visualizer client.")
        exit(EXIT_FAILURE);
    }

    // Just send a message with the "terminate" string
    char terminatebuf[10] = "terminate";

    int retval = send(m_sockfd, terminatebuf, 9, 0);

    if (retval == 9)
    {
        m_is_connected = false;
        m_is_server_active = false;
    }

    return retval;
}

int
VehicleVisualizer::socketOpen(void)
{
    struct sockaddr_in saddr = {};
    struct in_addr destIPaddr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockfd < 0)
    {
        LogError("Error! Cannot open the UDP socket for the vehicle visualizer.")
        exit(EXIT_FAILURE);
    }

    // Bind structure
    saddr.sin_family = AF_INET;
    saddr.sin_port = 0;
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&(saddr), sizeof(saddr)) < 0)
    {
        LogError("Error! Cannot bind the UDP socket for the vehicle visualizer.")
        exit(EXIT_FAILURE);
    }

    // Get struct in_addr corresponding to the destination IP address
    if (inet_pton(AF_INET, m_address.c_str(), &(destIPaddr)) != 1)
    {
        LogError("Error! Cannot parse the destination IP for the UDP socket for the vehicle visualizer.")
        exit(EXIT_FAILURE);
    }

    // Connect structure (re-using the same structure - sin_family should remain the same)
    saddr.sin_port = htons(m_port);
    saddr.sin_addr = destIPaddr;

    if (connect(sockfd, (struct sockaddr *)&(saddr), sizeof(saddr)) < 0)
    {
        LogError("Error! Cannot connect the UDP socket for the vehicle visualizer.")
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void
VehicleVisualizer::setAddress(std::string address)
{
    m_address = std::move(address);
}

void
VehicleVisualizer::setPort(int port)
{
    // Set the default port when an invalid port is specified
    if (port >= 1 && port <= 65535)
    {
        m_port = port;
    }
    else
    {
        LogError("Error: called setPort for vehicleVisualizer with an invalid port number. Using the default port.")
        m_port = 48110;
    }
}

void
VehicleVisualizer::setHTTPPort(int port)
{
    // Set the default port when an invalid port is specified
    if (port >= 1 && port <= 65535)
    {
        m_httpPort = port;
    }
    else
    {
        LogError("Error: called setPort for vehicleVisualizer with an invalid port number. Using the default port.")
        m_httpPort = 8080;
    }
}