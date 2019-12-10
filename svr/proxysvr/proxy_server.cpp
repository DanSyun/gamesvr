#include "jemalloc.h"

#include "proxy_server.h"
#include "daemon.hpp"

int run_flag = 0;
enum ENRunFlag
{
    EN_Flag_Exit    = 1,
    EN_Flag_Reload  = 2,
};
void SignalExit(int sig)
{
    run_flag |= EN_Flag_Exit;
}
void SignalReload(int sig)
{
    run_flag |= EN_Flag_Reload;
}

void ProxyServer::ParseArgs(int argc, char* argv[], char* conf, bool& daemon)
{
    int opt;
    while ((opt = getopt(argc, argv, "dc:")) != -1)
    {
        switch (opt)
        {
            case 'd': daemon = true; break;
            case 'c': strcpy(conf, optarg); break;
            default: break;
        }
    }
}

bool ProxyServer::LoadConfig(const char* conf)
{
    if (!_conf.Init(conf))
        return false;

    if (_conf.GetLogicSvrNum() == 0)
        return false;

    return true;
}

bool ProxyServer::InitNetwork()
{
    // socket
    _lsfd = socket(AF_INET, SOCK_STREAM |SOCK_NONBLOCK, 0);
    if (_lsfd == -1) return false;

    // socket options
    int opt = 1;
    struct linger ling = {0, 0};
    socklen_t buf_size = 102400;
    if (setsockopt(_lsfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) return false;
    if (setsockopt(_lsfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) return false;
    if (setsockopt(_lsfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) == -1) return false;
    if (setsockopt(_lsfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) == -1) return false;
    if (setsockopt(_lsfd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) return false;

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_conf.port());
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(_lsfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) return false;

    // listen
    if (listen(_lsfd, 1024) == -1) return false;

    // socket pool
    if (!SocketPoolInit()) return false;
    stSvrSocket* psocket = SocketCreate();
    psocket->_type = LISTEN_SOCKET;

    // epoll
    if (!EpollInit()) return false;
    if (!EpollAdd(_lsfd, psocket->_socket_pos)) return false;

    return true;
}

bool ProxyServer::SocketPoolInit()
{
    uint32 svr_num = _conf.GetLogicSvrNum() * 2;    // *2 for reconect use
    if (svr_num > _free_list.max_size()) return false;

    _pool = new stSvrSocket[svr_num];
    _all_recv_buf = new char[svr_num* RECV_BUF_LEN];
    for (uint16 pos = 0; pos < svr_num; ++pos)
    {
        _pool[pos]._fd = SOCKET_CLOSED;
        _pool[pos]._recv_buf = _all_recv_buf + pos* RECV_BUF_LEN;
        _pool[pos]._socket_pos = pos;
        _free_list.push_back(pos);
    }
    return true;
}

stSvrSocket* ProxyServer::SocketCreate()
{
    if (_free_list.empty())
        return NULL;

    uint16 pos = _free_list.front();
    _free_list.pop_front();

    return &_pool[pos];
}

void ProxyServer::SocketRelease(uint16 pos)
{
    stSvrSocket& socket = _pool[pos];

    if (socket._fd != SOCKET_CLOSED)
    {
        // close
        close(socket._fd);
        // del from ep
        EpollDel(socket._fd);
    }

    // clear
    memset(&socket, 0, sizeof(socket));
    socket._fd = SOCKET_CLOSED;
    socket._recv_buf = _all_recv_buf + pos* RECV_BUF_LEN;
    socket._socket_pos = pos;

    _free_list.push_back(pos);
}

void ProxyServer::SocketDisconn(uint16 pos)
{
    stSvrSocket& socket = _pool[pos];

    if (socket._fd != SOCKET_CLOSED)
    {
        // close
        close(socket._fd);
        // del from ep
        EpollDel(socket._fd);
    }

    socket._fd = SOCKET_CLOSED;
    socket._recv_offset = 0;
}

bool ProxyServer::EpollInit()
{
    memset(&_event, 0, sizeof(struct epoll_event));
    _event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    _event.data.ptr = NULL;
    _event.data.fd  = -1;

    uint32 svr_num = _conf.GetLogicSvrNum();
    _events = (struct epoll_event*)malloc(svr_num* sizeof(struct epoll_event));
    if ((_epfd = epoll_create(svr_num)) == -1)
        return false;

    return true;
}

int ProxyServer::Initialize(int argc, char* argv [])
{
    // set time
    UpdateTime();

    // args
    char conf[32] = {0};
    bool daemon = false;
    ParseArgs(argc, argv, conf, daemon);

    // only one process per config name can run at a time
    g_pinstance = new RunOneInstance((string(conf) + string(".lock")).c_str());
    if (!g_pinstance->IsOneInstance()) return -100;

    if (daemon) Daemonize();

    // load conf
    if (!LoadConfig(conf)) return -1;

    // net
    if (!InitNetwork()) return -2;

    // svr mng
    _pserver_mng = new ServerManager;
    _pserver_mng->Init(this);

    // signal
    signal(SIGRTMIN, SignalExit);
    signal(SIGRTMIN+1, SignalReload);

    return 0;
}

void ProxyServer::Run()
{
    while (true)
    {
        if (!CheckRunFlag())
            break;

        _pserver_mng->SendBufData();
        DealNetwork();
    }
}

bool ProxyServer::CheckRunFlag()
{
    if (run_flag & EN_Flag_Exit)
    {
        return false;
    }
    return true;
}

void ProxyServer::DealNetwork()
{
    int fd_num = epoll_wait(_epfd, _events, _conf.GetLogicSvrNum(), _conf.ep_timeout());
    if (fd_num == -1) return;

    struct epoll_event *pevent;
    for (int i = 0; i < fd_num; ++i)
    {
        pevent = &_events[i];
        uint16 pos = pevent->data.fd;
        stSvrSocket* psocket = SocketGet(pos);
        if (!psocket) continue;

        /*
        if ((EPOLLERR & events->events) != 0 || (EPOLLHUP & events->events) != 0)
        {
            SocketRelease(pos);
            continue;
        }
        if ((EPOLLIN & events->events) == 0) continue;
        */

        // accept conn
        if (psocket->_type == LISTEN_SOCKET)
        {
            AcceptConnect();
        }
        // recv data
        else
        {
            RecvServerData(*psocket);
        }
    }
}

void ProxyServer::AcceptConnect()
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int cli_fd = accept(_lsfd, (struct sockaddr*)&addr, &addr_len);
    if (cli_fd == -1)
    {
        return;
    }

    auto psocket = SocketCreate();
    if (!psocket)
    {
        close(cli_fd);
        return;
    }

    psocket->_fd = cli_fd;
    psocket->_type = CLIENT_SOCKET;

    // ??????????
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(cli_fd, &fds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 5000;  // 5ms
    int fd_num = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
    if (fd_num <= 0)
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
    stSvrInfo svr_info;
    int len = recv(cli_fd, (char*)&svr_info, sizeof(svr_info), 0);
    if (len != sizeof(svr_info))
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
    // ???????????p
    int flag;
    if ((flag = fcntl(cli_fd, F_GETFL, 0)) == -1 ||
        fcntl(cli_fd, F_SETFL, flag| O_NONBLOCK) == -1)
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
    if (!EpollAdd(cli_fd, psocket->_socket_pos))
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }

    // ???svr_info??????
    if (!_pserver_mng->Register(*psocket, svr_info))
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
}

void ProxyServer::RecvServerData(stSvrSocket& socket)
{
    int recv_len;
    do
    {
        if (RECV_BUF_LEN == socket._recv_offset)
            return;

        recv_len = recv(socket._fd, socket._recv_buf + socket._recv_offset, RECV_BUF_LEN - socket._recv_offset, 0);
        if (recv_len == 0)
        {
            SocketDisconn(socket._socket_pos);
            return;
        }
        if (recv_len == -1)
        {
            if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
                SocketDisconn(socket._socket_pos);
            return;
        }

        socket._recv_offset += recv_len;
        uint32 left = socket._recv_offset;
        char* tmp_recv = socket._recv_buf;
        while (left > 4)
        {
            uint32 len = *(int*)tmp_recv;
            if (len > left) break; // left not enough

            _pserver_mng->DoTransfer(tmp_recv, len);

            left -= len;
            tmp_recv += len;
        }
        if (left != socket._recv_offset)
        {
            if (left != 0)
                memmove(socket._recv_buf, tmp_recv, left);
            socket._recv_offset = left;
        }
    } while (recv_len > 0);
}


