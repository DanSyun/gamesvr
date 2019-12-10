#include "message_handler_data_p.h"

///////////////////////////////////////////////////////////////////////////////
// 获取DB数据
ENHandlerResult HandlerQueryDataP::OnMessage(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data_p();
    if (request.uids_size() == 0)
    {
        WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_p();
        response.set_result(EN_Web_Invalid_Params);
        MessageBase::SendMsgToWeb(psession);
        return EN_Handler_Done;
    }
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        psession->AddQueryKey(request.uids(i), PBDBKey::kMLipstickInfo);
    }
    return EN_Handler_Query;
}

ENHandlerResult HandlerQueryDataP::OnQuerySuccess(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data_p();
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_p();
    response.set_result(EN_Web_OK);
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        PBDBData* pdata = response.add_datas();
        pdata->CopyFrom(psession->GetDBData(i));
    }
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerQueryDataP::OnQueryFailed(Session * psession)
{
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_p();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 修改DB数据
ENHandlerResult HandlerSaveDataP::OnMessage(Session * psession)
{
    const WebReqSaveDBData& request = psession->GetWebMsg().web_req_save_db_data_p();
    psession->AddQueryKey(request.uid(), PBDBKey::kMLipstickInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerSaveDataP::OnQuerySuccess(Session * psession)
{
    PBDBData& data = psession->GetDBData();
    const PBDBData& update = psession->GetWebMsg().web_req_save_db_data_p().data();

    if (update.has_m_lipstick_info())
    {
        const DBMLipstickInfo& info = update.m_lipstick_info();
        // 业务数据
        if (info.has_max_retry_time())
            data.mutable_m_lipstick_info()->set_max_retry_time(info.max_retry_time());
        if (info.prizes_size())
            data.mutable_m_lipstick_info()->mutable_prizes()->CopyFrom(info.prizes());
        if (info.lottery_awards_size() == 12)
        {
            uint32 rate = 0;
            data.mutable_m_lipstick_info()->clear_lottery_awards();
            for (auto i = 0; i < info.lottery_awards_size(); ++i)
            {
                const DBLotteryAward& lottery = info.lottery_awards(i);
                rate += lottery.rate();
                DBLotteryAward* plottery = data.mutable_m_lipstick_info()->add_lottery_awards();
                plottery->CopyFrom(lottery);
                plottery->set_rate(rate);
            }
        }

        // 保底购买
        if (info.has_buy_consume())
            data.mutable_m_lipstick_info()->set_buy_consume(info.buy_consume());
        if (info.has_buy_fail())
            data.mutable_m_lipstick_info()->set_buy_fail(info.buy_fail());
        if (info.has_buy_time())
            data.mutable_m_lipstick_info()->set_buy_time(info.buy_time());
        if (info.round_knives_size() && info.round_knives_size() == info.round_timeouts_size())
        {
            data.mutable_m_lipstick_info()->mutable_round_knives()->CopyFrom(info.round_knives());
            data.mutable_m_lipstick_info()->mutable_round_timeouts()->CopyFrom(info.round_timeouts());
        }

        // 超时相关数据
        if (info.has_start_timeout())
            data.mutable_m_lipstick_info()->set_start_timeout(info.start_timeout());
        if (info.has_retry_timeout())
            data.mutable_m_lipstick_info()->set_retry_timeout(info.retry_timeout());
        if (info.has_lottery_timeout())
            data.mutable_m_lipstick_info()->set_lottery_timeout(info.lottery_timeout());
        if (info.has_lottery_ani_timeout())
            data.mutable_m_lipstick_info()->set_lottery_ani_timeout(info.lottery_ani_timeout());
        if (info.has_buy_timeout())
            data.mutable_m_lipstick_info()->set_buy_timeout(info.buy_timeout());
        if (info.has_buy_ani_timeout())
            data.mutable_m_lipstick_info()->set_buy_ani_timeout(info.buy_ani_timeout());
        if (info.has_game_succ_timeout())
            data.mutable_m_lipstick_info()->set_game_succ_timeout(info.game_succ_timeout());

        // 功能开关
        if (info.has_retry_switch())
            data.mutable_m_lipstick_info()->set_retry_switch(info.retry_switch());
        if (info.has_lottery_switch())
            data.mutable_m_lipstick_info()->set_lottery_switch(info.lottery_switch());
        if (info.has_buy_switch())
            data.mutable_m_lipstick_info()->set_buy_switch(info.buy_switch());
    }
    return EN_Handler_Save;
}

ENHandlerResult HandlerSaveDataP::OnQueryFailed(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_p();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSaveDataP::OnSaveSuccess(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_p();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSaveDataP::OnSaveFailed(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_p();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

