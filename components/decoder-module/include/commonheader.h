/**
 * @file commonheader.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef COMMONHEADER_H
#define COMMONHEADER_H

#include <cstdint>
#include "utils.h"

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class commonHeader
{
public:
    /**
     * @brief Construct a new common Header object
     * 
     */
    commonHeader();

    /**
     * @brief Destroy the common Header object
     * 
     */
    ~commonHeader();

    /**
     * @brief 
     * 
     * @param buffer 
     */
    void removeHeader(unsigned char *buffer);

    // Getters
    /**
     * @brief Get the Next Header object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetNextHeader() const
    {
        return m_nextHeader;
    }

    /**
     * @brief Get the Header Type object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetHeaderType() const
    {
        return m_headerType;
    }

    /**
     * @brief Get the Header Sub Type object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetHeaderSubType() const
    {
        return m_headerSubType;
    }

    /**
     * @brief Get the Traffic Class object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetTrafficClass() const
    {
        return m_trafficClass;
    }

    /**
     * @brief Get the Flag object
     * 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool GetFlag() const
    {
        return m_flag;
    }

    /**
     * @brief Get the Payload object
     * 
     * @return uint16_t 
     */
    [[nodiscard]] uint16_t GetPayload() const
    {
        return m_payload;
    }

    /**
     * @brief Get the Max Hop Limit object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetMaxHopLimit() const
    {
        return m_maxHopLimit;
    }

private:
    uint8_t m_nextHeader : 4;       //!<
    uint8_t m_headerType : 4;       //!<
    uint8_t m_headerSubType : 4;    //!<
    uint8_t m_trafficClass;         //!<
    bool m_flag : 1;                //!<
    uint16_t m_payload;             //!<
    uint8_t m_maxHopLimit;          //!<
    uint8_t m_reserved;             //!<
};

} // namespace etsiDecoder

#endif // COMMONHEADER_H
