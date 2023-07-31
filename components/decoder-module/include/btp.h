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

#include <common/log.h>
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
class Btp
{
public :
    /**
     * @brief Construct a new btp object
     * 
     */
    Btp();

    /**
     * @brief Destroy the btp object
     * 
     */
    ~Btp();

    /**
     * @brief 
     * 
     * @param dataIndication 
     * @param btpDataIndication 
     * @return btpError_e 
     */
    btpError_e decodeBTP(GNDataIndication_t dataIndication, BTPDataIndication_t* btpDataIndication);

private:
    std::string mLogTag{"[Btp] "};
};

} // namespace etsiDecoder

#endif // BTP_H