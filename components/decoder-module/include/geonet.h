/**
 * @file geonet.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GEONET_H
#define GEONET_H

#include <cstdint>
#include <map>
#include <mutex>
#include <set>
#include "basicheader.h"
#include "commonheader.h"
#include "gbcheader.h"
#include "shbheader.h"

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class GeoNet
{
public:
    /**
     * @brief Construct a new Geo Net object
     * 
     */
    GeoNet();

    /**
     * @brief Destroy the Geo Net object
     * 
     */
    ~GeoNet();

    /**
     * @brief 
     * 
     * @param packet 
     * @param dataIndication 
     * @return gnError_e 
     */
    gnError_e decodeGN(unsigned char *packet, GNDataIndication_t *dataIndication);

private:
    /**
     * @brief 
     * 
     * @param dataIndication 
     * @return GNDataIndication_t* 
     */
    GNDataIndication_t *processSHB(GNDataIndication_t *dataIndication);

    /**
     * @brief 
     * 
     * @param dataIndication 
     * @param shape 
     * @return GNDataIndication_t* 
     */
    GNDataIndication_t *processGBC(GNDataIndication_t *dataIndication, uint8_t shape);

    /**
     * @brief 
     * 
     * @param lifeTime 
     * @param seconds 
     * @return true 
     * @return false 
     */
    bool decodeLT(uint8_t lifeTime, double *seconds);

    // ETSI 302 636-4-1 ANNEX H: GeoNetworking protocol constans
    uint8_t m_GnPtotocolVersion = 1;                //!< 
    uint8_t m_GnIfType = 1;                         //!< 
    uint32_t m_GnPaiInterval = 80;                  //!< 
    uint32_t m_GnMaxSduSize = 1398;                 //!< 
    uint8_t m_GnMaxGeoNetworkingHeaderSize = 88;    //!< 
    uint8_t m_GnSecurity = 0;                       //! Disabled
    uint16_t m_GnDefaultTrafficClass = 0;           //!< s
};

} // namespace GEONET_H

#endif // GEONET_H
