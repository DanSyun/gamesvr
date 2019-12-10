#include "jemalloc.h"
#include "connectws_server.h"
#include "sha1.h"

#include "daemon.hpp"
#include "time.hpp"
#include "base64.hpp"
#include "ilogger.h"

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

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

void ConnectWsServer::ParseArgs(int argc, char* argv[], char* conf, bool& daemon)
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

bool ConnectWsServer::LoadConfig(const char* conf)
{
    if (!_conf.Init(conf))
        return false;

    if (_conf.max_client_num() == 0)
        return false;

    return true;
}

bool ConnectWsServer::InitNetwork()
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
    stWsCliSocket* psocket = SocketCreate();
    psocket->_type = LISTEN_SOCKET;

    // epoll
    if (!EpollInit()) return false;
    if (!EpollAdd(_lsfd, psocket->_socket_pos)) return false;

    return true;
}

bool ConnectWsServer::SocketPoolInit()
{
    uint32 client_num = _conf.max_client_num();
    if (client_num > _free_list.max_size()) return false;

    _pool = new stWsCliSocket[client_num];
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

stWsCliSocket* ConnectWsServer::SocketCreate()
{
    if (_free_list.empty())
        return NULL;

    uint16 pos = _free_list.front();
    _free_list.pop_front();

    return &_pool[pos];
}

void ConnectWsServer::SocketRelease(uint16 pos, bool need_notify_logic)
{
    stWsCliSocket& socket = _pool[pos];

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

bool ConnectWsServer::EpollInit()
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

bool ConnectWsServer::InitCsShmPipe()
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

int ConnectWsServer::Initialize(int argc, char* argv [])
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

void ConnectWsServer::Run()
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

bool ConnectWsServer::CheckRunFlag()
{
    if (run_flag & EN_Flag_Exit)
    {
        _pcs_shm->Detach();
        _psc_shm->Detach();
        return false;
    }
    return true;
}

void ConnectWsServer::OnRoutineCheck()
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
                const stWsCliSocket& client = _pool[pos];
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

void ConnectWsServer::DealNetwork()
{
    int fd_num = epoll_wait(_epfd, _events, _conf.max_client_num(), _conf.ep_timeout());
    if (fd_num == -1) return;

    struct epoll_event *pevent;
    for (int i = 0; i < fd_num; ++i)
    {
        pevent = &_events[i];
        int pos = pevent->data.fd;
        stWsCliSocket* psocket = SocketGet(pos);
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

void ConnectWsServer::AcceptConnect()
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int cli_fd = accept(_lsfd, (struct sockaddr*)&addr, &addr_len);
    if (cli_fd == -1)
    {
        return;
    }

    stWsCliSocket* psocket = SocketCreate();
    if (!psocket)
    {
        close(cli_fd);
        return;
    }

    uint32 sec = 0, usec = 0;
    GetSecAndUsec(sec, usec);
    psocket->_fd = cli_fd;
    psocket->_type = CLIENT_SOCKET;
    psocket->_create_time = UpdateTime();
    psocket->_hand_shaked = false;
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

void ConnectWsServer::RecvClientData(stWsCliSocket& socket)
{
    int recv_len;
    do
    {
        if ((RECV_BUF_LEN - CONNECT_HEAD_LEN) == socket._buf_offset)
            return;

        recv_len = recv(socket._fd, socket._recv_buf + socket._buf_offset,
            RECV_BUF_LEN - CONNECT_HEAD_LEN - socket._buf_offset, 0);
        if (recv_len == 0)
            return SocketRelease(socket._socket_pos);

        if (recv_len == -1)
        {
            if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
                SocketRelease(socket._socket_pos);
            return;
        }

        socket._buf_offset += recv_len;
        // http协议握手，只做简单检查
        if (socket._hand_shaked == false)
        {
            if (socket._buf_offset < strlen("GET"))
                continue;

            if (strncmp(socket._recv_buf, "GET", strlen("GET")) != 0)
                return SocketRelease(socket._socket_pos);


            if (strstr(socket._recv_buf, "\r\n\r\n") == NULL) 
                continue;

            if (strstr(socket._recv_buf, "Upgrade: websocket") == NULL)
                return SocketRelease(socket._socket_pos);

            char* key_start = NULL;
            if ((key_start = strstr(socket._recv_buf, "Sec-WebSocket-Key: ")) == NULL)
                return SocketRelease(socket._socket_pos);

            char* key = NULL;
            key = key_start + strlen("Sec-WebSocket-Key: ");
        	std::string server_key = string(key, 24);
        	server_key += MAGIC_KEY;
        	SHA1 sha;
        	unsigned int message_digest[5];
        	sha.Reset();
        	sha << server_key.c_str();
        	sha.Result(message_digest);
        	for (int i = 0; i < 5; i++) {
        		message_digest[i] = htonl(message_digest[i]);
        	}
        	server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
        	server_key += "\r\n";

            memset(_send_buf, 0, sizeof(_send_buf));
        	strcat(_send_buf, "HTTP/1.1 101 Switching Protocols\r\n");
        	strcat(_send_buf, "Connection: upgrade\r\n");
        	strcat(_send_buf, "Sec-WebSocket-Accept: ");
        	strcat(_send_buf, server_key.c_str());
        	strcat(_send_buf, "Upgrade: websocket\r\n\r\n");

            // block send
            uint32 count = 0;
            while (count++ < _conf.max_send_client_retry())
            {
                int len = send(socket._fd, _send_buf, strlen(_send_buf), 0);
                if (len == -1)
                {
                    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)  // try again
                        continue;
                    else    // failed
                        return SocketRelease(socket._socket_pos);
                }
                else    // succ
                {
                    socket._buf_offset = 0;
                    socket._hand_shaked = true;
                    return;
                }
            }
        }
        else
        {
            uint32 left = socket._buf_offset;
            char* tmp_recv = socket._recv_buf;
            char* tmp_pipe = socket._recv_buf - CONNECT_HEAD_LEN; // start from connect head
            while (left > 2)
            {
                uint32 pos = 0;
                // 第一个字节
                uint8 fin = (uint8)tmp_recv[pos] >> 7;
                if (fin != 1)   // 暂时不支持分片
                {
                    return SocketRelease(socket._socket_pos);
                }
                // uint8 opcode = tmp_recv[pos] & 0x0f;
                pos += 1;

                // 第二个字节
                uint8 mask = (uint8)tmp_recv[pos] >> 7;
                if (mask != 1)
                {
                    return SocketRelease(socket._socket_pos);
                }
                uint32 len = tmp_recv[pos] & 0x7f;
                pos += 1;

                // 长度
                if (len == 126)
                {
                    if (left - pos < 2) break; // left not enough

                    len = ntohl(*(uint16*)(tmp_recv + pos));
                    pos += 2;
                }
                else if (len == 127)    // len invalid
                {
                    return SocketRelease(socket._socket_pos);
                }
                else if (len < 4)
                {
                    return SocketRelease(socket._socket_pos);
                }

                // mask_key
                if (left - pos < 4) break;
                uint8 mask_key[4];
                for (uint32 i = 0; i < 4; ++i)
                {
                    mask_key[i] = tmp_recv[pos + i];
                }
                pos += 4;

                // 服务器端的协议
                if (left - pos < len) break;
                for (uint32 i = 0; i < len; ++i)
                {
                    int j = i % 4;
                    tmp_recv[pos + i] = tmp_recv[pos + i] ^ mask_key[j];
                }
                uint32 pb_len = ntohl((*(uint32*)(tmp_recv + pos)));
                if (pb_len != len || pb_len > MAX_CS_PKG_BUF_LEN)
                {
                    return SocketRelease(socket._socket_pos);
                }

                stConnectHead* phead = (stConnectHead*)(tmp_pipe + pos);
                phead->_socket_pos = socket._socket_pos;
                phead->_create_time = socket._create_time;
                phead->_socket_port = socket._socket_port;
                phead->_msg_type = conn_msg_normal;
                phead->_conn_type = conn_type_ws;
                strcpy(phead->_socket_addr, socket._socket_addr);

                if (!_pcs_pipe->AppendOneData(tmp_pipe + pos, len + CONNECT_HEAD_LEN)) break; // cs full
                pos += len;

                left -= pos;
                tmp_recv += pos;
                tmp_pipe += pos;
            }
            if (left != socket._buf_offset)
            {
                if (left != 0)
                    memmove(socket._recv_buf, tmp_recv, left);
                socket._buf_offset = left;
            }
        }
    } while (recv_len > 0);
}

void ConnectWsServer::SendClientData()
{
    for (uint32 i = 0; i < _conf.max_send_client_num(); ++i)
    {
        _send_len = MAX_CONNECT_BUF_LEN;
        if (!_psc_pipe->GetOneData(&_send_buf[0], _send_len)) break;  //sc err

        stConnectHead* phead = (stConnectHead*)_send_buf;
        stWsCliSocket* psocket = SocketGet(phead->_socket_pos);
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

        // wrap ws
        WrapWebsocketHead(_send_buf + CONNECT_HEAD_LEN, _send_len);

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

void ConnectWsServer::NotifyLogicDiscon(const stWsCliSocket& socket)
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

void ConnectWsServer::WrapWebsocketHead(char* buf, uint32& buf_len)
{
    if (buf_len <= 125)
    {
        memmove(buf + 2, buf, buf_len);
    }
    else
    {
        memmove(buf + 4, buf, buf_len);
    }

    buf[0] = 1;         // fin
    buf[0] <<= 7;
    buf[0] |= 0x02;     // opcode
    buf[1] = 0;         // mask
    buf[1] <<= 7;
    if (buf_len <= 125) // len
    {
        buf[1] |= (uint8)buf_len;
        buf_len += 2;
    }
    else
    {
        buf[1] |= (uint8)126;
        *(uint16*)(buf + 2) = htons((uint16)buf_len);
        buf_len += 4;
    }
}

