#ifndef SLDM_UTILS_H
#define SLDM_UTILS_H

#include <cinttypes>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <poll.h>
#include <sys/timerfd.h>

#define POLL_DEFINE_JUNK_VARIABLE() long int junk
#define POLL_CLEAR_EVENT(clockFd) read(clockFd,&junk,sizeof(junk))

uint64_t get_timestamp_us(void);
int timer_fd_create(struct pollfd &pollfd,int &clockFd,uint64_t time_us);

#endif // SLDM_UTILS_H