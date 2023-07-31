/**
 * @file sbhheader.cpp
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <common/log.h>
#include <cstring>
#include "shbheader.h"

namespace etsiDecoder
{
    shbHeader::shbHeader()
    {
        log4cplus::NDCContextCreator context(LOG4CPLUS_TEXT("SbhHeader"));
    }

    shbHeader::~shbHeader() = default;

    void shbHeader::removeHeader(unsigned char *buffer)
    {

        // Source long position vector
        memcpy(m_sourcePV.GnAddress, buffer, 8);
        buffer += 8;
        memcpy(&m_sourcePV.TST, buffer, sizeof(uint32_t));
        buffer += 4;
        m_sourcePV.TST = swap_32bit(m_sourcePV.TST);
        memcpy(&m_sourcePV.latitude, buffer, sizeof(uint32_t));
        buffer += 4;
        m_sourcePV.latitude = swap_32bit(m_sourcePV.latitude);
        memcpy(&m_sourcePV.longitude, buffer, sizeof(uint32_t));
        buffer += 4;
        m_sourcePV.longitude = swap_32bit(m_sourcePV.longitude);
        uint16_t pai_speed = 0;
        memcpy(&pai_speed, buffer, sizeof(uint16_t));
        buffer += 2;
        pai_speed = swap_16bit(pai_speed);
        m_sourcePV.positionAccuracy = pai_speed >> 15;
        m_sourcePV.speed = pai_speed & 0x7f;
        memcpy(&m_sourcePV.heading, buffer, sizeof(uint16_t));
        buffer += 2;
        m_sourcePV.heading = swap_16bit(m_sourcePV.heading);
    }
}