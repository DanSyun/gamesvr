#pragma once

#include "base.h"
#include "proto_base.h"
#include "frame_server.h"
#include "session_manager.h"
#include "timer_manager.h"

//class FrameServer;
class MessageBase
{
protected:
    static FrameServer* _pserver;
public:
    static void Init(FrameServer* pserver)      { _pserver = pserver; }
    static void SigHandlerSEGV(int sig)         { _pserver->Exit(); signal(SIGSEGV, SIG_DFL); }
    static uint32 GetSvrType()                  { return _pserver->GetSvrType(); }
    static uint32 GetSvrID()                    { return _pserver->GetSvrID(); }
    static const LogicSvrConf& GetSvrConf()     { return _pserver->GetSvrConf(); }

    // proto 解析
    static bool ParseMsgFromClient(const char* buf, uint32 buf_len, PBCSMsg& cs_msg);
    static bool ParseMsgFromServer(const char* buf, uint32 buf_len, PBSSMsg& ss_msg);
    static bool ParseMsgFromInternal(const char* buf, uint32 buf_len, PBInternalMsg& internal_msg);

    // 发包函数
    static void SendCommonErr(stConnectHead& head, ENResult result);
    static bool SendMsgToClient(Session* psession, bool close = false);
    static bool SendMsgToClient(stConnectHead& head, PBCSMsg& cs_msg, bool close = false);
    static bool SendMsgToWeb(Session* psession, bool close = false);
    static bool SendMsgToWeb(stConnectHead& head, PBSSMsg& ss_msg, bool close = false);
    static bool SendMsgToResponse(Session* psession);
    static bool SendMsgToResponse(stProxyHead& src_head, PBSSMsg& ss_msg);
    static bool SendMsgToInternal(const PBInternalMsg& internal_msg);
    static bool SendMsgToEvent(const PBEvent& event);
    static bool SendMsgToDBProxy(Session* psession, PBSSMsg& ss_msg);
    static bool SendMsgToPromoter(Session* psession, PBSSMsg& ss_msg);
    static bool SendMsgToServer(Session* psession, PBSSMsg& ss_msg, ENSvrType svr_type, uint32 svr_id);
    static bool BroadcastToServer(PBSSMsg& ss_msg, ENSvrType type);

    // 定时器接口
    static int SetTimer(uint32 sec, const PBInternalMsg& inter);
    static int SetTimerMsec(uint32 msec, const PBInternalMsg& inter);
    static void ClearTimer(int timer_id);

    // db process 相关接口
    static bool SendQueryToDBProxy(Session* psession);
    static bool SendSaveToDBProxy(Session* psession);
    static bool ParseDBData(PBDBData& player, const google::protobuf::RepeatedPtrField<PBKVDBData>& datas);
    static void InitDBData(uint32 key, PBDBData& data);
};


