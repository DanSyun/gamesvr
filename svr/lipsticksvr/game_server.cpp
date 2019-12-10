#include "game_server.h"

#include "message.h"
#include "message_manager.h"
#include "session_manager.h"
#include "timer_manager.h"
#include "config.h"
#include "h5_machine_manager.h"

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

    // 街机管理器
    if (!MachineManager::Instance()->Init(GetSvrConf().max_machine_num(), GetSvrConf().machine_shm_key()))
        return -3;

    // 玩家管理器
    if (!PlayerManager::Instance()->Init(GetSvrConf().max_player_num(), GetSvrConf().player_shm_key()))
        return -4;

    int ret;
    if ((ret = LoadConfig()) != 0)
        return ret;

    // 消息处理
    MessageManager::Instance()->Init();

    // h5 街机管理
    H5MachineManager::Instance()->Init();

    return 0;
}

int GameServer::LoadConfig()
{
    // 读取MySQL配置
    if (!ConfigMysql::Instance()->Init("../../../config/mysql_addr.cfg")) return -11;

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
    MachineManager::Instance()->Release();
    PlayerManager::Instance()->Release();
}

void GameServer::OnClientMessage(stConnectHead& head, const char* buf, uint32 buf_len)
{
    // 解包，消息过滤，处理
    PBCSMsg cs_msg;
    if (!Message::MessageFilter(head, buf, buf_len, cs_msg))
        return;

    // 创建会话
    Session* psession = SessionManager::Instance()->CreateSession(cs_msg.msg_union_case());
    if (!psession)
    {
        MessageBase::SendCommonErr(head, EN_Result_Sys_Err);
        return;
    }

    // 保存请求状态
    psession->SetConnectHead(head);
    psession->SetCSMsg(cs_msg);
    psession->set_uid(cs_msg.uid());
    // handler 处理
    MessageManager::Instance()->Process(psession);
}

void GameServer::OnClientOffline(const stConnectHead& head, uint64 logic_index)
{
    if (logic_index < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(logic_index);
        if (!pmachine) return;

        const stConnectHead& old_head = pmachine->GetConnectHead();
        if (old_head._socket_pos != head._socket_pos || old_head._create_time != head._create_time) return;

        Message::LogoutMachine(pmachine);
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(logic_index);
        if (!pplayer) return;

        const stConnectHead& old_head = pplayer->GetConnectHead();
        if (old_head._socket_pos != head._socket_pos || old_head._create_time != head._create_time) return;

        Message::LogoutPlayer(pplayer);
    }
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
        LOG_INFO("Machine count: %d", MachineManager::Instance()->Size());
        LOG_INFO("Player count: %d", PlayerManager::Instance()->Size());
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

    // 街机心跳检查
    static uint32 m_heart_beat_check = 0;
    if (m_heart_beat_check + GetSvrConf().heart_beat_interval() <= now)
    {
        m_heart_beat_check = now;

        std::vector<uint64> ids;
        for (auto iter = MachineManager::Instance()->Begin(); iter != MachineManager::Instance()->End(); ++iter)
        {
            if (iter->heart_beat_time() + GetSvrConf().heart_beat_interval() * 2 <= GetTime())
                ids.push_back(iter->machine_id());
        }
        for (auto iter = ids.begin(); iter != ids.end(); ++iter)
        {
            Message::LogoutMachine(*iter, true, EN_Logout_Heart_Beat);
        }
    }

    // 玩家心跳检查
    static uint32 p_heart_beat_check = 0;
    if (p_heart_beat_check + GetSvrConf().heart_beat_interval() <= now)
    {
        p_heart_beat_check = now;

        std::vector<uint64> ids;
        for (auto iter = PlayerManager::Instance()->Begin(); iter != PlayerManager::Instance()->End(); ++iter)
        {
            if (iter->heart_beat_time() + GetSvrConf().heart_beat_interval() * 2 <= GetTime())
                ids.push_back(iter->player_id());
        }
        for (auto iter = ids.begin(); iter != ids.end(); ++iter)
        {
            Message::LogoutPlayer(*iter, true, EN_Logout_Heart_Beat);
        }
    }
}

