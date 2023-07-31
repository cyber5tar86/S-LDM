/**
 * @file btp.cpp
 * @author carlos
 * @brief 
 * @version 0.1
 * @date 2021-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <common/log.h>
#include <iostream>
#include "btp.h"

namespace etsiDecoder
{

Btp::Btp()
{
    log4cplus::NDCContextCreator context(LOG4CPLUS_TEXT("Btp"));
}

Btp::~Btp() = default;

btpError_e
Btp::decodeBTP(GNDataIndication_t dataIndication, BTPDataIndication_t *btpDataIndication)
{
    btpHeader header;

    btpDataIndication->data = dataIndication.data;

    header.removeHeader(btpDataIndication->data);
    btpDataIndication->data += 4;

    btpDataIndication->BTPType = dataIndication.upperProtocol;

    if ((header.getDestPort() != CA_PORT) && (header.getDestPort() != DEN_PORT))
    {
        LogError("[Decoder] BTP port not supported");
        return BTP_ERROR;
    }

    btpDataIndication->destPort = header.getDestPort();

    if (btpDataIndication->BTPType == BTP_A)
    {
        btpDataIndication->sourcePort = header.getSourcePort();
        btpDataIndication->destPInfo = 0;
    }
    else if (btpDataIndication->BTPType == BTP_B) // BTP-B
    {
        btpDataIndication->destPInfo = header.getDestPortInfo();
        btpDataIndication->sourcePort = 0;
    }
    else
    {
        LogError("[Decoder] Incorrect transport protocol ");
        return BTP_ERROR;
    }

    btpDataIndication->GnAddress = dataIndication.GnAddressDest;
    btpDataIndication->GNTraClass = dataIndication.GNTraClass;
    btpDataIndication->GNRemPLife = dataIndication.GNRemainingLife;
    btpDataIndication->GNPositionV = dataIndication.SourcePV;
    btpDataIndication->data = dataIndication.data + 4;
    btpDataIndication->lenght = dataIndication.lenght - 4;

    return BTP_OK;
}

} // namespace etsiDecoder
