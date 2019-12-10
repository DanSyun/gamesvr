#pragma once

#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>

#include "base.h"
#include "shm_pipe.h"
#include "share_mem.hpp"
#include "proxy_manager.h"
#include "event_manager.h"


class FrameServer
{
    friend class ProxyManager<FrameServer>;
    friend class EventManager<FrameServer>;
public:
    FrameServer(): _pcs_shm(NULL),      _pcs_pipe(NULL),    _psc_shm(NULL),     _psc_pipe(NULL),
                   _pcs_shm_ws(NULL),   _pcs_pipe_ws(NULL), _psc_shm_ws(NULL),  _psc_pipe_ws(NULL),
                   _login_key("1XcsBhAjreWlresV")
                   {}
private:
    void ParseArgs(int argc, char* argv[], char* conf, bool& daemon);
    bool LoadConfig(const char* conf);
    bool InitMsgShmPipe();
    bool InitCsShmPipe();
    bool InitCsShmPipeWs();
    bool InitProxy();
    bool InitEvent();

    bool CheckRunFlag();
    void GetClientMessage();
    void GetClientMessageWs();
    void CheckProxyConnect();
    void SendProxyBuf();
    void GetProxyMessage();
    bool PostMessage(const stConnectHead& head, char* buf, uint32 buf_len);
    bool PostMessage(const stProxyHead& head, char* buf, uint32 buf_len);
    bool PostMessage(char* buf, uint32 buf_len);
    void DispatchMessage();

private:
    virtual int InitGame() = 0;
    virtual int Reload() = 0;
    virtual void ExitGame() = 0;
    virtual void OnClientMessage(stConnectHead& head, const char* buf, uint32 buf_len) = 0;
    virtual void OnClientOffline(const stConnectHead& head, uint64 logic_index) = 0;
    virtual void OnServerMessage(stProxyHead& head, char* buf, uint32 buf_len) = 0;
    virtual void OnInternalMessage(char* buf, uint32 buf_len) = 0;
    virtual void OnRoutineCheck() = 0;

public:
    int Initialize(int argc, char* argv[]);
    void Run();
    void Exit();
    const LogicSvrConf& GetSvrConf()    { return _conf; }
    const stSvrInfo& GetSvrInfo()       { return _svr_info; }
    uint32 GetSvrType()                 { return _svr_info._type; }
    uint32 GetSvrID()                   { return _svr_info._id; }
    const string& GetLoginKey()         { return _login_key; }

    bool SendMessageToInternal(char* buf, uint32 buf_len);
    bool SendMessageToProxy(const stProxyHead& head, char* buf, uint32 buf_len, int des_proxy_id = -1);
    bool SendMessageToEvent(char* buf, uint32 buf_len);
    bool SendMessageToClient(const stConnectHead& head, char* buf, uint32 buf_len);
    bool NotifyConnectLogicIndex(const stConnectHead& head, uint64 logic_index);

private:
    LogicSvrConf    _conf;
    stSvrInfo       _svr_info;

    ShareMem* _pmsg_shm;
    ShmPipe* _pmsg_pipe;

    ShareMem* _pcs_shm;
    ShmPipe* _pcs_pipe;
    ShareMem* _psc_shm;
    ShmPipe* _psc_pipe;

    ShareMem* _pcs_shm_ws;
    ShmPipe* _pcs_pipe_ws;
    ShareMem* _psc_shm_ws;
    ShmPipe* _psc_pipe_ws;

    ProxyManager<FrameServer>* _pproxy_manager;
    EventManager<FrameServer>* _pevent_manager;

    char _cs_pipe_buf[MAX_CONNECT_BUF_LEN];
    uint32 _cs_pipe_buf_len;

    stMessage _message;
    uint32 _message_len;

    string _login_key;
};

