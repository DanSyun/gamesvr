#pragma once

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <list>

#include "base.h"
#include "server_manager.h"

#define LISTEN_SOCKET   (1)
#define CLIENT_SOCKET   (2)
#define SOCKET_CLOSED   (-1)

#define RECV_BUF_LEN    (MAX_PROXY_BUF_LEN* 20)   // at least 20 package
#define SEND_BUF_LEN    (MAX_PROXY_BUF_LEN* 20)

// tcp socket
struct stSvrSocket
{
    int     _fd;
    uint32  _recv_offset;
    uint32  _send_offset;
    uint16  _socket_pos;
    uint8   _type;
    char*   _recv_buf;
    char    _send_buf[SEND_BUF_LEN];
};

class ServerManager;
class ProxyServer
{
public:
    int Initialize(int argc, char* argv[]);
    void Run();

    void ParseArgs(int argc, char* argv[], char* conf, bool& daemon);
    bool LoadConfig(const char* conf);
    bool InitNetwork();

    bool CheckRunFlag();
    void DealNetwork();
    void AcceptConnect();
    void RecvServerData(stSvrSocket& socket);

    bool SocketPoolInit();
    stSvrSocket* SocketCreate();
    stSvrSocket* SocketGet(uint16 pos) { return &_pool[pos]; }
    void SocketRelease(uint16 pos);
    void SocketDisconn(uint16 pos);

    bool EpollInit();
    bool EpollAdd(int fd, uint16 pos) { _event.data.fd = pos; return epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &_event) == -1? false: true; }
    bool EpollDel(int fd) { return epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1? false: true; }

    const ProxySvrConf& GetSvrConf() { return _conf; }
private:
    ProxySvrConf _conf;

    int _lsfd;
    stSvrSocket* _pool;
    char* _all_recv_buf;
    FREE_LIST _free_list;

    int _epfd;
    struct epoll_event _event;
    struct epoll_event* _events;

    ServerManager* _pserver_mng;
};


