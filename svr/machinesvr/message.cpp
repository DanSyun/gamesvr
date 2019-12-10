#include "message.h"

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
        case PBCSMsg::kCsReqMLogin:
            return OnMachineLogin(head, msg);

        default:
            return OnLoginCheck(head, msg);
    }
}

bool Message::OnMachineLogin(stConnectHead& head, const PBCSMsg & msg)
{
    Machine* pmachine = NULL;
    uint64 machine_id = msg.uid();
    const string& token = msg.cs_req_m_login().token();

    // 验证token
    int8 decrypted[128];
    if (!AESCipher::Instance()->Decrypt((uint8*)base64_decode(token).c_str(), (uint8*)_pserver->GetLoginKey().c_str(), (uint8*)decrypted))
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }
    if ((uint64)atoll(decrypted) != machine_id)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }
    char* divide = NULL;
    if ((divide = strstr(decrypted, "#$#")) == NULL)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }
    auto auth_time = atoi(divide + strlen("#$#"));
    if ((uint32)auth_time + 86400 <= GetTime())
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Auth_Failed);
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }

    // 重复登录
    pmachine = MachineManager::Instance()->GetMachine(machine_id);
    if (pmachine)
    {
        const auto& old_head = pmachine->GetConnectHead();
        if (old_head._socket_pos == head._socket_pos && old_head._create_time == head._create_time)
        {
            return true;
        }
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
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pmachine = MachineManager::Instance()->CreateMachine(machine_id);
    if (!pmachine)
    {
        PBCSMsg msg;
        msg.set_result(EN_Result_Server_Full);
        msg.mutable_cs_rsp_m_login();
        SendMsgToClient(head, msg, true);
        return false;
    }

    pmachine->SetConnectHead(head);
    return true;
}

bool Message::OnLoginCheck(stConnectHead & head, const PBCSMsg & msg)
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

    return true;
}

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

