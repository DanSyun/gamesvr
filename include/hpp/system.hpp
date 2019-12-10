#pragma once

#include <signal.h>

namespace dan
{

typedef void (*SignalHandler)(int);

static inline int system(const char* cmd)
{
    SignalHandler old_handler = signal(SIGCHLD, SIG_DFL);
    int ret = ::system(cmd);
    // LOG_INFO("curl ret: %d", ret);
    signal(SIGCHLD, old_handler);

    return ret;
}

}

