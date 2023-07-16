/**
 * @file shbheader.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SHBHEADER_H
#define SHBHEADER_H

#include <cstdint>
#include <cstring>
#include "utils.h"

namespace etsiDecoder
{
/**
 * @brief 
 * 
 */
class shbHeader
{
public:
    /**
     * @brief Construct a new shb Header object
     * 
     */
    shbHeader();

    /**
     * @brief Destroy the shb Header object
     * 
     */
    ~shbHeader();

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
    [[nodiscard]] GNlpv_t GetLongPositionV() const
    {
        return m_sourcePV;
    }

private:
    GNlpv_t m_sourcePV{};       //!< Source long position vector
    uint8_t m_reserved{0};      //!< aux variable for reading reserved fields
    uint32_t m_reserved32{0};   //!<
};

} // namespace etsiDecoder

#endif // SHBHEADER_H
