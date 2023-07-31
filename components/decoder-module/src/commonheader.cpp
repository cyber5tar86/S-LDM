/**
 * @file commonheader.cpp
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
#include "commonheader.h"

namespace etsiDecoder
{
commonHeader::commonHeader()
{
    log4cplus::NDCContextCreator context(LOG4CPLUS_TEXT("CommonHeader"));

    m_nextHeader = 0;
    m_headerType = 0;
    m_headerSubType = 0;
    m_trafficClass = 0;
    m_flag = false;
    m_payload = 0;
    m_maxHopLimit = 0;
    m_reserved = 0;
}

commonHeader::~commonHeader() = default;

void
commonHeader::removeHeader(unsigned char *buffer)
{

    uint8_t chNH = 0;
    chNH = (uint8_t)*buffer;
    buffer++;
    m_nextHeader = chNH >> 4;
    uint8_t headerType = 0;
    headerType = (uint8_t)*buffer;
    buffer++;
    m_headerType = headerType >> 4;
    m_headerSubType = headerType & 0x0f;
    m_trafficClass = (uint8_t)*buffer;
    buffer++;
    uint8_t chflag = 0;
    chflag = (uint8_t)*buffer;
    buffer++;
    m_flag = chflag >> 7;
    memcpy(&m_payload, buffer, sizeof(uint16_t));
    buffer += 2;
    m_payload = swap_16bit(m_payload);
    m_maxHopLimit = (uint8_t)*buffer;
    buffer++;
    m_reserved = (uint8_t)*buffer;
    buffer++;
}

} // namespace etsiDecoder