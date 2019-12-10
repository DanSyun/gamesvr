#include "message_handler_data.h"
#include "machine_manager.h"

///////////////////////////////////////////////////////////////////////////////
// 更新玩家信息
ENHandlerResult HandlerNotifyPlayerInfo::OnMessage(Session * psession)
{
    psession->AddQueryKey(psession->GetWebMsg().web_notify_player_info().uid(), PBDBKey::kBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerNotifyPlayerInfo::OnQuerySuccess(Session * psession)
{
    PBDBData& player = psession->GetDBData();
    player.mutable_base_info()->set_nick(psession->GetWebMsg().web_notify_player_info().nick());
    player.mutable_base_info()->set_pic_url(psession->GetWebMsg().web_notify_player_info().pic_url());
    return EN_Handler_Save;
}

ENHandlerResult HandlerNotifyPlayerInfo::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 刷新玩家积分
ENHandlerResult HandlerNotifyPoint::OnMessage(Session * psession)
{
    const WebNotifyPoint& notify = psession->GetWebMsg().web_notify_point();
    psession->AddQueryKey(notify.uid(), PBDBKey::kBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerNotifyPoint::OnQuerySuccess(Session * psession)
{
    const DBBaseInfo& info = psession->GetDBData().base_info();
    if (info.has_gamesvr_type())
    {
        psession->GetWebMsg().mutable_web_notify_point()->clear_machine_id();
        if (info.has_machine_id())
            psession->GetWebMsg().mutable_web_notify_point()->set_machine_id(info.machine_id());

        MessageBase::SendMsgToServer(NULL, psession->GetWebMsg(), (ENSvrType)info.gamesvr_type(), info.gamesvr_id());
    }
    return EN_Handler_Done;
}

ENHandlerResult HandlerNotifyPoint::OnQueryFailed(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 玩家发货
ENHandlerResult HandlerPlayerCharge::OnMessage(Session * psession)
{
    uint64 uid = psession->GetWebMsg().web_req_player_charge().uid();
    psession->AddQueryKey(uid, PBDBKey::kBaseInfo);
    psession->AddQueryKey(uid, PBDBKey::kGoldInfo);
    psession->AddQueryKey(uid, PBDBKey::kPrizeInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerPlayerCharge::OnQuerySuccess(Session * psession)
{
    const WebReqPlayerCharge& request = psession->GetWebMsg().web_req_player_charge();
    DataCommon::PlayerAddAwards(psession, psession->GetDBData(), (ENAccountReason)request.reason(), request.awards());
    return EN_Handler_Save;
}

ENHandlerResult HandlerPlayerCharge::OnQueryFailed(Session * psession)
{
    WebRspPlayerCharge& response = *psession->GetSSResponse().mutable_web_rsp_player_charge();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    
    return EN_Handler_Done;
}

ENHandlerResult HandlerPlayerCharge::OnSaveSuccess(Session * psession)
{
    WebRspPlayerCharge& response = *psession->GetSSResponse().mutable_web_rsp_player_charge();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToWeb(psession);

    const DBBaseInfo& info = psession->GetDBData().base_info();
    if (info.has_gamesvr_id() && !info.has_machine_id())
    {
        PBSSMsg msg;
        msg.mutable_ss_notify_player_gold()->set_uid(psession->GetDBData().uid());
        msg.mutable_ss_notify_player_gold()->set_gold(psession->GetDBData().gold_info().gold());
        MessageBase::SendMsgToServer(NULL, msg, (ENSvrType)info.gamesvr_type(), info.gamesvr_id());
    }
    return EN_Handler_Done;
}

ENHandlerResult HandlerPlayerCharge::OnSaveFailed(Session * psession)
{
    WebRspPlayerCharge& response = *psession->GetSSResponse().mutable_web_rsp_player_charge();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 扣除礼品
ENHandlerResult HandlerSubPrize::OnMessage(Session * psession)
{
    const WebReqSubPrize& request = psession->GetWebMsg().web_req_sub_prize();
    psession->AddQueryKey(request.uid(), PBDBKey::kPrizeInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerSubPrize::OnQuerySuccess(Session * psession)
{
    const WebReqSubPrize& request = psession->GetWebMsg().web_req_sub_prize();
    PBDBData& player = psession->GetDBData();
    for (auto i = 0; i < request.sub_prizes_size(); ++i)
    {
        if (request.sub_prizes(i).prize_num() > DataCommon::GetPlayerPrizeNum(player, request.sub_prizes(i).prize_id()))
        {
            WebRspSubPrize& response = *psession->GetSSResponse().mutable_web_rsp_sub_prize();
            response.set_result(EN_Web_Player_Prize_Not_Enough);
            MessageBase::SendMsgToWeb(psession);
            return EN_Handler_Done;
        }
    }

    for (auto i = 0; i < request.sub_prizes_size(); ++i)
    {
        const DBPrize& prize = request.sub_prizes(i);
        DataCommon::PlayerSubPrize(psession, player, (ENAccountReason)request.reason(), prize.prize_id(), prize.prize_num());
    }

    return EN_Handler_Save;
}

ENHandlerResult HandlerSubPrize::OnQueryFailed(Session * psession)
{
    WebRspSubPrize& response = *psession->GetSSResponse().mutable_web_rsp_sub_prize();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSubPrize::OnSaveSuccess(Session * psession)
{
    WebRspSubPrize& response = *psession->GetSSResponse().mutable_web_rsp_sub_prize();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSubPrize::OnSaveFailed(Session * psession)
{
    WebRspSubPrize& response = *psession->GetSSResponse().mutable_web_rsp_sub_prize();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 获取DB数据
ENHandlerResult HandlerQueryData::OnMessage(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data();
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        psession->AddQueryKey(request.uids(i), PBDBKey::kBaseInfo);
        psession->AddQueryKey(request.uids(i), PBDBKey::kGoldInfo);
        psession->AddQueryKey(request.uids(i), PBDBKey::kPrizeInfo);
        psession->AddQueryKey(request.uids(i), PBDBKey::kGameInfo);
    }
    return EN_Handler_Query;
}

ENHandlerResult HandlerQueryData::OnQuerySuccess(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data();
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data();
    response.set_result(EN_Web_OK);
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        PBDBData* pdata = response.add_datas();
        pdata->CopyFrom(psession->GetDBData(i));
    }
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerQueryData::OnQueryFailed(Session * psession)
{
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

