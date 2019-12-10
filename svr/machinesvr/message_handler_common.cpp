#include "message_handler_common.h"
#include "machine_manager.h"
#include "config.h"

///////////////////////////////////////////////////////////////////////////////
// 心跳
ENHandlerResult HandlerHeartBeat::OnMessage(Session * psession)
{
    CSRspHeartBeat& res = *psession->GetCSResponse().mutable_cs_rsp_heart_beat();
    res.set_timestamp(GetTime());
    MessageBase::SendMsgToClient(psession);

    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine)
    {
        pmachine->set_heart_beat_time(GetTime());
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机登录
ENHandlerResult HandlerMLogin::OnMessage(Session * psession)
{
    psession->AddQueryKey(psession->uid(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerMLogin::OnQuerySuccess(Session * psession)
{
    PBDBData& machine = psession->GetDBData();
    machine.mutable_m_base_info()->set_machinesvr_id(MessageBase::GetSvrID());

    return EN_Handler_Save;
}

ENHandlerResult HandlerMLogin::OnSaveSuccess(Session * psession)
{
    PBDBData& machine = psession->GetDBData();
    CSRspMLogin& response = *psession->GetCSResponse().mutable_cs_rsp_m_login();
    response.set_use_heart_beat(MessageBase::GetSvrConf().use_heart_beat());
    response.set_heart_beat_interval(MessageBase::GetSvrConf().heart_beat_interval());
    response.set_game_type(machine.m_base_info().game_type());

    const auto paddr = ConfigGameAddr::Instance()->GetAddrByType((ENGameType)machine.m_base_info().game_type());
    if (paddr)
    {
        uint32 index = machine.uid() % paddr->hosts_size();
        response.set_game_host(paddr->hosts(index));
        response.set_game_port(paddr->ports(index));
    }

    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机刷新
ENHandlerResult HandlerUpdateMachine::OnMessage(Session * psession)
{
    const SSNotifyUpdateMachine& notify = psession->GetSSMsg().ss_notify_update_machine();
    Machine* pmachine = MachineManager::Instance()->GetMachine(notify.machine_id());
    if (pmachine)
    {
        psession->GetCSResponse().mutable_cs_notify_update_machine();
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), psession->GetCSResponse());
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机删除
ENHandlerResult HandlerDeleteMachine::OnMessage(Session * psession)
{
    const WebNotifyDeleteMachine& notify = psession->GetSSMsg().web_notify_delete_machine();
    Machine* pmachine = MachineManager::Instance()->GetMachine(notify.machine_id());
    if (pmachine)
    {
        psession->GetCSResponse().mutable_cs_notify_delete_machine();
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), psession->GetCSResponse());
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机重启
ENHandlerResult HandlerRestartMachine::OnMessage(Session * psession)
{
    const WebNotifyRestartMachine& notify = psession->GetSSMsg().web_notify_restart_machine();
    Machine* pmachine = MachineManager::Instance()->GetMachine(notify.machine_id());
    if (pmachine)
    {
        psession->GetCSResponse().mutable_cs_notify_restart_machine();
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), psession->GetCSResponse());
    }
    return EN_Handler_Done;
}

