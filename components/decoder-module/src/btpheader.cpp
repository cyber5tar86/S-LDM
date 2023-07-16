/**
 * @file btpheader.cpp
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "btpheader.h"

namespace etsiDecoder
{

btpHeader::btpHeader() = default;

btpHeader::~btpHeader() = default;

void
btpHeader::removeHeader(unsigned char *buffer)
{

    memcpy(&m_destinationPort, buffer, sizeof(uint16_t));
    buffer += 2;
    m_destinationPort = swap_16bit(m_destinationPort);

    memcpy(&m_source_destInfo, buffer, sizeof(uint16_t));
    buffer += 2;
    m_source_destInfo = swap_16bit(m_source_destInfo);
}

} // namespace etsiDecoder