#include "message_handler_common.h"
#include "machine_manager.h"
#include "timer_manager.h"
#include "config.h"
#include "system.hpp"

///////////////////////////////////////////////////////////////////////////////
// 心跳
ENHandlerResult HandlerHeartBeat::OnMessage(Session * psession)
{
    CSRspHeartBeat& res = *psession->GetCSResponse().mutable_cs_rsp_heart_beat();
    res.set_timestamp(GetTime());
    MessageBase::SendMsgToClient(psession);

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机登录游戏
ENHandlerResult HandlerMLoginGame::OnMessage(Session * psession)
{
    psession->AddQueryKey(psession->uid(), PBDBKey::kMBaseInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kMLipstickInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerMLoginGame::OnQuerySuccess(Session * psession)
{
    // 保存登录态
    PBDBData& machine = psession->GetDBData();
    machine.mutable_m_base_info()->set_gamesvr_id(MessageBase::GetSvrID());
    machine.mutable_m_base_info()->set_gamesvr_type(MessageBase::GetSvrType());

    return EN_Handler_Save;
}

ENHandlerResult HandlerMLoginGame::OnSaveSuccess(Session * psession)
{
    PBDBData& machine = psession->GetDBData();
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    pmachine->mutable_machine()->CopyFrom(machine);

    CSRspMLoginGame& response = *psession->GetCSResponse().mutable_cs_rsp_m_login_game();
    response.set_use_heart_beat(MessageBase::GetSvrConf().use_heart_beat());
    response.set_heart_beat_interval(MessageBase::GetSvrConf().heart_beat_interval());

    const DBMLipstickInfo& info = machine.m_lipstick_info();
    response.mutable_lipsitck_info()->set_cost(info.cost());
    response.mutable_lipsitck_info()->set_max_retry_time(info.max_retry_time());
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        uint64 prize_id = info.prizes(i).prize_id();
        if (ConfigPrize::Instance()->IsExist(prize_id))
            response.mutable_lipsitck_info()->add_prize_ids(prize_id);
    }
    for (auto i = 0; i < info.lottery_awards_size(); ++i)
    {
        response.mutable_lipsitck_info()->add_lotterys()->mutable_awards()->CopyFrom(info.lottery_awards(i).awards());
    }

    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机玩家登录
ENHandlerResult HandlerPlayerLogin::OnMessage(Session * psession)
{
    const WebReqPlayerLogin& request = psession->GetSSMsg().web_req_player_login();
    Machine* pmachine = MachineManager::Instance()->GetMachine(request.machine_id());
    // 街机不在线
    if (!pmachine)
    {
        WebRspPlayerLogin& response = *psession->GetSSResponse().mutable_web_rsp_player_login();
        response.set_result(EN_Web_Machine_Not_Online);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    pmachine->set_heart_beat_time(GetTime());

    psession->AddQueryKey(request.uid(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(request.uid(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(request.uid(), PBDBKey::kPrizeInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerPlayerLogin::OnQuerySuccess(Session * psession)
{
    const WebReqPlayerLogin& request = psession->GetSSMsg().web_req_player_login();
    Machine* pmachine = MachineManager::Instance()->GetMachine(request.machine_id());
    PBDBData& player = psession->GetDBData();
    // 已有别的玩家登录
    if (pmachine->game_state() != EN_State_Wait_Player)
    {
        if (pmachine->player_id() != request.uid())
        {
            WebRspPlayerLogin& response = *psession->GetSSResponse().mutable_web_rsp_player_login();
            response.set_result(EN_Web_Machine_In_Use);
            response.mutable_player()->CopyFrom(player);
            MessageBase::SendMsgToResponse(psession);
            return EN_Handler_Done;
        }
        else
        {
            WebRspPlayerLogin& response = *psession->GetSSResponse().mutable_web_rsp_player_login();
            response.set_result(EN_Web_OK);
            response.mutable_player()->CopyFrom(player);
            MessageBase::SendMsgToResponse(psession);
            return EN_Handler_Done;
        }
    }

    // 顶号处理
    const DBBaseInfo& base_info = player.base_info();
    if (base_info.has_gamesvr_type())
    {
        // 在游戏或者其他街机上登录
        if (!base_info.has_machine_id() || base_info.machine_id() != pmachine->machine_id())
        {
            PBSSMsg ss_msg;
            ss_msg.mutable_ss_notify_kick_player()->set_player_id(player.uid());
            if (base_info.has_machine_id())
            {
                ss_msg.mutable_ss_notify_kick_player()->set_machine_id(base_info.machine_id());
            }
            MessageBase::SendMsgToServer(NULL, ss_msg, (ENSvrType)base_info.gamesvr_type(), base_info.gamesvr_id());
        }
    }

    // 保存状态
    pmachine->set_game_state(EN_State_Wait_Start);
    pmachine->set_player_id(request.uid());
    pmachine->clear_total_consume();
    pmachine->clear_total_fail();
    pmachine->clear_total_time();
    uint32 timeout = pmachine->machine().m_lipstick_info().start_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));

    // 通知街机
    CSNotifyMPlayerLogin& notify = *psession->GetCSResponse().mutable_cs_notify_m_player_login();
    notify.set_uid(player.uid());
    notify.set_nick(player.base_info().nick());
    notify.set_pic_url(player.base_info().pic_url());
    notify.set_timeout(timeout);
    notify.set_prize_num(DataCommon::GetPlayerPrizeNum(player));
    MessageBase::SendMsgToClient(pmachine->GetConnectHead(), psession->GetCSResponse());

    // 
    DataCommon::CheckPlayerBuyInTimeout(psession, player);
    player.mutable_base_info()->set_gamesvr_type(MessageBase::GetSvrType());
    player.mutable_base_info()->set_gamesvr_id(MessageBase::GetSvrID());
    player.mutable_base_info()->set_machine_id(pmachine->machine_id());
    return EN_Handler_Save;
}

ENHandlerResult HandlerPlayerLogin::OnSaveSuccess(Session * psession)
{
    // 回复web
    WebRspPlayerLogin& response = *psession->GetSSResponse().mutable_web_rsp_player_login();
    response.set_result(EN_Web_OK);
    response.mutable_player()->CopyFrom(psession->GetDBData());
    MessageBase::SendMsgToResponse(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机玩家投币
ENHandlerResult HandlerPlayerBuyIn::OnMessage(Session * psession)
{
    const WebReqPlayerBuyIn& request = psession->GetSSMsg().web_req_player_buy_in();
    Machine* pmachine = MachineManager::Instance()->GetMachine(request.machine_id());
    // 街机不在线
    if (!pmachine)
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Machine_Not_Online);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    pmachine->set_heart_beat_time(GetTime());

    psession->AddQueryKey(request.uid(), PBDBKey::kGoldInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerPlayerBuyIn::OnQuerySuccess(Session * psession)
{
    const WebReqPlayerBuyIn& request = psession->GetSSMsg().web_req_player_buy_in();
    Machine* pmachine = MachineManager::Instance()->GetMachine(request.machine_id());
    // 街机状态不对
    if (pmachine->game_state() == EN_State_Wait_Player)
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Player_Not_Login);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    // 不是当前玩家
    if (pmachine->player_id() != request.uid())
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Player_Not_Login);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    // 钱不够
    PBDBData& player = psession->GetDBData();
    DBGoldInfo& gold_info = *player.mutable_gold_info();
    if (gold_info.gold() < request.buy_in())
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Player_Gold_Not_Enough);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }

    // 修改数据
    CSNotifyMPlayerBuyIn& notify = *psession->GetCSResponse().mutable_cs_notify_m_player_buy_in();
    notify.set_cur_buy_in(DataCommon::PlayerAddBuyIn(psession, player, request.buy_in(), request.machine_id()));
    return EN_Handler_Save;
}

ENHandlerResult HandlerPlayerBuyIn::OnSaveSuccess(Session * psession)
{
    const WebReqPlayerBuyIn& request = psession->GetSSMsg().web_req_player_buy_in();
    Machine* pmachine = MachineManager::Instance()->GetMachine(request.machine_id());
    // 街机状态不对
    if (pmachine->game_state() == EN_State_Wait_Player)
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Player_Not_Login);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    // 不是当前玩家
    if (pmachine->player_id() != request.uid())
    {
        WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
        response.set_result(EN_Web_Player_Not_Login);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }

    // 通知街机
    MessageBase::SendMsgToClient(pmachine->GetConnectHead(), psession->GetCSResponse());

    // 回复web
    WebRspPlayerBuyIn& response = *psession->GetSSResponse().mutable_web_rsp_player_buy_in();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToResponse(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机开始游戏
ENHandlerResult HandlerStartGame::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(pmachine->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerStartGame::OnQuerySuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    // 机器状态不对
    if (pmachine->game_state() != EN_State_Wait_Start)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 礼品不存在
    int32 prize_index = -1;
    const CSReqStartLipstick& request = psession->GetCSMsg().cs_req_start_lipstick();
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (request.prize_id() == info.prizes(i).prize_id())
        {
            prize_index = i;
            break;
        }
    }
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(request.prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 金币不够
    PBDBData& player = psession->GetDBData();
    if (DataCommon::GetPlayerBuyIn(player, pmachine->machine_id()) < info.cost())
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 扣金币
    uint64 left = DataCommon::PlayerSubBuyIn(psession, player, EN_Acc_Lip_Play, info.cost(), pmachine->machine_id());

    // 回复
    CSRspStartLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
    response.set_left_buy_in(left);
    response.set_retry_timeout(info.retry_timeout());
    response.set_retry_switch(info.retry_switch());
    response.mutable_round_timeouts()->CopyFrom(info.round_timeouts());
    response.mutable_round_knives()->CopyFrom(info.round_knives());

    return EN_Handler_Save;
}

ENHandlerResult HandlerStartGame::OnSaveSuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    // 机器状态不对
    if (pmachine->game_state() != EN_State_Wait_Start)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 礼品不存在
    int32 prize_index = -1;
    const CSReqStartLipstick& request = psession->GetCSMsg().cs_req_start_lipstick();
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (request.prize_id() == info.prizes(i).prize_id())
        {
            prize_index = i;
            break;
        }
    }
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(request.prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    pmachine->set_game_state(EN_State_Wait_End);
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = DataCommon::SumRepeated32(info.round_timeouts()) + info.retry_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_end_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));

    // 保底购买相关
    pmachine->set_total_consume(pmachine->total_consume() + info.cost());
    pmachine->set_update_time(GetTime());
    pmachine->clear_retry_time();

    // 算法
    pmachine->mutable_machine()->mutable_m_base_info()->set_total_income(info.cost() + pmachine->machine().m_base_info().total_income());
    msg.mutable_internal_update_machine_prize()->set_machine_id(pmachine->machine_id());
    msg.mutable_internal_update_machine_prize()->set_income(pmachine->machine().m_base_info().total_income());
    MessageBase::SendMsgToInternal(msg);

    // 结果
    uint32 rate = DataCommon::GetPrizeRate(pmachine->machine(), info.cost(), info.prizes(prize_index).rate(),
        ConfigPrize::Instance()->GetPrize(info.prizes(prize_index).prize_id())->value,
        DataCommon::GetPromoterChargeRate(psession->GetDBData(1)));
    uint32 rand_num = rand() % 100000;
    bool can_get_prize = false;
    if (rand_num < rate) can_get_prize = true;
    pmachine->set_can_get_prize(can_get_prize);
    pmachine->set_prize_id(request.prize_id());

    CSRspStartLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_start_lipstick();
    response.set_can_get_prize(pmachine->can_get_prize());
    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机游戏续命
ENHandlerResult HandlerRetryGame::OnMessage(Session * psession)
{
    // 街机状态不对
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (!pmachine->machine().m_lipstick_info().retry_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 参数非法
    const CSReqRetryLipstick& request = psession->GetCSMsg().cs_req_retry_lipstick();
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    if (request.round() == 0 || request.round() > (uint32)info.round_timeouts_size())
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Params);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 复活最大次数
    if (pmachine->retry_time() >= info.max_retry_time())
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Req);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(pmachine->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerRetryGame::OnQuerySuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 礼品不存在
    int32 prize_index = -1;
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (pmachine->prize_id() == info.prizes(i).prize_id())
        {
            prize_index = i;
            break;
        }
    }
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pmachine->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 金币不够
    PBDBData& player = psession->GetDBData();
    if (DataCommon::GetPlayerBuyIn(player, pmachine->machine_id()) < info.cost())
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 扣金币
    uint64 left = DataCommon::PlayerSubBuyIn(psession, player, EN_Acc_Lip_Retry, info.cost(), pmachine->machine_id());

    // 回复
    CSRspRetryLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
    response.set_left_buy_in(left);

    return EN_Handler_Save;
}

ENHandlerResult HandlerRetryGame::OnSaveSuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 礼品不存在
    int32 prize_index = -1;
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (pmachine->prize_id() == info.prizes(i).prize_id())
        {
            prize_index = i;
            break;
        }
    }
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pmachine->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    const CSReqRetryLipstick& request = psession->GetCSMsg().cs_req_retry_lipstick();
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = DataCommon::SumRepeated32(info.round_timeouts(), request.round() - 1) + info.retry_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_end_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));

    // 保底购买相关
    pmachine->set_total_consume(pmachine->total_consume() + info.cost());
    pmachine->set_total_fail(pmachine->total_fail() + 1);
    pmachine->set_total_time(pmachine->total_time() + GetTime() - pmachine->update_time());
    pmachine->set_update_time(GetTime());
    pmachine->set_retry_time(pmachine->retry_time() + 1);

    // 算法
    pmachine->mutable_machine()->mutable_m_base_info()->set_total_income(info.cost() + pmachine->machine().m_base_info().total_income());
    msg.mutable_internal_update_machine_prize()->set_machine_id(pmachine->machine_id());
    msg.mutable_internal_update_machine_prize()->set_income(pmachine->machine().m_base_info().total_income());
    MessageBase::SendMsgToInternal(msg);

    // 结果
    uint32 rate = DataCommon::GetPrizeRate(pmachine->machine(), info.cost(), info.prizes(prize_index).rate(),
        ConfigPrize::Instance()->GetPrize(info.prizes(prize_index).prize_id())->value,
        DataCommon::GetPromoterChargeRate(psession->GetDBData(1)));
    uint32 rand_num = rand() % 100000;
    bool can_get_prize = false;
    if (rand_num < rate) can_get_prize = true;
    pmachine->set_can_get_prize(can_get_prize);

    CSRspRetryLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_retry_lipstick();
    response.set_retry_time(pmachine->retry_time());
    response.set_can_get_prize(can_get_prize);
    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机游戏结束
ENHandlerResult HandlerEndGame::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(pmachine->player_id(), PBDBKey::kPrizeInfo);
    psession->AddQueryKey(pmachine->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerEndGame::OnQuerySuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 礼品不存在
    int32 prize_index = -1;
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (pmachine->prize_id() == info.prizes(i).prize_id())
        {
            prize_index = i;
            break;
        }
    }
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pmachine->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    const CSReqEndLipstick& request = psession->GetCSMsg().cs_req_end_lipstick();
    // 未中奖
    if (!request.get_prize())
    {
        // 保底购买相关
        pmachine->set_total_fail(pmachine->total_fail() + 1);
        pmachine->set_total_time(pmachine->total_time() + GetTime() - pmachine->update_time());

        auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
        // 抽奖
        if (info.lottery_switch() &&
            DataCommon::GetLotteryRate(pmachine->machine(), info.prizes(prize_index).rate(),
                DataCommon::GetPromoterChargeRate(psession->GetDBData(1))) > 100)
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
            uint32 timeout = info.lottery_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_lottery_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
            pmachine->set_game_state(EN_State_Wait_Lottery);

            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(2);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
        // 保底购买
        else if (info.buy_switch() && pprize &&
            pmachine->total_consume() >= info.buy_consume() &&
            pmachine->total_fail() >= info.buy_fail() &&
            pmachine->total_time() >= info.buy_time())
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
            uint32 timeout = info.buy_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_buy_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Buy);

            uint64 cost = (pprize->value > pmachine->total_consume()) ? (pprize->value - pmachine->total_consume()) : 0;
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(3);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_buy_cost(cost);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_prize_id(pmachine->prize_id());
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
        // 下一局
        else
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
            uint32 timeout = info.start_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Start);

            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(1);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
    }
    // 服务器不让中奖
    else if (!pmachine->can_get_prize())
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
        uint32 timeout = info.start_timeout();
        PBInternalMsg msg;
        msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pmachine->set_game_state(EN_State_Wait_Start);

        psession->GetCSResponse().set_result(EN_Result_Sys_Err);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(1);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    // 中奖了
    else
    {
        DataCommon::PlayerAddPrize(psession, psession->GetDBData(), EN_Acc_Lip_Win, pmachine->prize_id(), 1);
        return EN_Handler_Save;
    }
}

ENHandlerResult HandlerEndGame::OnSaveSuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = info.start_timeout() + info.game_succ_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
    pmachine->set_game_state(EN_State_Wait_Start);

    // 保底购买相关
    pmachine->clear_total_fail();
    pmachine->clear_total_time();

    psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(1);
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(info.start_timeout());
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_game_succ_timeout(info.game_succ_timeout());
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_prize_id(pmachine->prize_id());
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    MessageBase::SendMsgToClient(psession);

    // 通知web
    static char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "sh ../../../web_script/curl_wechat.sh %lu %u %lu %d", pmachine->player_id(),
        EN_Award_Prize, pmachine->prize_id(), 1);
    dan::system(cmd);

    // 算法
    auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
    if (pprize)
    {
        DBMBaseInfo& info = *pmachine->mutable_machine()->mutable_m_base_info();
        info.set_total_prize_value(info.total_prize_value() + pprize->value);
        info.set_total_prize_num(info.total_prize_num() + 1);
        msg.mutable_internal_update_machine_prize()->set_machine_id(pmachine->machine_id());
        msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
        msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
        MessageBase::SendMsgToInternal(msg);
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 请求抽奖
ENHandlerResult HandlerLottery::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (!pmachine->machine().m_lipstick_info().lottery_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pmachine->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 出奖
    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery();
    uint32 rand_num = rand() % 10000;
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    for (auto i = 0; i < info.lottery_awards_size(); ++i)
    {
        const DBLotteryAward& award = info.lottery_awards(i);
        if (rand_num < award.sum_rate())
        {
            response.mutable_awards()->CopyFrom(award.awards());
            response.set_award_index(i);
            break;
        }
    }
    if (response.awards_size() == 0)
    {
        PBAward* paward = response.add_awards();
        paward->set_type(EN_Award_None);
        response.set_award_index(-1);
    }

    // 保存中奖
    pmachine->mutable_lottery_awards()->CopyFrom(response.awards());

    // 未中奖
    if (response.awards(0).type() == EN_Award_None)
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
        uint32 timeout = 10;
        PBInternalMsg msg;
        msg.mutable_internal_lottery_end_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
        pmachine->set_game_state(EN_State_Wait_Lottery_End);

        response.set_timeout(timeout);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    // 发奖
    else
    {
        psession->AddQueryKey(pmachine->player_id(), PBDBKey::kGoldInfo);
        psession->AddQueryKey(pmachine->player_id(), PBDBKey::kPrizeInfo);
        return EN_Handler_Query;
    }
}

ENHandlerResult HandlerLottery::OnQuerySuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery();
    DataCommon::PlayerAddAwards(psession, psession->GetDBData(), EN_Acc_Lip_Lottery, response.awards());
    response.set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    return EN_Handler_Save;
}

ENHandlerResult HandlerLottery::OnSaveSuccess(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = 10;
    PBInternalMsg msg;
    msg.mutable_internal_lottery_end_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
    pmachine->set_game_state(EN_State_Wait_Lottery_End);

    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery();
    response.set_timeout(timeout);
    MessageBase::SendMsgToClient(psession);

    // 通知web
    static char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "sh ../../../web_script/curl_wechat.sh %lu %u %lu %lu", pmachine->player_id(),
        response.awards(0).type(), response.awards(0).id(), response.awards(0).giving_num());
    dan::system(cmd);

    // 算法
    if (response.awards(0).type() == EN_Award_Prize)
    {
        auto pprize = ConfigPrize::Instance()->GetPrize(response.awards(0).id());
        if (pprize)
        {
            DBMBaseInfo& info = *pmachine->mutable_machine()->mutable_m_base_info();
            info.set_total_prize_value(info.total_prize_value() + pprize->value);
            info.set_total_prize_num(info.total_prize_num() + 1);
            msg.mutable_internal_update_machine_prize()->set_machine_id(pmachine->machine_id());
            msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
            msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
            MessageBase::SendMsgToInternal(msg);
        }
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 抽奖结束
ENHandlerResult HandlerLotteryEnd::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_Lottery_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_end();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = (pmachine->lottery_awards(0).type() == EN_Award_Prize) ? info.lottery_ani_timeout() : 10;
    PBInternalMsg msg;
    msg.mutable_internal_lottery_ani_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
    pmachine->set_game_state(EN_State_Wait_Lottery_Ani);

    psession->GetCSResponse().mutable_cs_rsp_lottery_end()->set_timeout(timeout);
    MessageBase::SendMsgToClient(psession);

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 保底购买
ENHandlerResult HandlerBuyPrize::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (!pmachine->machine().m_lipstick_info().buy_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pmachine->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    const CSReqBuyPrize& request = psession->GetCSMsg().cs_req_buy_prize();
    // 不买，下一轮游戏
    if (!request.buy())
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
        uint32 timeout = pmachine->machine().m_lipstick_info().start_timeout();
        PBInternalMsg msg;
        msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pmachine->set_game_state(EN_State_Wait_Start);

        psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_next_step(1);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    else
    {
        psession->AddQueryKey(pmachine->player_id(), PBDBKey::kBaseInfo);
        psession->AddQueryKey(pmachine->player_id(), PBDBKey::kGoldInfo);
        psession->AddQueryKey(pmachine->player_id(), PBDBKey::kPrizeInfo);
        return EN_Handler_Query;
    }
}

ENHandlerResult HandlerBuyPrize::OnQuerySuccess(Session * psession)
{
    // 状态不对
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pmachine->RefreshTimer();

    // 金币不够
    auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
    if (!pprize)
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    uint64 cost = (pprize->value > pmachine->total_consume()) ? (pprize->value - pmachine->total_consume()) : 0;
    if (DataCommon::GetPlayerBuyIn(psession->GetDBData(), pmachine->machine_id()) < cost)
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    if (cost)
    {
        DataCommon::PlayerSubBuyIn(psession, psession->GetDBData(), EN_Acc_Lip_Retry, cost, pmachine->machine_id());
    }
    DataCommon::PlayerAddPrize(psession, psession->GetDBData(), EN_Acc_Lip_Buy, pmachine->prize_id(), 1);

    return EN_Handler_Save;
}

ENHandlerResult HandlerBuyPrize::OnSaveSuccess(Session * psession)
{
    // 状态不对
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    uint32 timeout = pmachine->machine().m_lipstick_info().buy_ani_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_buy_ani_timeout()->set_uid(pmachine->machine_id());
    pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
    pmachine->set_game_state(EN_State_Wait_Buy_Ani);

    // 保底购买相关
    pmachine->clear_total_fail();
    pmachine->clear_total_time();
    auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
    if (pprize)
    {
        // 算法
        uint64 cost = (pprize->value > pmachine->total_consume()) ? (pprize->value - pmachine->total_consume()) : 0;
        DBMBaseInfo& info = *pmachine->mutable_machine()->mutable_m_base_info();
        if (cost)
        {
            info.set_total_income(cost + info.total_income());
            msg.mutable_internal_update_machine_prize()->set_income(info.total_income());
        }
        info.set_total_prize_value(info.total_prize_value() + pprize->value);
        info.set_total_prize_num(info.total_prize_num() + 1);
        msg.mutable_internal_update_machine_prize()->set_machine_id(pmachine->machine_id());
        msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
        msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
        MessageBase::SendMsgToInternal(msg);

        uint64 sub = (pprize->value > pmachine->total_consume()) ? pmachine->total_consume() : pprize->value;
        pmachine->set_total_consume(pmachine->total_consume() - sub);
    }

    // 回复客户端
    psession->GetCSResponse().mutable_cs_rsp_buy_prize()->set_prize_id(pmachine->prize_id());
    psession->GetCSResponse().mutable_cs_rsp_buy_prize()->set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    psession->GetCSResponse().mutable_cs_rsp_buy_prize()->set_left_buy_in(DataCommon::GetPlayerBuyIn(psession->GetDBData(), pmachine->machine_id()));
    psession->GetCSResponse().mutable_cs_rsp_buy_prize()->set_timeout(timeout);
    MessageBase::SendMsgToClient(psession);

    // 通知web
    static char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "sh ../../../web_script/curl_wechat.sh %lu %u %lu %d", pmachine->player_id(),
        EN_Award_Prize, pmachine->prize_id(), 1);
    dan::system(cmd);

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机用户登出
ENHandlerResult HandlerPlayerLogout::OnMessage(Session * psession)
{
    Machine* pmachine = MachineManager::Instance()->GetMachine(psession->uid());
    if (pmachine->game_state() == EN_State_Wait_End || pmachine->game_state() == EN_State_Wait_Player)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_player_logout();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
    pmachine->set_game_state(EN_State_Wait_Player);

    psession->GetCSResponse().mutable_cs_rsp_player_logout();
    MessageBase::SendMsgToClient(psession);

    PBInternalMsg msg;
    msg.mutable_internal_clear_buy_in()->set_player_id(pmachine->player_id());
    msg.mutable_internal_clear_buy_in()->set_machine_id(pmachine->machine_id());
    MessageBase::SendMsgToInternal(msg);

    return EN_Handler_Done;
}

