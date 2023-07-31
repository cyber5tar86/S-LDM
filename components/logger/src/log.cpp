/**
 * @file log.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "common/log.h"

Log::Log()
{
    log4cplus::Initializer initializer;

    try
    {
        log4cplus::Logger root = log4cplus::Logger::getRoot();
        log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("config/log4cplus.config"));
        log4cplus::initialize();
        logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("SLDM"));
    }
    catch (...)
    {
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Exception occured..."));
    }
}
