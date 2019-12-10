#pragma once

#include "base.h"
#include "proxy_server.h"

struct stSvrSocket;
class ProxyServer;
class ServerManager
{
public:
    void Init(ProxyServer* pserver);
    bool Register(stSvrSocket& socket, const stSvrInfo& svr_info);
    void DoTransfer(const char* buf, uint32 len);
    void SendBufData();

private:
    void ReserveData(stSvrSocket& socket, const char* buf, uint32 len);
    bool SendBufData(stSvrSocket& socket);
    bool SendOneData(stSvrSocket& socket, const char* buf, uint32 len);
private:
    // key: svr_type, value: svrs(index: svr_id)
    std::unordered_map<uint32, std::vector<stSvrSocket*> >  _servers;
    ProxyServer* _pserver;
};

