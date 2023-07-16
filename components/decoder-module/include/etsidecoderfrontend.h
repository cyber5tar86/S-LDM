/**
 * @file etsidecoderfrontend.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef S_LDM_ETSIDECODERFRONTEND_H
#define S_LDM_ETSIDECODERFRONTEND_H

#include <cinttypes>
#include <cstddef>
#include "namedenums.h"

/**
 * @brief
 *
 */
#define ETSI_DECODER_OK 0

/**
 * @brief
 *
 */
#define ETSI_DECODER_ERROR 1

//
/**
 * @brief Defined as "named enum" (see named_enums.h)
 *        Only the supported message types should be listed here
 *        The format to add a new supported type for decoding is:
 *          MSGTYPE(<message name>,<messageID>)
 *
 */
#define MSGTYPES(MSGTYPE) \
    MSGTYPE(DENM, = 1)    \
    MSGTYPE(CAM, = 2)

/**
 * @brief Construct a new named enum declare object
 *
 */
NAMED_ENUM_DECLARE(etsi_message_t, MSGTYPES);

namespace etsiDecoder
{
/**
 * @brief
 *
 */
typedef enum
{
    ETSI_DECODED_ERROR,     //!<
    ETSI_DECODED_CAM,       //!<
    ETSI_DECODED_DENM,      //!<
    ETSI_DECODED_IVIM,      //!<
    ETSI_DECODED_CPM,       //!<
    ETSI_DECODED_CAM_NOGN,  //!<
    ETSI_DECODED_DENM_NOGN, //!<
    ETSI_DECODED_IVIM_NOGN, //!<
    ETSI_DECODED_CPM_NOGN   //!<
} etsiDecodedType_e;

/**
 * @brief
 *
 */
typedef struct etsiDecodedData
{
    void *decoded_msg;      //!<
    etsiDecodedType_e type; //!<
    uint32_t gnTimestamp;   //!<
    // For DENMs GeoArea
    int32_t posLong; //!<
    int32_t posLat;  //!<
    uint16_t distA;  //!<
    uint16_t distB;  //!<
    uint16_t angle;  //!<
} etsiDecodedData_t;

/**
 * @brief
 *
 */
class decoderFrontend
{
public:
    /**
     * @brief 
     * 
     */
    typedef enum
    {
        /**
         * @brief The message to be decoded is a full ITS message (Facilities + BTP + GeoNetworking)
         * 
         */
        MSGTYPE_ITS = 0,
        /**
         * @brief The message to be decoded is a "pure" Facilities layer message (just a CAM or DENM without BTP and GeoNetworking)
         * 
         */
        MSGTYPE_FACILITYONLY = 1,
        /**
         * @brief MSGTYPE_AUTO is using a quite greedy algorithm to try to detect whether we receive either a full ITS message
         *        (Facilities + BTP + GeoNetworking), or a Facilities layer message with no BTP and GeoNetworking. The detection
         *        may not always be 100% accurate under certain specific circumstances.
         *        Thus, if possible, the two other options are always preferred.
         * 
         */
        MSGTYPE_AUTO = 2
    } msgType_e;

    /**
     * @brief Construct a new decoder Frontend object
     * 
     */
    decoderFrontend();

    /**
     * @brief 
     * 
     * @param buffer 
     * @param buflen 
     * @param decoded_data 
     * @param msgtype 
     * @return int 
     */
    int decodeEtsi(uint8_t *buffer, size_t buflen, etsiDecodedData_t &decoded_data, msgType_e msgtype = MSGTYPE_ITS);

    /**
     * @brief Set the Print Packet object
     * 
     * @param print_pkt 
     */
    void setPrintPacket(bool print_pkt) { m_print_pkt = print_pkt; }

private:
    bool m_print_pkt{false};   //!<
};

} // namespace etsiDecoder

#endif // ETSIDECODERFRONTEND_H
