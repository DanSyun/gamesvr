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
#include "shm_pipe.h"
#include "share_mem.hpp"

#define NONE_SOCKET     (0)
#define LISTEN_SOCKET   (1)
#define CLIENT_SOCKET   (2)
#define SOCKET_CLOSED   (-1)

#define RECV_BUF_LEN    (CONNECT_HEAD_LEN + (MAX_CS_PKG_BUF_LEN + MAX_CS_WS_HEAD_LEN) * 2)   // at least 2 package

struct stWsCliSocket
{
    int     _fd;
    int     _type;
    int64   _logic_index;    // logic对象索引，通常是uid
    char*   _recv_buf;
    uint32  _buf_offset;
    uint64  _create_time;
    uint32  _update_time;
    uint16  _socket_port;
    char    _socket_addr[16];
    uint16  _socket_pos;
    bool    _hand_shaked;
};


class ConnectWsServer
{
public:
    int Initialize(int argc, char* argv[]);
    void Run();

private:
    void ParseArgs(int argc, char* argv[], char* conf, bool& daemon);
    bool LoadConfig(const char* conf);
    bool InitNetwork();

    bool SocketPoolInit();
    stWsCliSocket* SocketCreate();
    stWsCliSocket* SocketGet(uint16 pos) { return &_pool[pos]; }
    void SocketRelease(uint16 pos, bool need_notify_logic = true);

    bool EpollInit();
    bool EpollAdd(int fd, uint16 pos) { _event.data.fd = pos; return epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &_event) == -1? false: true; }
    bool EpollDel(int fd) { return epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1? false: true; }

    bool InitCsShmPipe();

    bool CheckRunFlag();
    void OnRoutineCheck();
    void DealNetwork();
    void AcceptConnect();
    void RecvClientData(stWsCliSocket& socket);
    void SendClientData();
    void NotifyLogicDiscon(const stWsCliSocket& socket);
    void WrapWebsocketHead(char* buf, uint32& buf_len);
private:
    ConnectSvrConf _conf;

    int _lsfd;
    stWsCliSocket* _pool;
    char* _all_recv_buf;
    FREE_LIST _free_list;

    int _epfd;
    struct epoll_event _event;
    struct epoll_event* _events;

    ShareMem* _pcs_shm;
    ShmPipe* _pcs_pipe;
    ShareMem* _psc_shm;
    ShmPipe* _psc_pipe;

    char _send_buf[MAX_CONNECT_BUF_LEN + MAX_SC_WS_HEAD_LEN];
    uint32 _send_len;
};

