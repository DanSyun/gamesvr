#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include "jemalloc.h"

#include "frame_server.h"
#include "daemon.hpp"

static int run_flag = 0;
enum ENRunFlag
{
    EN_Flag_Exit    = 1,
    EN_Flag_Reload  = 2,
    // xx = 4
};
void SignalExit(int sig)
{
    run_flag |= EN_Flag_Exit;
}
void SignalReload(int sig)
{
    run_flag |= EN_Flag_Reload;
}


void FrameServer::ParseArgs(int argc, char* argv[], char* conf, bool& daemon)
{
    int opt;
    while ((opt = getopt(argc, argv, "dc:i:")) != -1)
    {
        switch (opt)
        {
            case 'd': daemon = true; break;
            case 'c': strcpy(conf, optarg); break;
            default: break;
        }
    }
}

bool FrameServer::LoadConfig(const char* conf)
{
    if (!_conf.Init(conf)) return false;

    _svr_info._type = _conf.svr_type();
    _svr_info._id = _conf.svr_id();

    return true;
}

bool FrameServer::InitMsgShmPipe()
{
    if (_conf.msg_shm_key() == 0)
        return false;

    size_t size = sizeof(ShmPipe) + _conf.msg_shm_size()* sizeof(stMessage) + 1;
    bool exsit;

    // create or attach shm
    if ((exsit = ShareMem::IsExist(_conf.msg_shm_key())) == false)
        ShareMem::Create(_conf.msg_shm_key(), size);
    _pmsg_shm = new ShareMem(_conf.msg_shm_key());

    if (!_pmsg_shm->IsAttached())
    {
        ShareMem::Delete(_conf.msg_shm_key());
        return false;
    }

    // create pipe
    ShmPipe::pshm = _pmsg_shm;
    _pmsg_pipe = new ShmPipe(exsit);

    if (!_pmsg_pipe)
    {
        _pmsg_shm->Detach();
        ShareMem::Delete(_conf.msg_shm_key());
        return false;
    }

    return true;
}

bool FrameServer::InitCsShmPipe()
{
    if (_conf.cs_shm_key() == 0 || _conf.sc_shm_key() == 0) return true;

    size_t cs_size = sizeof(ShmPipe) + _conf.cs_shm_size()* MAX_CONNECT_BUF_LEN + 1;
    size_t sc_size = sizeof(ShmPipe) + _conf.sc_shm_size()* MAX_CONNECT_BUF_LEN + 1;
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

bool FrameServer::InitCsShmPipeWs()
{
    if (_conf.cs_shm_key_ws() == 0 || _conf.sc_shm_key_ws() == 0) return true;

    size_t cs_size = sizeof(ShmPipe) + _conf.cs_shm_size_ws()* MAX_CONNECT_BUF_LEN + 1;
    size_t sc_size = sizeof(ShmPipe) + _conf.sc_shm_size_ws()* MAX_CONNECT_BUF_LEN + 1;
    bool cs_exist, sc_exist;

    // create or attach shm
    if ((cs_exist = ShareMem::IsExist(_conf.cs_shm_key_ws())) == false)
        ShareMem::Create(_conf.cs_shm_key_ws(), cs_size);
    _pcs_shm_ws = new ShareMem(_conf.cs_shm_key_ws());

    if ((sc_exist = ShareMem::IsExist(_conf.sc_shm_key_ws())) == false)
        ShareMem::Create(_conf.sc_shm_key_ws(), sc_size);
    _psc_shm_ws = new ShareMem(_conf.sc_shm_key_ws());

    if (!_pcs_shm_ws->IsAttached() || !_psc_shm_ws->IsAttached())
    {
        _pcs_shm_ws->Detach();
        _psc_shm_ws->Detach();
        ShareMem::Delete(_conf.cs_shm_key_ws());
        ShareMem::Delete(_conf.sc_shm_key_ws());
        return false;
    }

    // create pipe
    ShmPipe::pshm = _pcs_shm_ws;
    _pcs_pipe_ws = new ShmPipe(cs_exist);
    ShmPipe::pshm = _psc_shm_ws;
    _psc_pipe_ws = new ShmPipe(sc_exist);

    if (!_pcs_pipe_ws || !_psc_pipe_ws)
    {
        _pcs_shm_ws->Detach();
        _psc_shm_ws->Detach();
        ShareMem::Delete(_conf.cs_shm_key());
        ShareMem::Delete(_conf.sc_shm_key());
        return false;
    }

    return true;
}

bool FrameServer::InitProxy()
{
    _pproxy_manager = new ProxyManager<FrameServer>(this);
    return _pproxy_manager->Init();
}

bool FrameServer::InitEvent()
{
    _pevent_manager = new EventManager<FrameServer>(this);
    return _pevent_manager->Init();
}

int FrameServer::Initialize(int argc, char* argv[])
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

    // logger
    char log_name[64] = {0};
    snprintf(log_name, sizeof(log_name), "../../../log/%s_%u_%u", argv[0], _conf.svr_id(), getpid());
    InitRunLogger(log_name);
    memset(log_name, 0, sizeof(log_name));
    snprintf(log_name, sizeof(log_name), "../../../log/%s_%u.proto", argv[0], _conf.svr_id());
    InitProtoLogger(log_name);

    // msg shm pipe
    if (!InitMsgShmPipe()) return -2;

    // init cs/sc shm pipe
    if (!InitCsShmPipe()) return -3;
    if (!InitCsShmPipeWs()) return -3;

    // connect proxy
    if (!InitProxy()) return -4;

    // event writer
    if (!InitEvent()) return -5;
    
    // logic init
    int ret;
    if ((ret = InitGame()) != 0) return ret;

    // signal
    signal(SIGRTMIN, SignalExit);
    signal(SIGRTMIN+1, SignalReload);

    return 0;
}

void FrameServer::Run()
{
    while (true)
    {
        UpdateTime();           // update time
        OnRoutineCheck();       // logic routine
        GetClientMessage();     // 获取cspipe消息
        GetClientMessageWs();   // 获取cspipe消息
        CheckProxyConnect();    // 检查proxy连接
        SendProxyBuf();         // 发送proxy缓存消息
        GetProxyMessage();      // 获取proxy消息
        DispatchMessage();      // 分派消息

        if (!CheckRunFlag())
            break;
    }
}

void FrameServer::Exit()
{
    if (_pcs_shm)       _pcs_shm->Detach();
    if (_psc_shm)       _psc_shm->Detach();
    if (_pcs_shm_ws)    _pcs_shm_ws->Detach();
    if (_psc_shm_ws)    _psc_shm_ws->Detach();
    ExitGame();
}

bool FrameServer::CheckRunFlag()
{
    if (run_flag & EN_Flag_Exit)
    {
        Exit();
        return false;
    }
    if (run_flag & EN_Flag_Reload)
    {
        Reload();
        run_flag &= ~EN_Flag_Reload;
    }
    return true;
}

void FrameServer::CheckProxyConnect()
{
    // 每秒一次check
    static uint32 check_time = GetTime();
    if (check_time + 1 <= GetTime())
    {
        check_time = GetTime();
        _pproxy_manager->CheckConnect();
    }
}

void FrameServer::GetClientMessage()
{
    if (!_pcs_pipe) return;

    for (uint32 i = 0; i < _conf.msg_shm_size() / 2; ++i)
    {
        _cs_pipe_buf_len = MAX_CONNECT_BUF_LEN;
        if (!_pcs_pipe->GetOneData(&_cs_pipe_buf[0], _cs_pipe_buf_len)) return;  //cs empty

        if (_cs_pipe_buf_len <= sizeof(stConnectHead) + 4) continue;
        _cs_pipe_buf_len -= CONNECT_HEAD_LEN;
        stConnectHead* phead = (stConnectHead*)_cs_pipe_buf;
        if (phead->_msg_type == conn_msg_client_discon)
        {
            if (_cs_pipe_buf_len != sizeof(uint64)) continue;

            uint64 index = *(uint64*)(_cs_pipe_buf + CONNECT_HEAD_LEN);
            OnClientOffline(*phead, index);
        }
        else
        {
            if (_cs_pipe_buf_len <= sizeof(uint32)) continue;
            uint32 pb_len = ntohl(*(uint32*)(_cs_pipe_buf + CONNECT_HEAD_LEN));
            if (pb_len != _cs_pipe_buf_len) continue;

            if (!PostMessage(*phead, _cs_pipe_buf + CONNECT_HEAD_LEN + sizeof(uint32), _cs_pipe_buf_len - sizeof(uint32)))
                return;
        }
    }
}

void FrameServer::GetClientMessageWs()
{
    if (!_pcs_pipe_ws) return;

    for (uint32 i = 0; i < _conf.msg_shm_size() / 2; ++i)
    {
        _cs_pipe_buf_len = MAX_CONNECT_BUF_LEN;
        if (!_pcs_pipe_ws->GetOneData(&_cs_pipe_buf[0], _cs_pipe_buf_len)) return;  //cs empty

        if (_cs_pipe_buf_len <= sizeof(stConnectHead) + 4) continue;
        _cs_pipe_buf_len -= CONNECT_HEAD_LEN;
        stConnectHead* phead = (stConnectHead*)_cs_pipe_buf;
        if (phead->_msg_type == conn_msg_client_discon)
        {
            if (_cs_pipe_buf_len != sizeof(uint64)) continue;

            uint64 index = *(uint64*)(_cs_pipe_buf + CONNECT_HEAD_LEN);
            OnClientOffline(*phead, index);
        }
        else
        {
            if (_cs_pipe_buf_len <= sizeof(uint32)) continue;
            uint32 pb_len = ntohl(*(uint32*)(_cs_pipe_buf + CONNECT_HEAD_LEN));
            if (pb_len != _cs_pipe_buf_len) continue;

            if (!PostMessage(*phead, _cs_pipe_buf + CONNECT_HEAD_LEN + sizeof(uint32), _cs_pipe_buf_len - sizeof(uint32)))
                return;
        }
    }
}

void FrameServer::SendProxyBuf()
{
    _pproxy_manager->SendProxyBuf();
}

void FrameServer::GetProxyMessage()
{
    _pproxy_manager->GetProxyData();
}

bool FrameServer::PostMessage(const stConnectHead& head, char* buf, uint32 buf_len)
{
    _message._type = msg_type_client;
    _message._head._connect_head = head;
    _message._buf_len = buf_len;
    memcpy(&_message._buf, buf, buf_len);

    return _pmsg_pipe->AppendOneData((const char*)&_message, sizeof(_message));
}

bool FrameServer::PostMessage(const stProxyHead& head, char* buf, uint32 buf_len)
{
    _message._type = msg_type_server;
    _message._head._proxy_head = head;
    _message._buf_len = buf_len;
    memcpy(&_message._buf, buf, buf_len);

    return _pmsg_pipe->AppendOneData((const char*)&_message, sizeof(_message));
}

bool FrameServer::PostMessage(char* buf, uint32 buf_len)
{
    _message._type = msg_type_internal;
    _message._buf_len = buf_len;
    memcpy(&_message._buf, buf, buf_len);

    return _pmsg_pipe->AppendOneData((const char*)&_message, sizeof(_message));
}

void FrameServer::DispatchMessage()
{
    for (uint32 i = 0; i < _conf.msg_shm_size(); ++i)
    {
        _message_len = sizeof(_message);
        if (!_pmsg_pipe->GetOneData((char*)&_message, _message_len)) return;
        if (_message_len != sizeof(_message)) continue;

        if (_message._type == msg_type_client)
            OnClientMessage(_message._head._connect_head, _message._buf, _message._buf_len);

        else if (_message._type == msg_type_server)
            OnServerMessage(_message._head._proxy_head, _message._buf, _message._buf_len);

        else if (_message._type == msg_type_internal)
            OnInternalMessage(_message._buf, _message._buf_len);

        else;
            // unknown type
    }
}

bool FrameServer::SendMessageToInternal(char* buf, uint32 buf_len)
{
    return PostMessage(buf, buf_len);
}

bool FrameServer::SendMessageToProxy(const stProxyHead& head, char* buf, uint32 buf_len, int des_proxy_id)
{
    return _pproxy_manager->SendProxyData(head, buf, buf_len, des_proxy_id);
}

bool FrameServer::SendMessageToEvent(char* buf, uint32 buf_len)
{
    return _pevent_manager->SendEvent(buf, buf_len);
}

bool FrameServer::SendMessageToClient(const stConnectHead& head, char* buf, uint32 buf_len)
{
    if (head._conn_type == conn_type_tcp)
    {
        if (!_psc_pipe) return false;

        _cs_pipe_buf_len = 0;
        // tcp 头
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &head, sizeof(head));
        _cs_pipe_buf_len += sizeof(head);

        // 包长
        uint32 total_len = htonl(buf_len + sizeof(uint32));
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &total_len, sizeof(total_len));
        _cs_pipe_buf_len += sizeof(total_len);

        // 数据
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], buf, buf_len);
        _cs_pipe_buf_len += buf_len;

        return _psc_pipe->AppendOneData(_cs_pipe_buf, _cs_pipe_buf_len);
    }
    else
    {
        if (!_psc_pipe_ws) return false;

        _cs_pipe_buf_len = 0;
        // tcp 头
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &head, sizeof(head));
        _cs_pipe_buf_len += sizeof(head);

        // 包长
        uint32 total_len = htonl(buf_len + sizeof(uint32));
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &total_len, sizeof(total_len));
        _cs_pipe_buf_len += sizeof(total_len);

        // 数据
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], buf, buf_len);
        _cs_pipe_buf_len += buf_len;

        return _psc_pipe_ws->AppendOneData(_cs_pipe_buf, _cs_pipe_buf_len);
    }
}

bool FrameServer::NotifyConnectLogicIndex(const stConnectHead& head, uint64 logic_index)
{
    if (head._conn_type == conn_type_tcp)
    {
        if (!_psc_pipe) return false;

        _cs_pipe_buf_len = 0;
        uint32 len = sizeof(logic_index);

        // tcp 头
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &head, sizeof(head));
        stConnectHead* phead = (stConnectHead*)_cs_pipe_buf;
        phead->_msg_type = conn_msg_logic_index;
        _cs_pipe_buf_len += sizeof(head);

        // 数据
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &logic_index, len);
        _cs_pipe_buf_len += len;

        return _psc_pipe->AppendOneData(_cs_pipe_buf, _cs_pipe_buf_len);
    }
    else if (head._conn_type == conn_type_ws)
    {
        if (!_psc_pipe_ws) return false;

        _cs_pipe_buf_len = 0;
        uint32 len = sizeof(logic_index);

        // tcp 头
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &head, sizeof(head));
        stConnectHead* phead = (stConnectHead*)_cs_pipe_buf;
        phead->_msg_type = conn_msg_logic_index;
        _cs_pipe_buf_len += sizeof(head);

        // 数据
        memcpy(&_cs_pipe_buf[_cs_pipe_buf_len], &logic_index, len);
        _cs_pipe_buf_len += len;

        return _psc_pipe_ws->AppendOneData(_cs_pipe_buf, _cs_pipe_buf_len);
    }
    else
    {
        return false;
    }
}


