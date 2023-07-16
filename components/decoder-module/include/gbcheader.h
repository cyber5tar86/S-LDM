/**
 * @file gbcheader.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef GBCHEADER_H
#define GBCHEADER_H

#include <cstdint>
#include <cstring>
#include "utils.h"

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class gbcHeader
{
public:
    /**
     * @brief Construct a new gbc Header object
     * 
     */
    gbcHeader();

    /**
     * @brief Destroy the gbc Header object
     * 
     */
    ~gbcHeader();

    /**
     * @brief 
     * 
     * @param buffer 
     */
    void removeHeader(unsigned char *buffer);

    // Getters

    /**
     * @brief Get the Long Position V object
     * 
     * @return GNlpv_t 
     */
    GNlpv_t GetLongPositionV(void) const
    {
        return m_sourcePV;
    }

    /**
     * @brief Get the Seq Number object
     * 
     * @return uint16_t 
     */
    uint16_t GetSeqNumber(void) const
    {
        return m_seqNumber;
    }

    /**
     * @brief Get the Pos Long object
     * 
     * @return int32_t 
     */
    int32_t GetPosLong(void) const
    {
        return m_posLong;
    }

    /**
     * @brief Get the Pos Lat object
     * 
     * @return int32_t 
     */
    int32_t GetPosLat(void) const
    {
        return m_posLat;
    }

    /**
     * @brief Get the Dist A object
     * 
     * @return uint16_t 
     */
    uint16_t GetDistA(void) const
    {
        return m_distA;
    }

    /**
     * @brief Get the Dist B object
     * 
     * @return uint16_t 
     */
    uint16_t GetDistB(void) const
    {
        return m_distB;
    }

    /**
     * @brief Get the Angle object
     * 
     * @return uint16_t 
     */
    uint16_t GetAngle(void) const
    {
        return m_angle;
    }

    /**
     * @brief Get the Geo Area object
     * 
     * @return GeoArea_t 
     */
    GeoArea_t GetGeoArea(void) const;

private:
    GNlpv_t m_sourcePV{};       //!<  Source long position vector
    uint16_t m_seqNumber{0};    //!<
    int32_t m_posLong{0};       //!<
    int32_t m_posLat{0};        //!<
    uint16_t m_distA{0};        //!<
    uint16_t m_distB{0};        //!<
    uint16_t m_angle{0};        //!<
    uint16_t m_reserved{0};     //! aux variable for reading reserved fields
};

} // namespace etsiDecoder

#endif // GBCHEADER_H
