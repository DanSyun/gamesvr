#include "message_handler_common.h"
#include "machine_manager.h"
#include "string_ext.hpp"

///////////////////////////////////////////////////////////////////////////////
// GM命令
ENHandlerResult HandlerGMCommand::OnMessage(Session * psession)
{
    const SSGMCommand& request = psession->GetWebMsg().ss_gm_command();
    uint64 uid = strtoull(SpiltString(request.cmd(), " ", 0).c_str(), NULL, 0);

    psession->AddQueryKey(uid, PBDBKey::kBaseInfo);
    psession->AddQueryKey(uid, PBDBKey::kGoldInfo);
    psession->AddQueryKey(uid, PBDBKey::kPrizeInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerGMCommand::OnQuerySuccess(Session * psession)
{
    const SSGMCommand& request = psession->GetWebMsg().ss_gm_command();
    PBDBData& data = psession->GetDBData();

    stCommand cmd;
    cmd.cmd = SpiltString(request.cmd(), " ", 1);
    cmd.arg1 = strtoull(SpiltString(request.cmd(), " ", 2).c_str(), NULL, 0);
    cmd.arg2 = strtoull(SpiltString(request.cmd(), " ", 3).c_str(), NULL, 0);

    if (cmd.cmd == "add_prize")
    {
        ProcessAddPrize(psession, data, cmd);
    }
    else if (cmd.cmd == "add_gold")
    {
        ProcessAddGold(psession, data, cmd);
    }
    else
    {
        SSGMCommand& response = *psession->GetSSResponse().mutable_ss_gm_command();
        response.set_result(EN_Web_DB_Error);
        MessageBase::SendMsgToWeb(psession, true);
        return EN_Handler_Done;
    }
    return EN_Handler_Save;
}

void HandlerGMCommand::ProcessAddPrize(Session * psession, PBDBData & data, const stCommand& cmd)
{
    DataCommon::PlayerAddPrize(psession, data, EN_Acc_GM_Add, cmd.arg1, cmd.arg2);
}

void HandlerGMCommand::ProcessAddGold(Session * psession, PBDBData & data, const stCommand& cmd)
{
    DataCommon::PlayerAddGold(psession, data, EN_Acc_GM_Add, cmd.arg1);
}

ENHandlerResult HandlerGMCommand::OnQueryFailed(Session * psession)
{
    SSGMCommand& response = *psession->GetSSResponse().mutable_ss_gm_command();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession, true);
    return EN_Handler_Done;
}

ENHandlerResult HandlerGMCommand::OnSaveSuccess(Session * psession)
{
    SSGMCommand& response = *psession->GetSSResponse().mutable_ss_gm_command();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToWeb(psession, true);
    return EN_Handler_Done;
}

ENHandlerResult HandlerGMCommand::OnSaveFailed(Session * psession)
{
    SSGMCommand& response = *psession->GetSSResponse().mutable_ss_gm_command();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession, true);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 更新MySQL配置
ENHandlerResult HandlerUpdateConfig::OnMessage(Session * psession)
{
    const string& name = psession->GetWebMsg().web_notify_update_config().name();
    if (name == "prize")
    {
        MessageBase::BroadcastToServer(psession->GetWebMsg(), EN_4_Lipstick);
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机玩家登录
ENHandlerResult HandlerPlayerLogin::OnMessage(Session * psession)
{
    const WebReqPlayerLogin& request = psession->GetWebMsg().web_req_player_login();
    psession->AddQueryKey(request.machine_id(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerPlayerLogin::OnQuerySuccess(Session * psession)
{
    const DBMBaseInfo& info = psession->GetDBData().m_base_info();
    if (!info.has_gamesvr_id())
    {
        PBSSMsg& msg = psession->GetSSResponse();
        msg.mutable_web_rsp_player_login()->set_result(EN_Web_Machine_Not_Online);
        MessageBase::SendMsgToWeb(psession);
        return EN_Handler_Done;
    }
    else
    {
        MessageBase::SendMsgToServer(psession, psession->GetWebMsg(), (ENSvrType)info.gamesvr_type(), info.gamesvr_id());
        return EN_Handler_Hup;
    }
}

ENHandlerResult HandlerPlayerLogin::OnServerResponse(Session * psession)
{
//    psession->GetSSResponse().mutable_web_rsp_player_login()->mutable_player()->mutable_base_info()->clear_nick();
    psession->GetSSResponse().mutable_web_rsp_player_login()->mutable_player()->mutable_base_info()->clear_pic_url();
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机玩家投币
ENHandlerResult HandlerPlayerBuyIn::OnMessage(Session * psession)
{
    const WebReqPlayerBuyIn& request = psession->GetWebMsg().web_req_player_buy_in();
    psession->AddQueryKey(request.machine_id(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerPlayerBuyIn::OnQuerySuccess(Session * psession)
{
    const DBMBaseInfo& info = psession->GetDBData().m_base_info();
    if (!info.has_gamesvr_id())
    {
        PBSSMsg& msg = psession->GetSSResponse();
        msg.mutable_web_rsp_player_buy_in()->set_result(EN_Web_Machine_Not_Online);
        MessageBase::SendMsgToWeb(psession);
        return EN_Handler_Done;
    }
    else
    {
        MessageBase::SendMsgToServer(psession, psession->GetWebMsg(), (ENSvrType)info.gamesvr_type(), info.gamesvr_id());
        return EN_Handler_Hup;
    }
}

ENHandlerResult HandlerPlayerBuyIn::OnServerResponse(Session * psession)
{
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机删除
ENHandlerResult HandlerDeleteMachine::OnMessage(Session * psession)
{
    const WebNotifyDeleteMachine& notify = psession->GetWebMsg().web_notify_delete_machine();
    psession->AddQueryKey(notify.machine_id(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerDeleteMachine::OnQuerySuccess(Session * psession)
{
    const PBDBData& machine = psession->GetDBData();
    if (machine.m_base_info().has_machinesvr_id())
    {
        MessageBase::SendMsgToServer(NULL, psession->GetWebMsg(), EN_2_Machine, machine.m_base_info().machinesvr_id());
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机重启
ENHandlerResult HandlerRestartMachine::OnMessage(Session * psession)
{
    const WebNotifyRestartMachine& notify = psession->GetWebMsg().web_notify_restart_machine();
    psession->AddQueryKey(notify.machine_id(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerRestartMachine::OnQuerySuccess(Session * psession)
{
    const PBDBData& machine = psession->GetDBData();
    if (machine.m_base_info().has_machinesvr_id())
    {
        MessageBase::SendMsgToServer(NULL, psession->GetWebMsg(), EN_2_Machine, machine.m_base_info().machinesvr_id());
    }
    return EN_Handler_Done;
}

