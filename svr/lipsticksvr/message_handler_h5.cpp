#include "message_handler_h5.h"
#include "player_manager.h"
#include "timer_manager.h"
#include "config.h"
#include "h5_machine_manager.h"
#include "message.h"

///////////////////////////////////////////////////////////////////////////////
// 街机登录游戏
ENHandlerResult HandlerLoginGameH5::OnMessage(Session * psession)
{
    // 分配街机id
    int64 machine_id = H5MachineManager::Instance()->Alloc();
    if (machine_id == -1)
    {
        psession->GetCSResponse().set_result(EN_Result_Server_Full);
        psession->GetCSResponse().mutable_cs_rsp_login_game_h5();
        MessageBase::SendMsgToClient(psession, true);
        return EN_Handler_Done;
    }

    // 获取玩家数据，街机算法数据
    psession->AddQueryKey(psession->uid(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kPrizeInfo);
    psession->AddQueryKey(machine_id, PBDBKey::kMBaseInfo);

    psession->set_logic_state(state_wait_player_data);
    return EN_Handler_Query;
}

ENHandlerResult HandlerLoginGameH5::OnQuerySuccess(Session * psession)
{
    if (psession->logic_state() == state_wait_player_data)
    {
        // 获取街机数据，在推广员数据中
        psession->AddQueryKey(psession->GetDBData().base_info().promoter_id(), PBDBKey::kMLipstickInfo);

        psession->set_logic_state(state_wait_promoter_data);
        return EN_Handler_Query;
    }
    else if (psession->logic_state() == state_wait_promoter_data)
    {
        // 顶号处理，在别处已经登录
        PBDBData& player = psession->GetDBData();
        const DBBaseInfo& base_info = player.base_info();
        if (base_info.has_gamesvr_type())
        {
            // 在街机或者其他游戏上登录
            if (base_info.has_machine_id() || base_info.gamesvr_type() != MessageBase::GetSvrType() ||
                base_info.gamesvr_id() != MessageBase::GetSvrID())
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

        // 保存登录态
        player.mutable_base_info()->set_gamesvr_type(MessageBase::GetSvrType());
        player.mutable_base_info()->set_gamesvr_id(MessageBase::GetSvrID());

        return EN_Handler_Save;
    }
    else
    {
        return EN_Handler_Done;
    }
}

ENHandlerResult HandlerLoginGameH5::OnSaveSuccess(Session * psession)
{
    const PBDBData& machine = psession->GetDBData(2);

    CSRspLoginGame& response = *psession->GetCSResponse().mutable_cs_rsp_login_game_h5();
    response.set_heart_beat_interval(MessageBase::GetSvrConf().heart_beat_interval());
    response.set_gold(psession->GetDBData().gold_info().gold());
    response.set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));

    const DBMLipstickInfo& info = machine.m_lipstick_info();
    response.mutable_lipsitck_info()->set_max_retry_time(info.max_retry_time());
    for (auto i = 0; i < info.lottery_awards_size(); ++i)
    {
        response.mutable_lipsitck_info()->add_lotterys()->mutable_awards()->CopyFrom(info.lottery_awards(i).awards());
    }

    // 保存状态
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    pplayer->mutable_machine()->CopyFrom(machine);                                                      // 从推广员设置街机配置
    pplayer->mutable_machine()->set_uid(psession->GetDBData(1).uid());                                  // 设置街机id
    pplayer->mutable_machine()->mutable_m_base_info()->CopyFrom(psession->GetDBData(1).m_base_info());  // 设置算法数据
    pplayer->mutable_machine()->mutable_m_base_info()->set_promoter_id(psession->GetDBData().base_info().promoter_id()); // 推广员id
    pplayer->set_game_state(EN_State_Wait_Start);
    pplayer->clear_total_consume();
    pplayer->clear_total_fail();
    pplayer->clear_total_time();

    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机开始游戏
ENHandlerResult HandlerStartGameH5::OnMessage(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    psession->AddQueryKey(psession->uid(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(pplayer->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerStartGameH5::OnQuerySuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    // 机器状态不对
    if (pplayer->game_state() != EN_State_Wait_Start)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 礼品不存在
    int32 prize_index = -1;
    const CSReqStartLipstick& request = psession->GetCSMsg().cs_req_start_lipstick_h5();
    const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
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
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 金币不够
    PBDBData& player = psession->GetDBData();
    if (player.gold_info().gold() < info.prizes(prize_index).cost())
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 扣金币
    uint64 left = DataCommon::PlayerSubGold(psession, player, EN_Acc_Lip_Play, info.prizes(prize_index).cost());

    // 回复
    CSRspStartLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
    response.set_left_buy_in(left);
    response.set_retry_timeout(info.retry_timeout());
    response.set_retry_switch(info.retry_switch());
    response.mutable_round_timeouts()->CopyFrom(info.round_timeouts());
    response.mutable_round_knives()->CopyFrom(info.round_knives());

    return EN_Handler_Save;
}

ENHandlerResult HandlerStartGameH5::OnSaveSuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    // 机器状态不对
    if (pplayer->game_state() != EN_State_Wait_Start)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 礼品不存在
    int32 prize_index = -1;
    const CSReqStartLipstick& request = psession->GetCSMsg().cs_req_start_lipstick_h5();
    const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
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
        psession->GetCSResponse().mutable_cs_rsp_start_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    pplayer->set_game_state(EN_State_Wait_End);
    uint32 timeout = DataCommon::SumRepeated32(info.round_timeouts()) + info.retry_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_end_timeout()->set_uid(pplayer->player_id());
    pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
    pplayer->clear_pre_end();

    // 保底购买相关
    pplayer->set_total_consume(pplayer->total_consume() + info.prizes(prize_index).cost());
    pplayer->set_update_time(GetTime());
    pplayer->clear_retry_time();

    // 算法
    pplayer->mutable_machine()->mutable_m_base_info()->set_total_income(info.prizes(prize_index).cost() + pplayer->machine().m_base_info().total_income());
    msg.mutable_internal_update_machine_prize()->set_machine_id(pplayer->machine().uid());
    msg.mutable_internal_update_machine_prize()->set_income(pplayer->machine().m_base_info().total_income());
    MessageBase::SendMsgToInternal(msg);

    // 结果
    uint32 rate = DataCommon::GetPrizeRate(pplayer->machine(), info.prizes(prize_index).cost(), info.prizes(prize_index).rate(),
        ConfigPrize::Instance()->GetPrize(info.prizes(prize_index).prize_id())->value,
        DataCommon::GetPromoterChargeRate(psession->GetDBData(1)));
    uint32 rand_num = rand() % 100000;
    bool can_get_prize = false;
    if (rand_num < rate) can_get_prize = true;
    pplayer->set_can_get_prize(can_get_prize);
    pplayer->set_prize_id(request.prize_id());

    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机游戏续命
ENHandlerResult HandlerRetryGameH5::OnMessage(Session * psession)
{
    // 街机状态不对
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (!pplayer->machine().m_lipstick_info().retry_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 参数非法
    const CSReqRetryLipstick& request = psession->GetCSMsg().cs_req_retry_lipstick_h5();
    const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
    if (request.round() == 0 || request.round() > (uint32)info.round_timeouts_size())
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Params);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 最大复活次数
    if (pplayer->retry_time() >= info.max_retry_time())
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Req);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    psession->AddQueryKey(psession->uid(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(pplayer->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerRetryGameH5::OnQuerySuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 礼品不存在
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
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pplayer->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 金币不够
    PBDBData& player = psession->GetDBData();
    if (player.gold_info().gold() < info.prizes(prize_index).cost())
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 扣金币
    uint64 left = DataCommon::PlayerSubGold(psession, player, EN_Acc_Lip_Retry, info.prizes(prize_index).cost());

    // 回复
    CSRspRetryLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
    response.set_left_buy_in(left);

    return EN_Handler_Save;
}

ENHandlerResult HandlerRetryGameH5::OnSaveSuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 礼品不存在
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
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pplayer->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    const CSReqRetryLipstick& request = psession->GetCSMsg().cs_req_retry_lipstick_h5();
    TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
    uint32 timeout = DataCommon::SumRepeated32(info.round_timeouts(), request.round() - 1) + info.retry_timeout();
    PBInternalMsg msg;
    msg.mutable_internal_end_timeout()->set_uid(pplayer->player_id());
    pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
    pplayer->clear_pre_end();

    // 保底购买相关
    pplayer->set_total_consume(pplayer->total_consume() + info.prizes(prize_index).cost());
    pplayer->set_total_fail(pplayer->total_fail() + 1);
    pplayer->set_total_time(pplayer->total_time() + GetTime() - pplayer->update_time());
    pplayer->set_update_time(GetTime());
    pplayer->set_retry_time(pplayer->retry_time() + 1);

    // 算法
    pplayer->mutable_machine()->mutable_m_base_info()->set_total_income(info.prizes(prize_index).cost() + pplayer->machine().m_base_info().total_income());
    msg.mutable_internal_update_machine_prize()->set_machine_id(pplayer->machine().uid());
    msg.mutable_internal_update_machine_prize()->set_income(pplayer->machine().m_base_info().total_income());
    MessageBase::SendMsgToInternal(msg);

    // 结果
    uint32 rate = DataCommon::GetPrizeRate(pplayer->machine(), info.prizes(prize_index).cost(), info.prizes(prize_index).rate(),
        ConfigPrize::Instance()->GetPrize(info.prizes(prize_index).prize_id())->value,
        DataCommon::GetPromoterChargeRate(psession->GetDBData(1)));
    uint32 rand_num = rand() % 100000;
    bool can_get_prize = false;
    if (rand_num < rate) can_get_prize = true;
    pplayer->set_can_get_prize(can_get_prize);

    CSRspRetryLipstick& response = *psession->GetCSResponse().mutable_cs_rsp_retry_lipstick_h5();
    response.set_retry_time(pplayer->retry_time());
    response.set_can_get_prize(can_get_prize);
    MessageBase::SendMsgToClient(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 街机游戏结束
ENHandlerResult HandlerEndGameH5::OnMessage(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
    psession->AddQueryKey(psession->uid(), PBDBKey::kPrizeInfo);
    psession->AddQueryKey(pplayer->machine().m_base_info().promoter_id(), PBDBKey::kPBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerEndGameH5::OnQuerySuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 礼品不存在
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
    if (prize_index == -1 || !ConfigPrize::Instance()->IsExist(pplayer->prize_id()))
    {
        psession->GetCSResponse().set_result(EN_Result_Invalid_Prize_Id);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    const CSReqEndLipstick& request = psession->GetCSMsg().cs_req_end_lipstick_h5();
    // 未中奖
    if (!request.get_prize())
    {
        // 更新游戏记录
        PBInternalMsg msg;
        msg.mutable_internal_game_record()->set_player_id(pplayer->player_id());
        msg.mutable_internal_game_record()->set_prize_id(pplayer->prize_id());
        msg.mutable_internal_game_record()->set_get_prize(request.get_prize());
        MessageBase::SendMsgToInternal(msg);

        auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
        // 抽奖
        if (info.lottery_switch() &&
            DataCommon::GetLotteryRate(pplayer->machine(), info.prizes(prize_index).rate(),
                DataCommon::GetPromoterChargeRate(psession->GetDBData(1))) > 100)
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
            uint32 timeout = pplayer->machine().m_lipstick_info().lottery_timeout();
            msg.mutable_internal_lottery_timeout()->set_uid(pplayer->player_id());
            pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pplayer->set_game_state(EN_State_Wait_Lottery);

            // 保底购买相关
            pplayer->set_total_fail(pplayer->total_fail() + 1);
            pplayer->set_total_time(pplayer->total_time() + GetTime() - pplayer->update_time());

            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(2);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_timeout(timeout);
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
        // 保底购买
        else if (info.buy_switch() && pprize &&
            pplayer->total_consume() >= info.buy_consume() &&
            pplayer->total_fail() >= info.buy_fail() &&
            pplayer->total_time() >= info.buy_time())
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
            uint32 timeout = pplayer->machine().m_lipstick_info().buy_timeout();
            msg.mutable_internal_buy_timeout()->set_uid(pplayer->player_id());
            pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pplayer->set_game_state(EN_State_Wait_Buy);

            // 通知保底
            uint64 cost = (pprize->value > pplayer->total_consume()) ? (pprize->value - pplayer->total_consume()) : 0;
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(3);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_timeout(timeout);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_buy_cost(cost);
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_prize_id(pplayer->prize_id());
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
        // 下一局
        else
        {
            // 重置定时器
            TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
            pplayer->set_game_state(EN_State_Wait_Start);

            // 通知下一局
            psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
            MessageBase::SendMsgToClient(psession);
            return EN_Handler_Done;
        }
    }
    // 服务器不让中奖
    else if (!pplayer->can_get_prize())
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
        pplayer->set_game_state(EN_State_Wait_Start);

        psession->GetCSResponse().set_result(EN_Result_Sys_Err);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    // 中奖了
    else
    {
        DataCommon::PlayerAddPrize(psession, psession->GetDBData(), EN_Acc_Lip_Win, pplayer->prize_id(), 1);
        return EN_Handler_Save;
    }
}

ENHandlerResult HandlerEndGameH5::OnSaveSuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
    pplayer->set_game_state(EN_State_Wait_Start);

    // 保底购买相关
    pplayer->clear_total_fail();
    pplayer->clear_total_time();

    psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_prize_id(pplayer->prize_id());
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    MessageBase::SendMsgToClient(psession);

    // 更新游戏记录
    const CSReqEndLipstick& request = psession->GetCSMsg().cs_req_end_lipstick_h5();
    PBInternalMsg msg;
    msg.mutable_internal_game_record()->set_player_id(pplayer->player_id());
    msg.mutable_internal_game_record()->set_prize_id(pplayer->prize_id());
    msg.mutable_internal_game_record()->set_get_prize(request.get_prize());
    MessageBase::SendMsgToInternal(msg);

    // 算法
    auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
    if (pprize)
    {
        DBMBaseInfo& info = *pplayer->mutable_machine()->mutable_m_base_info();
        info.set_total_prize_value(info.total_prize_value() + pprize->value);
        info.set_total_prize_num(info.total_prize_num() + 1);
        msg.mutable_internal_update_machine_prize()->set_machine_id(pplayer->machine().uid());
        msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
        msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
        MessageBase::SendMsgToInternal(msg);
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 请求抽奖
ENHandlerResult HandlerLotteryH5::OnMessage(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (!pplayer->machine().m_lipstick_info().lottery_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pplayer->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 出奖
    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
    uint32 rand_num = rand() % 10000;
    const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
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

    // 未中奖
    if (response.awards(0).type() == EN_Award_None)
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
        uint32 timeout = 10;
        PBInternalMsg msg;
        msg.mutable_internal_lottery_end_timeout()->set_uid(pplayer->player_id());
        pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
        pplayer->set_game_state(EN_State_Wait_Lottery_End);

        response.set_timeout(timeout);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    // 发奖
    else
    {
        psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
        psession->AddQueryKey(psession->uid(), PBDBKey::kPrizeInfo);
        return EN_Handler_Query;
    }
}

ENHandlerResult HandlerLotteryH5::OnQuerySuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
    DataCommon::PlayerAddAwards(psession, psession->GetDBData(), EN_Acc_Lip_Lottery, response.awards());
    response.set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    return EN_Handler_Save;
}

ENHandlerResult HandlerLotteryH5::OnSaveSuccess(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_Lottery)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
    uint32 timeout = 10;
    PBInternalMsg msg;
    msg.mutable_internal_lottery_end_timeout()->set_uid(pplayer->player_id());
    pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout * 2, msg));
    pplayer->set_game_state(EN_State_Wait_Lottery_End);

    CSRspLottery& response = *psession->GetCSResponse().mutable_cs_rsp_lottery_h5();
    response.set_timeout(timeout);
    MessageBase::SendMsgToClient(psession);

    // 算法
    if (response.awards(0).type() == EN_Award_Prize)
    {
        auto pprize = ConfigPrize::Instance()->GetPrize(response.awards(0).id());
        if (pprize)
        {
            DBMBaseInfo& info = *pplayer->mutable_machine()->mutable_m_base_info();
            info.set_total_prize_value(info.total_prize_value() + pprize->value);
            info.set_total_prize_num(info.total_prize_num() + 1);
            PBInternalMsg msg;
            msg.mutable_internal_update_machine_prize()->set_machine_id(pplayer->machine().uid());
            msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
            msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
            MessageBase::SendMsgToInternal(msg);
        }
    }

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 抽奖结束
ENHandlerResult HandlerLotteryEndH5::OnMessage(Session * psession)
{
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_Lottery_End)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_lottery_end_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
    // 保底购买
    auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
    if (info.buy_switch() && pprize &&
        pplayer->total_consume() >= info.buy_consume() &&
        pplayer->total_fail() >= info.buy_fail() &&
        pplayer->total_time() >= info.buy_time())
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
        uint32 timeout = pplayer->machine().m_lipstick_info().buy_timeout();
        PBInternalMsg msg;
        msg.mutable_internal_buy_timeout()->set_uid(pplayer->player_id());
        pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pplayer->set_game_state(EN_State_Wait_Buy);

        // 回复
        MessageBase::SendMsgToClient(psession);

        // 通知保底
        uint64 cost = (pprize->value > pplayer->total_consume()) ? (pprize->value - pplayer->total_consume()) : 0;
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(3);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_timeout(timeout);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_buy_cost(cost);
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_prize_id(pplayer->prize_id());
        MessageBase::SendMsgToClient(psession);
    }
    // 下一局
    else
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
        pplayer->set_game_state(EN_State_Wait_Start);

        // 回复
        MessageBase::SendMsgToClient(psession);

        // 通知下一局
        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
        MessageBase::SendMsgToClient(psession);
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 保底购买
ENHandlerResult HandlerBuyPrizeH5::OnMessage(Session * psession)
{
    // 状态不对
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (!pplayer->machine().m_lipstick_info().buy_switch())
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    if (pplayer->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    const CSReqBuyPrize& request = psession->GetCSMsg().cs_req_buy_prize_h5();
    // 不买，下一轮游戏
    if (!request.buy())
    {
        // 重置定时器
        TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
        pplayer->set_game_state(EN_State_Wait_Start);

        psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    else
    {
        psession->AddQueryKey(psession->uid(), PBDBKey::kBaseInfo);
        psession->AddQueryKey(psession->uid(), PBDBKey::kGoldInfo);
        psession->AddQueryKey(psession->uid(), PBDBKey::kPrizeInfo);
        return EN_Handler_Query;
    }
}

ENHandlerResult HandlerBuyPrizeH5::OnQuerySuccess(Session * psession)
{
    // 状态不对
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    pplayer->RefreshTimer();

    // 金币不够
    auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
    if (!pprize)
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }
    uint64 cost = (pprize->value > pplayer->total_consume()) ? (pprize->value - pplayer->total_consume()) : 0;
    if (psession->GetDBData().gold_info().gold() < cost)
    {
        psession->GetCSResponse().set_result(EN_Result_Gold_Not_Enough);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    DataCommon::PlayerSubGold(psession, psession->GetDBData(), EN_Acc_Lip_Buy, cost);
    DataCommon::PlayerAddPrize(psession, psession->GetDBData(), EN_Acc_Lip_Buy, pplayer->prize_id(), 1);

    return EN_Handler_Save;
}

ENHandlerResult HandlerBuyPrizeH5::OnSaveSuccess(Session * psession)
{
    // 状态不对
    Player* pplayer = PlayerManager::Instance()->GetPlayer(psession->uid());
    if (pplayer->game_state() != EN_State_Wait_Buy)
    {
        psession->GetCSResponse().set_result(EN_Result_Machine_State_Diff);
        psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5();
        MessageBase::SendMsgToClient(psession);
        return EN_Handler_Done;
    }

    // 回复客户端
    psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5()->set_prize_id(pplayer->prize_id());
    psession->GetCSResponse().mutable_cs_rsp_buy_prize_h5()->set_prize_num(DataCommon::GetPlayerPrizeNum(psession->GetDBData()));
    MessageBase::SendMsgToClient(psession);

    // 重置定时器
    TimerManager::Instance()->ReleaseTimer(pplayer->timer_id());
    pplayer->set_game_state(EN_State_Wait_Start);

    // 保底购买相关
    pplayer->clear_total_fail();
    pplayer->clear_total_time();
    auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
    if (pprize)
    {
        // 算法
        PBInternalMsg msg;
        uint64 cost = (pprize->value > pplayer->total_consume()) ? (pprize->value - pplayer->total_consume()) : 0;
        DBMBaseInfo& info = *pplayer->mutable_machine()->mutable_m_base_info();
        if (cost)
        {
            info.set_total_income(cost + info.total_income());
            msg.mutable_internal_update_machine_prize()->set_income(info.total_income());
        }
        info.set_total_prize_value(info.total_prize_value() + pprize->value);
        info.set_total_prize_num(info.total_prize_num() + 1);
        msg.mutable_internal_update_machine_prize()->set_machine_id(pplayer->machine().uid());
        msg.mutable_internal_update_machine_prize()->set_value(info.total_prize_value());
        msg.mutable_internal_update_machine_prize()->set_num(info.total_prize_num());
        MessageBase::SendMsgToInternal(msg);

        uint64 sub = (pprize->value > pplayer->total_consume()) ? pplayer->total_consume() : pprize->value;
        pplayer->set_total_consume(pplayer->total_consume() - sub);
    }

    // 下一局
    psession->GetCSResponse().mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
    MessageBase::SendMsgToClient(psession);

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 用户登出
ENHandlerResult HandlerPlayerLogoutH5::OnMessage(Session * psession)
{
    Message::LogoutPlayer(psession->uid());

    psession->GetCSResponse().mutable_cs_rsp_player_logout_h5();
    MessageBase::SendMsgToClient(psession);

    return EN_Handler_Done;
}


