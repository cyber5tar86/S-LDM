/**
 * @file basicheader.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BASICHEADER_H
#define BASICHEADER_H

#include <cstdint>

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class basicHeader
{
public:
    /**
     * @brief Construct a new basic Header object
     * 
     */
    basicHeader();

    /**
     * @brief Destroy the basic Header object
     * 
     */
    ~basicHeader();

    /**
     * @brief 
     * 
     * @param buffer 
     */
    void removeHeader(unsigned char * buffer);

    //Getters

    /**
     * @brief Get the Version object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetVersion() const
    {
        return m_version;
    }

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
     * @brief Get the Life Time object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetLifeTime() const
    {
        return m_lifeTime;
    }

    /**
     * @brief Get the Remaining H L object
     * 
     * @return uint8_t 
     */
    [[nodiscard]] uint8_t GetRemainingHL() const
    {
        return m_remainingHopLimit;
    }

private:
    uint8_t m_version: 4;           //!< 
    uint8_t m_nextHeader: 4;        //!< 
    uint8_t m_reserved{0};          //!< 
    uint8_t m_lifeTime{0};          //!< 
    uint8_t m_remainingHopLimit{0}; //!<
};

} // namespace etsiDecoder

#endif // BASICHEADER_H