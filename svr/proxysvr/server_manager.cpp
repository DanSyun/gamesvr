#include "server_manager.h"

void ServerManager::Init(ProxyServer * pserver)
{
    _pserver = pserver;
    auto& conf = _pserver->GetSvrConf();
    for (int32 i = 0; i < conf.svrs_size(); ++i)
    {
        _servers[conf.svrs(i).svr_type()].resize(conf.svrs(i).svr_num(), NULL);
    }
}

bool ServerManager::Register(stSvrSocket& socket, const stSvrInfo& svr_info)
{
    if (_servers.find(svr_info._type) == _servers.end())
    {
        return false;
    }
    if (svr_info._id >= _servers[svr_info._type].size())
    {
        return false;
    }

    // 如果是重连需要拷贝缓存，销毁之前socket
    stSvrSocket* pbefore = _servers[svr_info._type][svr_info._id];
    if (pbefore)
    {
        socket._send_offset = pbefore->_send_offset;
        memcpy(&socket._send_buf, &pbefore->_send_buf, pbefore->_send_offset);
        SendBufData(socket);

        _pserver->SocketRelease(pbefore->_socket_pos);
    }
    _servers[svr_info._type][svr_info._id] = &socket;

    return true;
}

void ServerManager::DoTransfer(const char* buf, uint32 len)
{
    if (len <= sizeof(stProxyHead)) return;    // 

    stProxyHead* phead = (stProxyHead*)(buf + 4);
    phead->_src_proxy_id = _pserver->GetSvrConf().svr_id();
    if (_servers.find(phead->_des_svr._type) == _servers.end()) return;

    switch (phead->_route_type)
    {
        case proxy_route_p2p:
        {
            if (_servers[phead->_des_svr._type][phead->_des_svr._id] == NULL) return;
            SendOneData(*_servers[phead->_des_svr._type][phead->_des_svr._id], buf, len);
            break;
        }
        case proxy_route_broadcast:
        {
            const auto psvr = _pserver->GetSvrConf().GetLogicSvr(phead->_des_svr._type);
            if (psvr == NULL) return;
            for (uint32 i = 0; i < psvr->svr_num(); ++i)
            {
                if (_servers[phead->_des_svr._type][i] == NULL) continue;
                SendOneData(*_servers[phead->_des_svr._type][i], buf, len);
            }
            break;
        }
        case proxy_route_hash:
        {
            const auto psvr = _pserver->GetSvrConf().GetLogicSvr(phead->_des_svr._type);
            if (psvr == NULL) return;
            uint32 hash_id = phead->_hash_key % psvr->svr_num();
            if (_servers[phead->_des_svr._type][hash_id] == NULL) return;
            SendOneData(*_servers[phead->_des_svr._type][hash_id], buf, len);
            break;
        }
        default: break;
    }
}

void ServerManager::ReserveData(stSvrSocket& socket, const char* buf, uint32 len)
{
    // 如果满了就丢掉防止雪崩
    // 或者动态扩展缓存
    if (socket._send_offset + len > sizeof(socket._send_buf))
        socket._send_offset = 0;

    memcpy(&socket._send_buf[socket._send_offset], buf, len);
    socket._send_offset += len;
}

void ServerManager::SendBufData()
{
    for (auto map_iter = _servers.begin(); map_iter != _servers.end(); ++map_iter)
    {
        const auto& vec = map_iter->second;
        for (auto vec_iter = vec.begin(); vec_iter != vec.end(); ++vec_iter)
        {
            if (*vec_iter == NULL) continue;
            if ((*vec_iter)->_send_offset == 0) continue;
            SendBufData(**vec_iter);
        }
    }
}

bool ServerManager::SendBufData(stSvrSocket& socket)
{
    int ret = send(socket._fd, socket._send_buf, socket._send_offset, 0);
    if (ret == -1)
    {
        if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
        {
            _pserver->SocketDisconn(socket._socket_pos);
        }
        return false;
    }
    else
    {
        socket._send_offset = 0;
        return true;
    }
}

bool ServerManager::SendOneData(stSvrSocket& socket, const char* buf, uint32 len)
{
    // 如果已断开直接缓存等待重连
    if (socket._fd == SOCKET_CLOSED)
    {
        ReserveData(socket, buf, len);
        return false;
    }

    // 先发送缓存
    if (socket._send_offset != 0)
    {
        if (!SendBufData(socket))
        {
            // 发送失败则直接缓存本次数据
            ReserveData(socket, buf, len);
            return false;
        }
    }

    // 再发送本次数据
    int ret = send(socket._fd, buf, len, 0);
    if (ret == -1)
    {
        if (!(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
        {
            _pserver->SocketDisconn(socket._socket_pos);
        }
        ReserveData(socket, buf, len);
        return false;
    }

    return true;
}

