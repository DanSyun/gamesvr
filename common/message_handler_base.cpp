#include "message_handler_base.h"

void MessageHandlerBase::Process(Session* psession)
{
    ENHandlerResult result = EN_Handler_Done;
    switch (psession->session_state())
    {
        case EN_Session_On_Message:         result = OnMessage(psession);           break;
        case EN_Session_Wait_Response:      result = OnServerResponse(psession);    break;
        case EN_Session_Wait_Query:         result = QueryResponse(psession);       break;
        case EN_Session_Wait_Save:          result = SaveResponse(psession);        break;
        default: break;
    }

    while (true)
    {
        switch (result)
        {
            case EN_Handler_Query:          result = QueryAllData(psession);  break;
            case EN_Handler_Save:           result = SaveOneData(psession);   break;
            case EN_Handler_Next:           result = ProcessNext(psession);   break;

            case EN_Handler_Done:           ProcessDone(psession);            return;
            case EN_Handler_Failed:         ProcessFailed(psession);          return;
            case EN_Handler_Hup:            /* do nothing */                  return;
            default: break;
        }
    }
}

ENHandlerResult MessageHandlerBase::OnMessage(Session* psession)
{
    // sub class should overwrite this
    return EN_Handler_Done;
}
ENHandlerResult MessageHandlerBase::OnServerResponse(Session* psession)
{
    // sub class should overwrite this
    return EN_Handler_Done;
}
ENHandlerResult MessageHandlerBase::OnQuerySuccess(Session * psession)
{
    // sub class should overwrite this
    return EN_Handler_Done;
}
ENHandlerResult MessageHandlerBase::OnSaveSuccess(Session * psession)
{
    // 默认回复玩家消息
    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}
ENHandlerResult MessageHandlerBase::OnQueryFailed(Session* psession)
{
    // 默认回复通用错误
    MessageBase::SendCommonErr(psession->GetConnectHead(), EN_Result_Sys_Err);
    return EN_Handler_Failed;
}
ENHandlerResult MessageHandlerBase::OnSaveFailed(Session* psession)
{
    // 默认回复通用错误
    MessageBase::SendCommonErr(psession->GetConnectHead(), EN_Result_Sys_Err);
    return EN_Handler_Failed;
}

ENHandlerResult MessageHandlerBase::ProcessNext(Session* psession)
{
    // 如果还有可处理的玩家数据则继续否则结束会话
    psession->set_process_index(psession->process_index() + 1);
    if ((int)psession->process_index() < psession->db_processs_size())
    {
        return OnQuerySuccess(psession);
    }
    else
    {
        return EN_Handler_Done;
    }
}

// db返回
ENHandlerResult MessageHandlerBase::QueryResponse(Session* psession)
{
    auto& response = psession->GetSSResponse().ss_rsp_query();
    // 拉取成功
    if (response.result() == EN_KVDB_Sucess)
    {
        // 解包
        PBDBData player;
        player.set_uid(response.uid());
        if (!MessageBase::ParseDBData(player, response.datas()))
            return OnQueryFailed(psession);

        int process_index = psession->GetDBProcessIndex(response.uid());
        if (process_index == -1)
            return OnQueryFailed(psession);

        psession->GetDBData(process_index).CopyFrom(player);

        // 如果拉到所有数据开始处理
        if (psession->HaveAllDBData())
        {
            return OnQuerySuccess(psession);
        }
        // 否则继续挂起等待回复
        else
        {
            return EN_Handler_Hup;
        }
    }
    // db 错误
    else
    {
        return OnQueryFailed(psession);
    }
}

ENHandlerResult MessageHandlerBase::SaveResponse(Session* psession)
{
    auto& response = psession->GetSSResponse().ss_rsp_save();
    // 保存成功
    if (response.result() == EN_KVDB_Sucess)
    {
        return OnSaveSuccess(psession);
    }
    // 版本号不一致，重做
    else if (response.result() == EN_KVDB_Ver_Diff)
    {
        // 重做次数限制
        psession->set_redo_count(psession->redo_count() + 1);
        if (psession->redo_count() >= EN_Max_DB_Redo_Time)
        {
            return OnSaveFailed(psession);
        }
        // 用新数据重做逻辑
        PBDBData player;
        player.set_uid(response.uid());
        if (!MessageBase::ParseDBData(player, response.datas()))
            return OnSaveFailed(psession);

        int process_index = psession->GetDBProcessIndex(response.uid());
        if (process_index == -1)
            return OnSaveFailed(psession);

        psession->GetDBData(process_index).CopyFrom(player);

        // 清除events
        psession->ClearEvents();

        return OnQuerySuccess(psession);
    }
    // db 错误
    else
    {
        return OnSaveFailed(psession);
    }
}

// 拉取所有数据
ENHandlerResult MessageHandlerBase::QueryAllData(Session* psession)
{
    if (MessageBase::SendQueryToDBProxy(psession))
    {
        return EN_Handler_Hup;
    }
    else //没有设置要拉取的数据
    {
        return EN_Handler_Failed;
    }
}
// 保存单个玩家数据
ENHandlerResult MessageHandlerBase::SaveOneData(Session* psession)
{
    if (MessageBase::SendSaveToDBProxy(psession))
    {
        return EN_Handler_Hup;
    }
    else //没有被改动的数据
    {
        return EN_Handler_Failed;
    }
}

void MessageHandlerBase::ProcessDone(Session* psession)
{
    // post 日志消息
    OnPostMessage(psession);
    SessionManager::Instance()->ReleaseSession(psession->session_id());
}

void MessageHandlerBase::ProcessFailed(Session* psession)
{
    SessionManager::Instance()->ReleaseSession(psession->session_id());
}

void MessageHandlerBase::OnPostMessage(Session* psession)
{
    for (auto i = 0; i < psession->events_size(); ++i)
    {
        const PBEvent& event = psession->events(i);
        MessageBase::SendMsgToEvent(event);

        // 统计代理数据
        if (event.event_name() == "gold_account")
        {
            static unordered_map<string, int64> event_map_num;
            static unordered_map<string, string> event_map_str;
            event_map_num.clear();
            event_map_str.clear();
            for (auto i = 0; i < event.num_fields_size(); ++i)
                event_map_num[event.num_fields(i).key()] = event.num_fields(i).value();
            for (auto i = 0; i < event.str_fields_size(); ++i)
                event_map_str[event.str_fields(i).key()] = event.str_fields(i).value();

            if (event_map_num["num"] <= 0) return;

            if (event_map_num["reason"] == EN_Acc_Game_Refund)
            {
                PBSSMsg msg;
                msg.set_uid(event_map_num["promoter_id"]);
                SSNotifyPromoterGold& request = *msg.mutable_ss_notify_promoter_gold();
                request.set_promoter_id(event_map_num["promoter_id"]);
                request.set_giving_num(- event_map_num["giving_num"]);
                MessageBase::SendMsgToPromoter(psession, msg);
            }
            else
            {
                PBSSMsg msg;
                msg.set_uid(event_map_num["promoter_id"]);
                SSNotifyPromoterGold& request = *msg.mutable_ss_notify_promoter_gold();
                request.set_promoter_id(event_map_num["promoter_id"]);
                request.set_charge_num(event_map_num["charge_num"]);
                request.set_giving_num(event_map_num["giving_num"]);
                MessageBase::SendMsgToPromoter(psession, msg);
            }
        }
    }
}

