/**
 * @file log.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOG_H
#define LOG_H

#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/ndc.h>

#define LogError(_s)   LOG4CPLUS_ERROR(Log::logger, _s);
#define LogWarning(_s) LOG4CPLUS_WARN(Log::logger,  _s);
#define LogDebug(_s)   LOG4CPLUS_DEBUG(Log::logger,  _s);
#define LogInfo(_s)    LOG4CPLUS_INFO(Log::logger,  _s);
#define LogTrace(_s)   LOG4CPLUS_TRACE(Log::logger, _s);

class Log
{
public:
    ~Log() = default;

    static Log& getInstance();

    static log4cplus::Logger logger;

private:
    Log();

    Log(const Log&) = delete;

    Log& operator=(const Log&);

};

#endif /* LOG_H */