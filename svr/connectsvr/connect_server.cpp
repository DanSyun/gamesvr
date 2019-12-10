#include "jemalloc.h"
#include "connect_server.h"

#include "daemon.hpp"
#include "time.hpp"

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

void ConnectServer::ParseArgs(int argc, char* argv[], char* conf, bool& daemon)
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

bool ConnectServer::LoadConfig(const char* conf)
{
    if (!_conf.Init(conf))
        return false;

    if (_conf.max_client_num() == 0)
        return false;

    return true;
}

bool ConnectServer::InitNetwork()
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
    stCliSocket* psocket = SocketCreate();
    psocket->_type = LISTEN_SOCKET;

    // epoll
    if (!EpollInit()) return false;
    if (!EpollAdd(_lsfd, psocket->_socket_pos)) return false;

    return true;
}

bool ConnectServer::SocketPoolInit()
{
    uint32 client_num = _conf.max_client_num();
    if (client_num > _free_list.max_size()) return false;

    _pool = new stCliSocket[client_num];
    _all_recv_buf = new char[client_num* RECV_BUF_LEN];
    for (uint16 pos = 0; pos < client_num; ++pos)
    {
        _pool[pos]._recv_buf = _all_recv_buf + pos* RECV_BUF_LEN + CONNECT_HEAD_LEN;
        _pool[pos]._socket_pos = pos;
        _pool[pos]._type = NONE_SOCKET;
        _free_list.push_back(pos);
    }
    return true;
}

stCliSocket* ConnectServer::SocketCreate()
{
    if (_free_list.empty())
        return NULL;

    uint16 pos = _free_list.front();
    _free_list.pop_front();

    return &_pool[pos];
}

void ConnectServer::SocketRelease(uint16 pos, bool need_notify_logic)
{
    stCliSocket& socket = _pool[pos];

    if (socket._fd != SOCKET_CLOSED)
    {
        // close
        close(socket._fd);
        // del from ep
        EpollDel(socket._fd);
    }

    // notify logic client close
    if (need_notify_logic)
    {
        NotifyLogicDiscon(socket);
    }

    // clear
    memset(&socket, 0, sizeof(socket));
    socket._recv_buf = _all_recv_buf + pos* RECV_BUF_LEN + CONNECT_HEAD_LEN;
    socket._socket_pos = pos;

    _free_list.push_back(pos);
}

bool ConnectServer::EpollInit()
{
    memset(&_event, 0, sizeof(struct epoll_event));
    _event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    _event.data.ptr = NULL;
    _event.data.fd  = -1;

    uint32 client_num = _conf.max_client_num();
    _events = (struct epoll_event*)malloc(client_num* sizeof(struct epoll_event));
    if ((_epfd = epoll_create(client_num)) == -1)
        return false;

    return true;
}

bool ConnectServer::InitCsShmPipe()
{
    if (_conf.cs_shm_key() == 0 || _conf.sc_shm_key() == 0)
        return false;

    size_t cs_size = sizeof(ShmPipe) + _conf.cs_shm_size() * MAX_CONNECT_BUF_LEN + 1;
    size_t sc_size = sizeof(ShmPipe) + _conf.sc_shm_size() * MAX_CONNECT_BUF_LEN + 1;

    bool cs_exist, sc_exist;
    // create or attach shm
    if ((cs_exist = ShareMem::IsExist(_conf.cs_shm_key())) == false)
        ShareMem::Create(_conf.cs_shm_key(), cs_size);
    _pcs_shm = new ShareMem(_conf.cs_shm_key());

    if ((sc_exist = ShareMem::IsExist(_conf.sc_shm_key())) == false)
        ShareMem::Create(_conf.sc_shm_key(), sc_size);
    _psc_shm = new ShareMem(_conf.sc_shm_key());

    if (!_pcs_shm->IsAttached() || !_psc_shm->IsAttached())
    {
        _pcs_shm->Detach();
        _psc_shm->Detach();
        ShareMem::Delete(_conf.cs_shm_key());
        ShareMem::Delete(_conf.sc_shm_key());
        return false;
    }

    // create pipe
    ShmPipe::pshm = _pcs_shm;
    _pcs_pipe = new ShmPipe(cs_exist);
    ShmPipe::pshm = _psc_shm;
    _psc_pipe = new ShmPipe(sc_exist);

    if (!_pcs_pipe || !_psc_pipe)
    {
        _pcs_shm->Detach();
        _psc_shm->Detach();
        ShareMem::Delete(_conf.cs_shm_key());
        ShareMem::Delete(_conf.sc_shm_key());
        return false;
    }

    return true;
}

int ConnectServer::Initialize(int argc, char* argv [])
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

    // init cs/sc shm pipe
    if (!InitCsShmPipe()) return -3;

    // signal
    signal(SIGRTMIN, SignalExit);
    signal(SIGRTMIN+1, SignalReload);

    return 0;
}

void ConnectServer::Run()
{
    while (true)
    {
        if (!CheckRunFlag())
            break;

        UpdateTime();
        OnRoutineCheck();
        DealNetwork();
        SendClientData();
    }
}

bool ConnectServer::CheckRunFlag()
{
    if (run_flag & EN_Flag_Exit)
    {
        _pcs_shm->Detach();
        _psc_shm->Detach();
        return false;
    }
    return true;
}

void ConnectServer::OnRoutineCheck()
{
    // 未登录的连接timeout_sec后断开，已登录的连接有更长的容忍时间timeout_sec * 100
    if (_conf.heart_beat_check())
    {
        uint32 timeout_sec = 6;
        static uint32 heart_beat_check = GetTime();
        if (heart_beat_check + timeout_sec <= GetTime())
        {
            heart_beat_check = GetTime();
            for (uint16 pos = 0; pos < _conf.max_client_num(); ++pos)
            {
                const stCliSocket& client = _pool[pos];
                if (client._type != CLIENT_SOCKET) continue;
                if (client._logic_index == -1)
                {
                    if (client._create_time / 1000 + timeout_sec <= GetTime())
                        SocketRelease(pos, false);
                }
                else
                {
                    if (client._update_time + timeout_sec * 100 <= GetTime())
                        SocketRelease(pos);
                }
            }
        }
    }
}

void ConnectServer::DealNetwork()
{
    int fd_num = epoll_wait(_epfd, _events, _conf.max_client_num(), _conf.ep_timeout());
    if (fd_num == -1) return;

    struct epoll_event *pevent;
    for (int i = 0; i < fd_num; ++i)
    {
        pevent = &_events[i];
        int pos = pevent->data.fd;
        stCliSocket* psocket = SocketGet(pos);
        if (!psocket) continue;

        /*
        if ((EPOLLERR & pevent->events) != 0)
        {
            SocketRelease(pos);
            continue;
        }
        if ((EPOLLIN & pevent->events) == 0) continue;
        */

        // accept conn
        if (psocket->_type == LISTEN_SOCKET)
        {
            AcceptConnect();
        }
        // recv data
        else
        {
            RecvClientData(*psocket);
        }
    }
}

void ConnectServer::AcceptConnect()
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
    psocket->_create_time = UpdateTime();
    psocket->_logic_index = -1;
    psocket->_buf_offset = 0;
    psocket->_socket_port = ntohs(addr.sin_port);
    strcpy(psocket->_socket_addr, inet_ntoa(addr.sin_addr));

    int flag;
    if ((flag = fcntl(cli_fd, F_GETFL, 0)) == -1 || fcntl(cli_fd, F_SETFL, flag| O_NONBLOCK) == -1)
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
    if (!EpollAdd(cli_fd, psocket->_socket_pos))
    {
        SocketRelease(psocket->_socket_pos);
        return;
    }
}

void ConnectServer::RecvClientData(stCliSocket& socket)
{
    int recv_len;
    do
    {
        if ((RECV_BUF_LEN - CONNECT_HEAD_LEN) == socket._buf_offset)
            return;

        recv_len = recv(socket._fd, socket._recv_buf + socket._buf_offset,
            RECV_BUF_LEN - CONNECT_HEAD_LEN - socket._buf_offset, 0);
        if (recv_len == 0)
        {
            SocketRelease(socket._socket_pos);
            return;
        }
        if (recv_len == -1)
        {
            if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
                SocketRelease(socket._socket_pos);
            return;
        }

        socket._buf_offset += recv_len;
        uint32 left = socket._buf_offset;
        char* tmp_recv = socket._recv_buf;
        char* tmp_pipe = socket._recv_buf - CONNECT_HEAD_LEN; // start from connect head
        while (left >= 4)
        {
            uint32 len = ntohl(*(uint32*)tmp_recv);
            if (len > MAX_CS_PKG_BUF_LEN || len < 4)   // len invalid
            {
                SocketRelease(socket._socket_pos);
                return;
            }

            if (len > left) break; // left not enough

            stConnectHead* phead = (stConnectHead*)tmp_pipe;
            phead->_socket_pos = socket._socket_pos;
            phead->_create_time = socket._create_time;
            phead->_socket_port = socket._socket_port;
            phead->_msg_type = conn_msg_normal;
            phead->_conn_type = conn_type_tcp;
            strcpy(phead->_socket_addr, socket._socket_addr);

            if (!_pcs_pipe->AppendOneData(tmp_pipe, len + CONNECT_HEAD_LEN)) break; // cs full

            left -= len;
            tmp_recv += len;
            tmp_pipe += len;
        }
        if (left != socket._buf_offset)
        {
            if (left != 0)
                memmove(socket._recv_buf, tmp_recv, left);
            socket._buf_offset = left;
        }
    } while (recv_len > 0);
}

void ConnectServer::SendClientData()
{
    for (uint32 i = 0; i < _conf.max_send_client_num(); ++i)
    {
        _send_len = MAX_CONNECT_BUF_LEN;
        if (!_psc_pipe->GetOneData(&_send_buf[0], _send_len)) break;  //sc err

        stConnectHead* phead = (stConnectHead*)_send_buf;
        stCliSocket* psocket = SocketGet(phead->_socket_pos);
        if (!psocket) return;

        if (phead->_create_time != psocket->_create_time)
            return; // reuse socket

        psocket->_update_time = GetTime();

        // skip head
        _send_len -= CONNECT_HEAD_LEN;

        if (phead->_msg_type == conn_msg_logic_index)
        {
            if (_send_len == sizeof(uint64))
                psocket->_logic_index = *(uint64*)(_send_buf + CONNECT_HEAD_LEN);
            return;
        }

        // block send
        uint32 count = 0;
        while (count++ < _conf.max_send_client_retry())
        {
            int len = send(psocket->_fd, _send_buf + CONNECT_HEAD_LEN, _send_len, 0);
            if (len == -1)
            {
                if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)  // try again
                    continue;
                else    // failed
                    return SocketRelease(psocket->_socket_pos);
            }
            else    // succ
            {
                if (phead->_need_close) // close
                {
                    SocketRelease(psocket->_socket_pos, false);
                }
                return;
            }
        }
    }
}

void ConnectServer::NotifyLogicDiscon(const stCliSocket& socket)
{
    if (socket._logic_index != -1)
    {
        char* tmp_recv = socket._recv_buf;
        char* tmp_pipe = socket._recv_buf - CONNECT_HEAD_LEN; // start from connecthead

        stConnectHead* phead = (stConnectHead*)tmp_pipe;
        phead->_msg_type = conn_msg_client_discon;
        memcpy(tmp_recv, &socket._logic_index, sizeof(socket._logic_index));

        _pcs_pipe->AppendOneData(tmp_pipe, sizeof(socket._logic_index) + CONNECT_HEAD_LEN);
    }
}

