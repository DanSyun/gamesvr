#pragma once

#include "object_manager.hpp"
#include "singleton.hpp"
#include "base.h"
#include "proto_base.h"

class Session: public PBSession
{
private:
    typedef PBSession base;
public:
    size_t GetByteSize();
    bool Serialize(uint8* buf, size_t size, size_t& offset);
    bool UnSerialize(uint8* buf, size_t size, size_t& offset);

    void AddQueryKey(uint64 uid, uint32 key);
    bool HaveAllDBData();
    int GetDBProcessIndex(uint64 uid);
    PBEvent& AddEvent();
    void ClearEvents();

    stConnectHead   & GetConnectHead()      { return _msg_head._connect_head; }
    stProxyHead     & GetProxyHead()        { return _msg_head._proxy_head; }
    PBCSMsg         & GetCSMsg()            { return *base::mutable_cs_msg(); }
    PBCSMsg         & GetCSResponse()       { return *base::mutable_cs_response(); }
    PBSSMsg         & GetSSMsg()            { return *base::mutable_ss_msg(); }
    PBSSMsg         & GetSSResponse()       { return *base::mutable_ss_response(); }
    PBSSMsg         & GetWebMsg()           { return *base::mutable_web_msg(); }
    PBInternalMsg   & GetInternalMsg()      { return *base::mutable_inter_msg(); }

    PBDBProcess     & GetDBProcessInfo()        { return *base::mutable_db_processs(base::process_index()); }
    PBDBData        & GetDBData()               { return *base::mutable_db_processs(base::process_index())->mutable_player(); }
    PBDBData        & GetDBData(uint32 index)   { return *base::mutable_db_processs(index)->mutable_player(); }

    void            SetConnectHead(const stConnectHead& head)   { _msg_head._connect_head = head; }
    void            SetProxyHead(const stProxyHead& head)       { _msg_head._proxy_head = head; }
    void            SetCSMsg(const PBCSMsg& msg)                { base::mutable_cs_msg()->CopyFrom(msg); }
    void            SetSSMsg(const PBSSMsg& msg)                { base::mutable_ss_msg()->CopyFrom(msg); }
    void            SetSSResponse(const PBSSMsg& msg)           { base::mutable_ss_response()->CopyFrom(msg); }
    void            SetWebMsg(const PBSSMsg& msg)               { base::mutable_web_msg()->CopyFrom(msg); }
    void            SetInternalMsg(const PBInternalMsg& msg)    { base::mutable_inter_msg()->CopyFrom(msg); }
private:
    // 非pb对象也要序列化
    uMessageHead _msg_head;
};

class SessionManager:
    public ObjectManager<Session>,
    public Singleton<SessionManager>
{
public:
    typedef ObjectManager<Session> base;
    typedef std::unordered_map<uint32, uint32> index;
public:
    bool Init(uint32 size, uint32 shmkey = 0);

    Session* CreateSession(uint32 msg_id);
    Session* GetSession(uint32 session_id);
    bool ReleaseSession(uint32 session_id);
    void CheckTimeOut();

private:
    index   _index;
    uint32  _cur_max_id;
};


