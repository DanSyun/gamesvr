#pragma once

#include <unistd.h>
#include <sys/file.h>

class RunOneInstance
{
public:
    RunOneInstance(const char* file_name): _running(false)
    {
        _lock_fd = open(file_name, O_RDWR|O_CREAT, 0640);
        if (_lock_fd > 0)
        {
            int ret = flock(_lock_fd, LOCK_EX | LOCK_NB);
            if (ret < 0)
            {
                cout << "lock file failed, server is already running!" << endl;
                _running =  true;
            }
        }
    }
    ~RunOneInstance()
    {
        if (_lock_fd > 0)
        {
            close(_lock_fd);
        }
    }
    
public:
    bool IsOneInstance()
    {
        return !_running;
    }

private:
    int _lock_fd;
    bool _running;
};

