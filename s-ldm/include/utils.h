/**
 * @file utils.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SLDM_UTILS_H
#define SLDM_UTILS_H

#include <cinttypes>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <poll.h>
#include <sys/timerfd.h>
#include <string>

/**
 * @brief 
 * 
 */
#define POLL_DEFINE_JUNK_VARIABLE() long int junk

/**
 * @brief 
 * 
 */
#define POLL_CLEAR_EVENT(clockFd) junk = read(clockFd, &junk, sizeof(junk))

/**
 * @brief Get the timestamp us object
 * 
 * @return uint64_t 
 */
uint64_t get_timestamp_us(void);

/**
 * @brief Get the timestamp ns object
 * 
 * @return uint64_t 
 */
uint64_t get_timestamp_ns(void);

/**
 * @brief Get the timestamp ms gn object
 * 
 * @return uint64_t 
 */
uint64_t get_timestamp_ms_gn(void);

/**
 * @brief Get the timestamp ms cam object
 * 
 * @return uint64_t 
 */
uint64_t get_timestamp_ms_cam(void);

/**
 * @brief 
 * 
 * @param pollfd 
 * @param clockFd 
 * @param time_us 
 * @return int 
 */
int timer_fd_create(struct pollfd &pollfd, int &clockFd, uint64_t time_us);

/**
 * @brief 
 * 
 * @param extLights 
 * @return std::string 
 */
std::string exteriorLights_bit_to_string(uint8_t extLights);

/**
 * @brief 
 * 
 * @param d1 
 * @param d2 
 * @param eps 
 * @return true 
 * @return false 
 */
bool doublecomp(double d1, double d2, double eps = 0.0001);

/**
 * @brief logfprintf is an alternative to fprintf for logging purposes
 *        It works just like fprintf, by printing:
 *        "[LOG - <modulename>] (<current date and time>) <fprintf content>"
 *        As it is retrieving the time for each call to logfprintf() it is expected
 *        be slightly slower than fprintf and should thus be used only when really needed
 * 
 * @param stream 
 * @param modulename 
 * @param format 
 * @param ... 
 * @return int 
 */
int logfprintf(FILE *stream, std::string modulename, const char *format, ...);

#endif // SLDM_UTILS_H