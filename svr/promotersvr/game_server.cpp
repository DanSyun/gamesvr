#include "game_server.h"

#include "message.h"
#include "message_manager.h"
#include "session_manager.h"
#include "timer_manager.h"
#include "config.h"
#include "message_handler_common.h"

#include "proto_log.hpp"

int GameServer::InitGame()
{
    ProtoLog::Instance()->Init();   // 获取pb日志
    MessageBase::Init(this);
    signal(SIGSEGV, MessageBase::SigHandlerSEGV);    // 捕获段错误

    // 管理器一定要在ExitGame释放
    // 消息会话管理器
    if (!SessionManager::Instance()->Init(GetSvrConf().max_session_num(), GetSvrConf().session_shm_key()))
        return -1;
    // 定时器管理器
    if (!TimerManager::Instance()->Init(GetSvrConf().max_timer_num(), GetSvrConf().timer_shm_key()))
        return -2;
    HandlerPromoterGold::Init();

    int ret;
    if ((ret = LoadConfig()) != 0)
        return ret;

    // 消息处理
    MessageManager::Instance()->Init();

    return 0;
}

int GameServer::LoadConfig()
{
    if (!ConfigGameAddr::Instance()->Init("../../../config/game_addr.cfg")) return -11;

    return 0;
}

int GameServer::Reload()
{
    return LoadConfig();
}

void GameServer::ExitGame()
{
    SessionManager::Instance()->Release();
    TimerManager::Instance()->Release();
    HandlerPromoterGold::Release();
}

void GameServer::OnClientMessage(stConnectHead& head, const char* buf, uint32 buf_len)
{
}

void GameServer::OnClientOffline(const stConnectHead& head, uint64 logic_index)
{
}

void GameServer::OnServerMessage(stProxyHead& head, char* buf, uint32 buf_len)
{
    PBSSMsg ss_msg;
    if (!MessageBase::ParseMsgFromServer(buf, buf_len, ss_msg))
        return;

    Session* psession = NULL;
    // 创建session
    if (ss_msg.type() == EN_SS_Request)
    {
        psession = SessionManager::Instance()->CreateSession(ss_msg.msg_union_case());
        if (!psession) return;

        psession->SetProxyHead(head);
        psession->SetSSMsg(ss_msg);
    }
    // 恢复session
    else
    {
        psession = SessionManager::Instance()->GetSession(head._session_id);
        if (!psession) return;

        psession->SetSSResponse(ss_msg);
    }

    MessageManager::Instance()->Process(psession);
}

void GameServer::OnInternalMessage(char* buf, uint32 buf_len)
{
    PBInternalMsg internal_msg;
    if (!MessageBase::ParseMsgFromInternal(buf, buf_len, internal_msg))
        return;

    Session* psession = SessionManager::Instance()->CreateSession(internal_msg.msg_union_case());
    if (!psession)
        return;

    psession->SetInternalMsg(internal_msg);

    MessageManager::Instance()->Process(psession);
}

void GameServer::OnRoutineCheck()
{
    auto now = GetTime();
    auto now_msec = GetTimeMsec();

    // 打印一些容器数量
    static uint32 print_check = 0;
    if (print_check + 300 <= now)
    {
        print_check = now;
        LOG_INFO("Session count: %d", SessionManager::Instance()->Size());
        LOG_INFO("Timer count: %d", TimerManager::Instance()->Size());
    }

    // timer check，精确到100毫秒
    static uint64 timer_check = 0;
    if (timer_check + EN_Timer_Check_Interval_Msec <= now_msec)
    {
        timer_check = now_msec;
        TimerManager::Instance()->Tick();
    }
    // session 超时检查
    static uint32 session_check = 0;
    if (session_check + EN_Session_Timeout_Check_Interval_Sec <= now)
    {
        session_check = now;
        SessionManager::Instance()->CheckTimeOut();
    }
}

