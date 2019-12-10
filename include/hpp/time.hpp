#pragma once

#include <algorithm>
#include <string>
#include <sys/time.h>

#include "type_def.h"

using namespace std;

const uint32 SECS_PER_DAY = 60 *60 *24;

static inline string GetYYYYMMDD(time_t t)
{
    struct tm sttm;
    localtime_r(&t, &sttm);

    static char buf[12] = {0};
    sprintf(buf, "_%d_%02d_%02d", sttm.tm_year + 1900, sttm.tm_mon + 1, sttm.tm_mday);

    return string(buf);
}

static inline uint32 GetDiffDays(time_t time1, time_t time2)
{
    if (time1 > time2) std::swap(time1, time2);

    struct tm tm1, tm2;
    localtime_r(&time1, &tm1);
    localtime_r(&time2, &tm2);
    tm1.tm_hour = 0;
    tm1.tm_min = 0;
    tm1.tm_sec = 0;
    tm2.tm_hour = 0;
    tm2.tm_min = 0;
    tm2.tm_sec = 0;

    time1 = mktime(&tm1);
    time2 = mktime(&tm2);

    return (time2 - time1) /SECS_PER_DAY;
}

static inline void GetSecAndUsec(uint32& sec, uint32& usec)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
    {
        sec = tv.tv_sec;
        usec = tv.tv_usec;
    }
}

