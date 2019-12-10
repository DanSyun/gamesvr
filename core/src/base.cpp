#include "base.h"

uint64 g_cur_time = UpdateTime();

uint64 UpdateTime()
{
    uint32 sec = 0, usec = 0;
    GetSecAndUsec(sec, usec);
    g_cur_time = (uint64)sec* 1000 + usec/ 1000;
    return g_cur_time;
}

uint64 GetTimeMsec()
{
	return g_cur_time;
}

uint32 GetTime()
{
	return g_cur_time/ 1000;
}

RunOneInstance* g_pinstance = NULL;


