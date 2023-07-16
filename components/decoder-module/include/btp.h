/**
 * @file btp.h
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BTP_H
#define BTP_H

#include <cstdint>
#include <cstring>
#include "btpheader.h"
#include "utils.h"

/**
 * @brief 
 * 
 */
#define CA_PORT 2001

/**
 * @brief 
 * 
 */
#define DEN_PORT 2002

namespace etsiDecoder
{

/**
 * @brief 
 * 
 */
class btp
{
public :
    /**
     * @brief Construct a new btp object
     * 
     */
    btp();

    /**
     * @brief Destroy the btp object
     * 
     */
    ~btp();

    /**
     * @brief 
     * 
     * @param dataIndication 
     * @param btpDataIndication 
     * @return btpError_e 
     */
    btpError_e decodeBTP(GNDataIndication_t dataIndication, BTPDataIndication_t* btpDataIndication);
};

} // namespace etsiDecoder

#endif // BTP_H