#pragma once

#include "type_def.h"


// client 2 connect:    4len + pb
// connect 2 logic:     4len + stConnectHead + pb
// logic 2 proxy:       4len + stProxyHead + pb


#define CONNECT_HEAD_LEN    (sizeof(stConnectHead))
#define PROXY_HEAD_LEN      (sizeof(stProxyHead))

#define MAX_CS_WS_HEAD_LEN  (8)
#define MAX_SC_WS_HEAD_LEN  (4)
#define MAX_CS_PKG_BUF_LEN  (65536)
#define MAX_SS_PKG_BUF_LEN  (65536)
#define MAX_KVDB_BUF_LEN    (32768)
#define MAX_EVENT_BUF_LEN   (8192)
#define MAX_CS_PB_BUF_LEN   (MAX_CS_PKG_BUF_LEN - sizeof(uint32))
#define MAX_SS_PB_BUF_LEN   (MAX_SS_PKG_BUF_LEN - sizeof(uint32))
#define MAX_PB_BUF_LEN      (MAX_CS_PB_BUF_LEN > MAX_SS_PB_BUF_LEN ? MAX_CS_PB_BUF_LEN : MAX_SS_PB_BUF_LEN)
#define MAX_CONNECT_BUF_LEN (MAX_CS_PKG_BUF_LEN + CONNECT_HEAD_LEN)
#define MAX_PROXY_BUF_LEN   (MAX_SS_PKG_BUF_LEN + PROXY_HEAD_LEN)

/////////////////////////////////////////////////////////////////////////
// connect & logic 协议头
enum ConnectMsgType
{
    conn_msg_normal         = 1,    // 普通CS消息
    conn_msg_logic_index    = 2,    // logic通知conn建立索引
    conn_msg_client_discon  = 3,    // conn通知logic断开连接
};
enum ConnectType
{
    conn_type_tcp           = 1,    // tcp 连接服
    conn_type_ws            = 2,    // websocket 连接服
};
struct stConnectHead
{
    uint64  _create_time;
    uint32  _socket_pos;
    uint32  _socket_port;
    char    _socket_addr[16];
    bool    _need_close;
    ConnectMsgType  _msg_type;
    ConnectType     _conn_type;
};

/////////////////////////////////////////////////////////////////////////
// logic & proxy 协议头
struct stSvrInfo
{
    uint32  _type;
    uint32  _id;
};
enum ProxyRouteType
{
    proxy_route_p2p           = 1,    // 点对点传输
    proxy_route_hash          = 2,    // 根据给定key散列
    proxy_route_random        = 3,    // 随机
    proxy_route_broadcast     = 4,    // 广播
};
struct stProxyHead
{
    stSvrInfo   _src_svr;
    stSvrInfo   _des_svr;
    uint32      _session_id;
    uint64      _hash_key;
    uint32      _src_proxy_id; // 原路返回
    ProxyRouteType   _route_type;
};

/////////////////////////////////////////////////////////////////////////
// 逻辑服消息结构
union uMessageHead
{
    stConnectHead   _connect_head;
    stProxyHead     _proxy_head;
};
enum MessageType
{
    msg_type_client     = 1,
    msg_type_server     = 2,
    msg_type_internal   = 3,
};
struct stMessage
{
    MessageType     _type;
    char            _buf[MAX_PB_BUF_LEN];
    uint32          _buf_len;
    uMessageHead    _head;
};

