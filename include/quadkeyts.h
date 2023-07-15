/**
 * @file quadkeyts.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef QUADKEYTILESYSTEM_H
#define QUADKEYTILESYSTEM_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

namespace QuadKeys
{

/**
 * @brief 
 * 
 */
class QuadKeyTS
{
public:
    /**
     * @brief Construct a new Quad Key T S object
     * 
     */
    QuadKeyTS();

    /**
     * @brief 
     * 
     * @param levelOfDetail 
     * @return unsigned int 
     */
    unsigned int MapSize(int levelOfDetail);

    /**
     * @brief Set the Level Of Detail object
     * 
     * @param levelOfDetail 
     */
    void setLevelOfDetail(int levelOfDetail = 16);

    /**
     * @brief 
     * 
     * @param latitude 
     * @param longitude 
     * @return std::string 
     */
    std::string LatLonToQuadKey(double latitude, double longitude);

    /**
     * @brief 
     * 
     * @param min_latitude 
     * @param max_latitude 
     * @param min_longitude 
     * @param max_longitude 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> LatLonToQuadKeyRange(double min_latitude, double max_latitude, double min_longitude, double max_longitude);

    /**
     * @brief This method should be called on the output of LatLonToQuadKeyRange(), in order to consolidate together quadkeys, when possible,
     *        and reduce the size of the filter
     *        The input vector is passed by reference and it is thus modified by unifyQuadkeys()
     *        Warning: as of now, unifyQuadkeys may have some bugs which occurs for few particular input vectors
     *        Please use unifyQuadkeys2() for the time being
     * 
     * @param quadKeys 
     */
    void unifyQuadkeys(std::vector<std::string> &quadKeys);

    /**
     * @brief This is a different implementation of the unifyQuadkeys() function
     * 
     * @param quadKeys 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> unifyQuadkeys2(std::vector<std::string> quadKeys);

    /**
     * @brief 
     * 
     * @param quadKeys 
     */
    void checkdim(std::vector<std::string> &quadKeys);

    /**
     * @brief Get the Quad Key Filter object
     * 
     * @param min_latitude 
     * @param min_longitude 
     * @param max_latitude 
     * @param max_longitude 
     * @param cachefilefound 
     * @return std::string 
     */
    std::string getQuadKeyFilter(double min_latitude, double min_longitude, double max_latitude, double max_longitude, bool *cachefilefound = nullptr);

private:
    /**
     * @brief 
     * 
     * @param n 
     * @param minValue 
     * @param maxValue 
     * @return double 
     */
    double Clip(double n, double minValue, double maxValue);

    /**
     * @brief 
     * 
     * @param vi 
     * @param vk 
     * @param l 
     * @param chars 
     * @return true 
     * @return false 
     */
    static bool finder(std::string vi, std::string vk, int l, std::array<int, 4> &chars);

private:
    /*! @brief used to set the levelOfDetail indipendently */
    int m_levelOfDetail;

    /*! @brief */
    double m_latlon_variation;
};

} // namespace QuadKeyTS

#endif // QUADKEYTILESYSTEM_H