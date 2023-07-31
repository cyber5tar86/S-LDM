/**
 * @file basicheader.cpp
 * @author carlos
 * @brief
 * @version 0.1
 * @date 2021-05-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <common/log.h>
#include "basicheader.h"

namespace etsiDecoder
{

basicHeader::basicHeader()
{
    log4cplus::NDCContextCreator context(LOG4CPLUS_TEXT("BasicHeader"));

    m_version = 0;
    m_nextHeader = 0;
    m_reserved = 0;
    m_lifeTime = 0;
    m_remainingHopLimit = 0;
}

basicHeader::~basicHeader() = default;

void
basicHeader::removeHeader(unsigned char *buffer)
{
    uint8_t version_NH = 0;
    version_NH = (uint8_t)*buffer;
    m_version = version_NH >> 4;
    m_nextHeader = version_NH & 0x0f;

    buffer++;
    m_reserved = (uint8_t)*buffer;
    buffer++;
    m_lifeTime = (uint8_t)*buffer;
    buffer++;
    m_remainingHopLimit = (uint8_t)*buffer;
}

} // namespace etsiDecoder