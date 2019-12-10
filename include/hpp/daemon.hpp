#pragma once

#include <signal.h>
#include <sys/stat.h>

static inline void Daemonize()
{
    int pid;
    if ((pid = fork()) != 0)
        exit(0);

    setsid();

    if (fork() != 0)
        exit(0);

    umask(0);

    close(0);
    close(1);
    close(2);

    signal(SIGHUP,  SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

