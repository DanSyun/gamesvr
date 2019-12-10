#pragma once

#include "jemalloc.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base.h"

template <class T>
class EventManager
{
public:
    EventManager(T* pserver): _pserver(pserver) {}
    bool Init()
    {
        if (GetEventNum() == 0)
            return false;

        _fd = new int[GetEventNum()];
        _addr = new sockaddr_in[GetEventNum()];

        auto& conf = _pserver->GetSvrConf();
        for (auto i = 0; i < conf.event_addrs_size(); ++i)
        {
            _fd[i] = socket(AF_INET, SOCK_DGRAM, 0);
            if (_fd[i] == -1)
                return false;

            _addr[i].sin_family = AF_INET;
            _addr[i].sin_port = htons(conf.event_addrs(i).port());
            _addr[i].sin_addr.s_addr = inet_addr(conf.event_addrs(i).ip().c_str());
        }

        return true;
    }
    bool SendEvent(char* buf, uint32 buf_len)
    {
        static uint32 count = 0;
        auto index = count++ % GetEventNum();

        int ret = sendto(_fd[index], buf, buf_len, 0, (const sockaddr*)&_addr[index], sizeof(_addr[index]));

        return (ret == (int)buf_len);
    }
    uint32 GetEventNum() { return _pserver->GetSvrConf().event_addrs_size(); }
private:
    T* _pserver;
    int* _fd;
    sockaddr_in* _addr;
    uint8 _buf[MAX_EVENT_BUF_LEN];
    uint32 _buf_len;
};

