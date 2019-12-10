#include "message.h"
#include "h5_machine_manager.h"

#include "cipher.hpp"
#include "base64.hpp"

bool Message::MessageFilter(stConnectHead& head, const char* buf, uint32 buf_len, PBCSMsg& msg)
{
    if (!ParseMsgFromClient(buf, buf_len, msg))
    {
        SendCommonErr(head, EN_Result_Invalid_Msg);
        return false;
    }

    switch (msg.msg_union_case())
    {
        case PBCSMsg::kCsReqMLoginGame:
            return OnMachineLogin(head, msg);

        case PBCSMsg::kCsReqLoginGameH5:
            return OnPlayerLogin(head, msg);

        default:
            return OnLoginCheck(head, msg);
    }
}

///////////////////////////////////////////////////////////////////////////////
// 街机登录请求
bool Message::OnMachineLogin(stConnectHead& head, const PBCSMsg & msg)
{
    Machine* pmachine = NULL;
    uint64 machine_id = msg.uid();
    const string& token = msg.cs_req_m_login_game().token();

    // 验证token
    int8 decrypted[128];
    if (!AESCipher::Instance()->Decrypt((uint8*)base64_decode(token).c_str(), (uint8*)_pserver->GetLoginKey().c_str(), (uint8*)decrypted))
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }
    if ((uint64)atoll(decrypted) != machine_id)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }
    char* divide = NULL;
    if ((divide = strstr(decrypted, "#$#")) == NULL)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }
    auto auth_time = atoi(divide + strlen("#$#"));
    if ((uint32)auth_time + 86400 <= GetTime())
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }

    // 重复登录
    pmachine = MachineManager::Instance()->GetMachine(machine_id);
    if (pmachine)
    {
        const auto& old_head = pmachine->GetConnectHead();
        // 同一个连接发了两次登录
        if (old_head._socket_pos == head._socket_pos && old_head._create_time == head._create_time)
        {
            pmachine->set_heart_beat_time(GetTime());
            return true;
        }
        // 踢掉上一个连接
        else
        {
            LogoutMachine(pmachine, true, EN_Logout_Multi_Login, false);
        }
    }

    // 通知connect 建立索引
    if (!_pserver->NotifyConnectLogicIndex(head, machine_id))
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Sys_Err);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pmachine = MachineManager::Instance()->CreateMachine(machine_id);
    if (!pmachine)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Server_Full);
        msg.mutable_cs_rsp_m_login_game();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pmachine->SetConnectHead(head);
    pmachine->set_heart_beat_time(GetTime());
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// 玩家登录请求
bool Message::OnPlayerLogin(stConnectHead & head, const PBCSMsg & msg)
{
    Player* pplayer = NULL;
    uint64 player_id = msg.uid();
    const string& token = msg.cs_req_login_game_h5().token();

    // 验证token
    int8 decrypted[128];
    if (!AESCipher::Instance()->Decrypt((uint8*)base64_decode(token).c_str(), (uint8*)_pserver->GetLoginKey().c_str(), (uint8*)decrypted))
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }
    if ((uint64)atoll(decrypted) != player_id)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }
    char* divide = NULL;
    if ((divide = strstr(decrypted, "#$#")) == NULL)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }
    auto auth_time = atoi(divide + strlen("#$#"));
    if ((uint32)auth_time + 86400 <= GetTime())
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }

    // 重复登录
    pplayer = PlayerManager::Instance()->GetPlayer(player_id);
    if (pplayer)
    {
        const auto& old_head = pplayer->GetConnectHead();
        // 同一个连接发了两次登录
        if (old_head._socket_pos == head._socket_pos && old_head._create_time == head._create_time)
        {
            pplayer->set_heart_beat_time(GetTime());
            return true;
        }
        // 踢掉上一个连接
        else
        {
            LogoutPlayer(pplayer, true, EN_Logout_Multi_Login, false);
        }
    }

    // 通知connect 建立索引
    if (!_pserver->NotifyConnectLogicIndex(head, player_id))
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Sys_Err);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pplayer = PlayerManager::Instance()->CreatePlayer(player_id);
    if (!pplayer)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Server_Full);
        msg.mutable_cs_rsp_login_game_h5();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pplayer->SetConnectHead(head);
    pplayer->set_heart_beat_time(GetTime());
    return true;

}

///////////////////////////////////////////////////////////////////////////////
// 检查登录态
bool Message::OnLoginCheck(stConnectHead & head, const PBCSMsg & msg)
{
    if (msg.uid() < 10000000)
    {
        uint64 machine_id = msg.uid();
        Machine* pmachine = MachineManager::Instance()->GetMachine(machine_id);
        if (!pmachine)
        {
            SendCommonErr(head, EN_Result_Not_Login);
            return false;
        }

        const auto& old_head = pmachine->GetConnectHead();
        if (head._socket_pos != old_head._socket_pos || head._create_time != old_head._create_time)
        {
            SendCommonErr(head, EN_Result_Not_Login);
            return false;
        }

        pmachine->set_heart_beat_time(GetTime());
        return true;
    }
    else
    {
        uint64 player_id = msg.uid();
        Player* pplayer = PlayerManager::Instance()->GetPlayer(player_id);
        if (!pplayer)
        {
            SendCommonErr(head, EN_Result_Not_Login);
            return false;
        }

        const auto& old_head = pplayer->GetConnectHead();
        if (head._socket_pos != old_head._socket_pos || head._create_time != old_head._create_time)
        {
            SendCommonErr(head, EN_Result_Not_Login);
            return false;
        }

        pplayer->set_heart_beat_time(GetTime());
        return true;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 登出街机
void Message::LogoutMachine(uint64 machine_id, bool notify_cli, ENLogoutReason reason, bool clear_db)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(machine_id);
    if (pmachine)
    {
        LogoutMachine(pmachine, notify_cli, reason, clear_db);
    }
}

void Message::LogoutMachine(Machine* pmachine, bool notify_cli, ENLogoutReason reason, bool clear_db)
{
    if (pmachine == NULL) return;

    // 有玩家
    if (pmachine->game_state() != EN_State_Wait_Player)
    {
        // 定时器清除
        TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
        // 清除投币
        PBInternalMsg msg;
        msg.mutable_internal_clear_buy_in()->set_player_id(pmachine->player_id());
        msg.mutable_internal_clear_buy_in()->set_machine_id(pmachine->machine_id());
        SendMsgToInternal(msg);
        // 退款
        if (pmachine->game_state() == EN_State_Wait_End)
        {
            // 两个消息留出间隔
            msg.mutable_internal_game_refund()->set_player_id(pmachine->player_id());
            msg.mutable_internal_game_refund()->set_refund_num(pmachine->machine().m_lipstick_info().cost());
            TimerManager::Instance()->CreateTimerMsec(100, msg);
        }
    }

    // 踢出消息
    if (notify_cli == true)
    {
        PBCSMsg msg;
        msg.mutable_cs_notify_logout()->set_reason(reason);
        SendMsgToClient(pmachine->GetConnectHead(), msg, true);
    }

    // 修改db
    if (clear_db == true)
    {
        PBInternalMsg internal;
        internal.mutable_internal_logout()->set_uid(pmachine->machine_id());
        SendMsgToInternal(internal);
    }

    // 删除对象
    MachineManager::Instance()->ReleaseMachine(pmachine->machine_id());
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// 登出玩家
void Message::LogoutPlayer(uint64 player_id, bool notify_cli, ENLogoutReason reason, bool clear_db)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(player_id);
    if (pplayer)
    {
        LogoutPlayer(pplayer, notify_cli, reason, clear_db);
    }
}

void Message::LogoutPlayer(Player* pplayer, bool notify_cli, ENLogoutReason reason, bool clear_db)
{
    if (pplayer == NULL) return;

    // 定时器清除
    if (pplayer->game_state() != EN_State_Wait_Start)
    {
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
    }

    // 退款
    if (pplayer->game_state() == EN_State_Wait_End && !pplayer->pre_end())
    {
        int32 prize_index = -1;
        const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
        for (auto i = 0; i < info.prizes_size(); ++i)
        {
            if (pplayer->prize_id() == info.prizes(i).prize_id())
            {
                prize_index = i;
                break;
            }
        }
        if (prize_index != -1)
        {
            PBInternalMsg msg;
            msg.mutable_internal_game_refund()->set_player_id(pplayer->player_id());
            msg.mutable_internal_game_refund()->set_refund_num(info.prizes(prize_index).cost());
            SendMsgToInternal(msg);
        }
    }

    // 踢出消息
    if (notify_cli == true)
    {
        PBCSMsg msg;
        msg.mutable_cs_notify_logout()->set_reason(reason);
        SendMsgToClient(pplayer->GetConnectHead(), msg, true);
    }

    // 修改db
    if (clear_db == true)
    {
        PBInternalMsg internal;
        internal.mutable_internal_logout()->set_uid(pplayer->player_id());
        SendMsgToInternal(internal);
    }

    // 释放街机
    if (pplayer->machine().has_uid())
        H5MachineManager::Instance()->Release(pplayer->machine().uid());

    // 删除对象
    PlayerManager::Instance()->ReleasePlayer(pplayer->player_id());
    return true;
}

