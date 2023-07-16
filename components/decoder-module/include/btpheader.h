/**
 * @file btpheader.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BTPHEADER_H
#define BTPHEADER_H

#include <cstdint>
#include <cstring>
#include "utils.h"

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class btpHeader
{
public:
    /**
     * @brief Construct a new btp Header object
     * 
     */
    btpHeader();

    /**
     * @brief Destroy the btp Header object
     * 
     */
    ~btpHeader();

    /**
     * @brief 
     * 
     * @param buffer 
     */
    void removeHeader(unsigned char * buffer);

    //getters
    /**
     * @brief Get the Dest Port object
     * 
     * @return uint16_t 
     */
    [[nodiscard]] uint16_t getDestPort() const
    {
        return m_destinationPort;
    }

    /**
     * @brief Get the Source Port object
     * 
     * @return uint16_t 
     */
    [[nodiscard]] uint16_t getSourcePort() const
    {
        return m_source_destInfo;
    }

    /**
     * @brief Get the Dest Port Info object
     * 
     * @return uint16_t 
     */
    [[nodiscard]] uint16_t getDestPortInfo() const
    {
        return m_source_destInfo;
    }

private:
    uint16_t m_destinationPort{0}; //!< Destination port
    uint16_t m_source_destInfo{0}; //!< Source port/Destination port info
};

} // namespace etsiDecoder

#endif // BTPHEADER_H