#pragma once

#include "jemalloc.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "base.h"

#define MAX_PROXY_NUM   (8)
#define SOCKET_CLOSED   (-1)

#define RECV_BUF_LEN    (MAX_PROXY_BUF_LEN* 20)   // at least 20 package
#define SEND_BUF_LEN    (MAX_PROXY_BUF_LEN* 20)

struct stSvrCliSocket
{
    int32   _fd;
    char    _recv_buf[RECV_BUF_LEN];
    uint32  _recv_offset;
    char    _send_buf[SEND_BUF_LEN];
    uint32  _send_offset;

    string  _ip;
    uint32  _port;
};

template <class T>
class ProxyManager
{
public:
    ProxyManager(T* pserver): _pserver(pserver) {}

    bool Init()
    {
        auto& conf = _pserver->GetSvrConf();
        if (conf.proxy_addrs_size() == 0)
            return false;
        if (conf.proxy_addrs_size() > MAX_PROXY_NUM)
            return false;

        _pool = new stSvrCliSocket[conf.proxy_addrs_size()];
        for (auto i = 0; i < conf.proxy_addrs_size(); ++i)
        {
            _pool[i]._recv_offset = 0;
            _pool[i]._send_offset = 0;
            _pool[i]._ip = conf.proxy_addrs(i).ip();
            _pool[i]._port = conf.proxy_addrs(i).port();

            if (!Connect(i))
                return false;
            if (!Register(i))
                return false;
        }
        return true;
    }
    void CheckConnect()
    {
        for (uint32 i = 0; i < GetProxyNum(); ++i)
        {
            auto& cli = _pool[i];
            if (cli._fd != SOCKET_CLOSED)
                return;

            if (!Connect(i))
                return;

            if (!Register(i))
                return;

            SendBufData(i);
        }
    }
    void SendProxyBuf()
    {
        for (uint32 i = 0; i < GetProxyNum(); ++i)
        {
            auto& cli = _pool[i];
            if (cli._fd == SOCKET_CLOSED || cli._send_offset == 0) continue;
            SendBufData(i);
        }
    }
    void GetProxyData()
    {
        // select fds
        fd_set fds;
        FD_ZERO(&fds);
        for (uint32 i = 0; i < GetProxyNum(); ++i)
        {
            if (_pool[i]._fd != SOCKET_CLOSED)
            {
                FD_SET(_pool[i]._fd, &fds);
            }
        }

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 5000;  // 5ms
        select(FD_SETSIZE, &fds, NULL, NULL, &tv);
        // int fd_num = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
        // if (fd_num <= 0) return;

        // recv data
        for (uint32 i = 0; i < GetProxyNum(); ++i)
        {
            if (FD_ISSET(_pool[i]._fd, &fds))
            {
                RecvData(i);
            }
        }

        // post to msg queue
        for (uint32 i = 0; i < GetProxyNum(); ++i)
        {
            uint32 count = 0;
            uint32 max_count = _pserver->GetSvrConf().msg_shm_size() / GetProxyNum() / 4;

            while (GetOneData(i, _recv_send_buf, sizeof(_recv_send_buf)))
            {
                uint32 len = *(uint32*)_recv_send_buf;
                if (len <= 4 + sizeof(stProxyHead)) continue;

                stProxyHead* phead = (stProxyHead*)(_recv_send_buf + 4);
                char* pb_buf = _recv_send_buf + 4 + sizeof(stProxyHead);
                uint32 pb_len = len - 4 - sizeof(stProxyHead);

                if (!_pserver->PostMessage(*phead, pb_buf, pb_len))
                    return;
                if (++count > max_count)
                    break;
            }
        }
    }
    bool SendProxyData(const stProxyHead& head, const char* buf, uint32 buf_len, int des_proxy_id)
    {
        static uint32 send_count = 0;
        if (des_proxy_id == -1)
        {
            switch (head._route_type)
            {
                case proxy_route_hash:
                    des_proxy_id = head._hash_key % GetProxyNum();
                    break;
                default:
                    des_proxy_id = send_count++ % GetProxyNum();
                    break;
            }
        }

        if (des_proxy_id >= 0 && des_proxy_id < (int)GetProxyNum())
        {
            _recv_send_len = 0;
            // 包长
            uint32 total_len = 4 + sizeof(head) + buf_len;
            memcpy(&_recv_send_buf[_recv_send_len], &total_len, sizeof(total_len));
            _recv_send_len += sizeof(total_len);
            // center 头
            memcpy(&_recv_send_buf[_recv_send_len], &head, sizeof(head));
            _recv_send_len += sizeof(head);
            // 数据
            memcpy(&_recv_send_buf[_recv_send_len], buf, buf_len);
            _recv_send_len += buf_len;

            SendOneData(des_proxy_id, _recv_send_buf, _recv_send_len);

            return true;
        }
        else
        {
            return false;
        }
    }

private:
    uint32 GetProxyNum()
    {
        return _pserver->GetSvrConf().proxy_addrs_size();
    }
    bool Connect(uint32 proxy_id)
    {
        auto& cli = _pool[proxy_id];

        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) return false;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(cli._port);
        addr.sin_addr.s_addr = inet_addr(cli._ip.c_str());
        if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) return false;

        uint32 buf_size = 102400;
        if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) == -1) return false;
        if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) return false;

        int flag = -1;
        if ((flag = fcntl(fd, F_GETFL, 0)) == -1)
            return false;
        if (fcntl(fd, F_SETFL, flag| O_NONBLOCK) == -1)
            return false;

        cli._fd = fd;

        return true;
    }
    void DisConnect(uint32 proxy_id)
    {
        auto& cli = _pool[proxy_id];
        close(cli._fd);
        cli._fd = SOCKET_CLOSED;
        cli._recv_offset = 0;
    }
    bool Register(uint32 proxy_id)
    {
        auto& cli = _pool[proxy_id];
        while (true)
        {
            int len = send(cli._fd, (char*)&_pserver->GetSvrInfo(), sizeof(_pserver->GetSvrInfo()), 0);
            if (len == -1)
            {
                if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)  // try again
                    continue;
                else    // failed
                {
                    return false;
                }
            }
            else    // succ
            {
                return true;
            }
        }
    }
    void RecvData(uint32 proxy_id)
    {
        auto& cli = _pool[proxy_id];
        int recv_len;
        do
        {
            if (sizeof(cli._recv_buf) == cli._recv_offset) return;
            recv_len = recv(cli._fd, cli._recv_buf + cli._recv_offset, sizeof(cli._recv_buf) - cli._recv_offset, 0);
            if (recv_len == 0)
            {
                close(cli._fd);
                cli._fd = SOCKET_CLOSED;
                cli._recv_offset = 0;
                return;
            }
            if (recv_len == -1)
            {
                if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
                {
                    DisConnect(proxy_id);
                }
                return;
            }
            cli._recv_offset += recv_len;
        } while (recv_len > 0);
    }
    bool GetOneData(uint32 proxy_id, char* buf, uint32 buf_len)
    {
        auto& cli = _pool[proxy_id];
        if (cli._recv_offset <= 4) return false;

        uint32 len = *(uint32*)cli._recv_buf;
        if (len > cli._recv_offset) return false;

        memcpy(buf, cli._recv_buf, len);
        cli._recv_offset -= len;

        if (cli._recv_offset != 0)
        {
            memmove(cli._recv_buf, cli._recv_buf + len, cli._recv_offset);
        }
        return true;
    }
    bool SendBufData(uint32 proxy_id)
    {
        auto& cli = _pool[proxy_id];
        if (cli._send_offset == 0)
            return true;

        int ret = send(cli._fd, cli._send_buf, cli._send_offset, 0);
        if (ret == -1)
        {
            if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
            {
                DisConnect(proxy_id);
            }
            return false;
        }
        else
        {
            cli._send_offset = 0;
            return true;
        }
    }
    void SendOneData(uint32 proxy_id, const char* buf, uint32 buf_len)
    {
        auto& cli = _pool[proxy_id];
        // 如果已断开直接缓存等待重连
        if (cli._fd == SOCKET_CLOSED)
        {
            ReserveData(proxy_id, buf, buf_len);
            return;
        }

        // 先发送缓存
        if (!SendBufData(proxy_id))
        {
            // 发送失败则直接缓存本次数据
            ReserveData(proxy_id, buf, buf_len);
            return;
        }

        // 再发送本次数据
        int ret = send(cli._fd, buf, buf_len, 0);
        if (ret == -1)
        {
            if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
            {
                DisConnect(proxy_id);
            }
            ReserveData(proxy_id, buf, buf_len);
        }
    }
    void ReserveData(uint32 proxy_id, const char* buf, uint32 buf_len)
    {
        auto& cli = _pool[proxy_id];
        // 如果满了就丢掉防止雪崩
        if (cli._send_offset + buf_len > sizeof(cli._send_buf))
            cli._send_offset = 0;

        memcpy(&cli._send_buf[cli._send_offset], buf, buf_len);
        cli._send_offset += buf_len;
    }

private:
    T* _pserver;
    stSvrCliSocket* _pool;

    char _recv_send_buf[MAX_PROXY_BUF_LEN];
    uint32 _recv_send_len;
};

