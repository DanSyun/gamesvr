#include "message_handler_internal.h"
#include "machine_manager.h"
#include "player_manager.h"
#include "config.h"


///////////////////////////////////////////////////////////////////////////////
// 街机登出
ENHandlerResult HandlerLogout::OnMessage(Session * psession)
{
    uint64 uid = psession->GetInternalMsg().internal_logout().uid();
    if (uid < 10000000)
    {
        psession->AddQueryKey(uid, PBDBKey::kMBaseInfo);
        return EN_Handler_Query;
    }
    else
    {
        psession->AddQueryKey(uid, PBDBKey::kBaseInfo);
        return EN_Handler_Query;
    }
}

ENHandlerResult HandlerLogout::OnQuerySuccess(Session * psession)
{
    uint64 uid = psession->GetInternalMsg().internal_logout().uid();
    if (uid < 10000000)
    {
        PBDBData& machine = psession->GetDBData();
        machine.mutable_m_base_info()->clear_gamesvr_id();
        machine.mutable_m_base_info()->clear_gamesvr_type();
        return EN_Handler_Save;
    }
    else
    {
        PBDBData& player = psession->GetDBData();
        player.mutable_base_info()->clear_gamesvr_type();
        player.mutable_base_info()->clear_gamesvr_id();
        player.mutable_base_info()->clear_machine_id();
        return EN_Handler_Save;
    }
}

ENHandlerResult HandlerLogout::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 投币清除
ENHandlerResult HandlerClearBuyIn::OnMessage(Session * psession)
{
    psession->AddQueryKey(psession->GetInternalMsg().internal_clear_buy_in().player_id(), PBDBKey::kGoldInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerClearBuyIn::OnQuerySuccess(Session * psession)
{
    const InternalClearBuyIn& request = psession->GetInternalMsg().internal_clear_buy_in();
    if (DataCommon::PlayerClearBuyIn(psession, psession->GetDBData(), request.machine_id()))
    {
        return EN_Handler_Save;
    }
    else
    {
        return EN_Handler_Done;
    }
}

ENHandlerResult HandlerClearBuyIn::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 游戏开始超时
ENHandlerResult HandlerStartTimeout::OnMessage(Session * psession)
{
    uint32 machine_id = psession->GetInternalMsg().internal_start_timeout().uid();
    Machine* pmachine = MachineManager::Instance()->GetMachine(machine_id);
    if (!pmachine) return EN_Handler_Done;
    if (pmachine->game_state() != EN_State_Wait_Start) return EN_Handler_Done;

    pmachine->clear_timer_id();
    pmachine->set_game_state(EN_State_Wait_Player);

    PBCSMsg cs_msg;
    cs_msg.mutable_cs_notify_player_logout();
    MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);

    PBInternalMsg msg;
    msg.mutable_internal_clear_buy_in()->set_player_id(pmachine->player_id());
    msg.mutable_internal_clear_buy_in()->set_machine_id(pmachine->machine_id());
    MessageBase::SendMsgToInternal(msg);

    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 游戏结束超时
ENHandlerResult HandlerEndTimeout::OnMessage(Session * psession)
{
    uint64 uid = psession->GetInternalMsg().internal_end_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_End) return EN_Handler_Done;

        pmachine->clear_timer_id();
        pmachine->set_game_state(EN_State_Wait_Player);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_notify_player_logout();
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);

        // 退款
        PBInternalMsg msg;
        msg.mutable_internal_clear_buy_in()->set_player_id(pmachine->player_id());
        msg.mutable_internal_clear_buy_in()->set_machine_id(pmachine->machine_id());
        MessageBase::SendMsgToInternal(msg);
        // 两个消息留出间隔
        msg.mutable_internal_game_refund()->set_player_id(pmachine->player_id());
        msg.mutable_internal_game_refund()->set_refund_num(pmachine->machine().m_lipstick_info().cost());
        TimerManager::Instance()->CreateTimerMsec(500, msg);

        return EN_Handler_Done;
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(uid);
        if (!pplayer) return EN_Handler_Done;
        if (pplayer->game_state() != EN_State_Wait_End) return EN_Handler_Done;
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
        if (prize_index == -1) return EN_Handler_Done;

        pplayer->clear_timer_id();
        pplayer->set_game_state(EN_State_Wait_Start);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
        MessageBase::SendMsgToClient(pplayer->GetConnectHead(), cs_msg);

        // 退款
        if (!pplayer->pre_end())
        {
            PBInternalMsg msg;
            msg.mutable_internal_game_refund()->set_player_id(pplayer->player_id());
            msg.mutable_internal_game_refund()->set_refund_num(info.prizes(prize_index).cost());
            MessageBase::SendMsgToInternal(msg);
        }

        return EN_Handler_Done;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 抽奖超时
ENHandlerResult HandlerLotteryTimeout::OnMessage(Session * psession)
{
    uint32 uid = psession->GetInternalMsg().internal_lottery_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_Lottery) return EN_Handler_Done;

        // 保底购买
        auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
        const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
        if (info.buy_switch() && pprize &&
            pmachine->total_consume() >= info.buy_consume() &&
            pmachine->total_fail() >= info.buy_fail() &&
            pmachine->total_time() >= info.buy_time())
        {
            uint32 timeout = info.buy_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_buy_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Buy);

            PBCSMsg cs_msg;
            uint64 cost = (pprize->value > pmachine->total_consume()) ? (pprize->value - pmachine->total_consume()) : 0;
            cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(3);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_buy_cost(cost);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_prize_id(pmachine->prize_id());
            MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);
        }
        // 下一局
        else
        {
            uint32 timeout = info.start_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Start);

            PBCSMsg cs_msg;
            cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(1);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);
        }
        return EN_Handler_Done;
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(uid);
        if (!pplayer) return EN_Handler_Done;
        if (pplayer->game_state() != EN_State_Wait_Lottery) return EN_Handler_Done;

        // 保底购买
        auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
        const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
        if (info.buy_switch() && pprize &&
            pplayer->total_consume() >= info.buy_consume() &&
            pplayer->total_fail() >= info.buy_fail() &&
            pplayer->total_time() >= info.buy_time())
        {
            uint32 timeout = info.buy_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_buy_timeout()->set_uid(pplayer->player_id());
            pplayer->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pplayer->set_game_state(EN_State_Wait_Buy);

            PBCSMsg cs_msg;
            uint64 cost = (pprize->value > pplayer->total_consume()) ? (pprize->value - pplayer->total_consume()) : 0;
            cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_next_step(3);
            cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_timeout(timeout);
            cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_buy_cost(cost);
            cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_prize_id(pplayer->prize_id());
            MessageBase::SendMsgToClient(pplayer->GetConnectHead(), cs_msg);
        }
        // 下一局
        else
        {
            pplayer->clear_timer_id();
            pplayer->set_game_state(EN_State_Wait_Start);

            PBCSMsg cs_msg;
            cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
            MessageBase::SendMsgToClient(pplayer->GetConnectHead(), cs_msg);
        }
        return EN_Handler_Done;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 抽奖结束超时
ENHandlerResult HandlerLotteryEndTimeout::OnMessage(Session * psession)
{
    uint32 uid = psession->GetInternalMsg().internal_lottery_end_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_Lottery_End) return EN_Handler_Done;

        const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
        uint32 timeout = (pmachine->lottery_awards(0).type() == EN_Award_Prize) ? info.lottery_ani_timeout() : 10;
        PBInternalMsg msg;
        msg.mutable_internal_lottery_ani_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pmachine->set_game_state(EN_State_Wait_Lottery_Ani);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_lottery_end()->set_timeout(timeout);
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);
        return EN_Handler_Done;
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(uid);
        if (!pplayer) return EN_Handler_Done;
        if (pplayer->game_state() != EN_State_Wait_Lottery_End) return EN_Handler_Done;

        const DBMLipstickInfo& info = pplayer->machine().m_lipstick_info();
        // 保底购买
        auto pprize = ConfigPrize::Instance()->GetPrize(pplayer->prize_id());
        if (info.buy_switch() && pprize &&
            pplayer->total_consume() >= info.buy_consume() &&
            pplayer->total_fail() >= info.buy_fail() &&
            pplayer->total_time() >= info.buy_time())
        {
            // 重置定时器
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
}

///////////////////////////////////////////////////////////////////////////////
// 抽奖动画超时
ENHandlerResult HandlerLotteryAniTimeout::OnMessage(Session * psession)
{
    uint32 uid = psession->GetInternalMsg().internal_lottery_ani_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_Lottery_Ani) return EN_Handler_Done;

        // 保底购买
        auto pprize = ConfigPrize::Instance()->GetPrize(pmachine->prize_id());
        const DBMLipstickInfo& info = pmachine->machine().m_lipstick_info();
        if (info.buy_switch() && pprize &&
            pmachine->total_consume() >= info.buy_consume() &&
            pmachine->total_fail() >= info.buy_fail() &&
            pmachine->total_time() >= info.buy_time())
        {
            uint32 timeout = info.buy_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_buy_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Buy);

            PBCSMsg cs_msg;
            uint64 cost = (pprize->value > pmachine->total_consume()) ? (pprize->value - pmachine->total_consume()) : 0;
            cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(3);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_buy_cost(cost);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_prize_id(pmachine->prize_id());
            MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);
        }
        // 下一局
        else
        {
            uint32 timeout = info.start_timeout();
            PBInternalMsg msg;
            msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
            pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
            pmachine->set_game_state(EN_State_Wait_Start);

            PBCSMsg cs_msg;
            cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(1);
            cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
            MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);
        }
        return EN_Handler_Done;
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 保底购买超时
ENHandlerResult HandlerBuyTimeout::OnMessage(Session * psession)
{
    uint32 uid = psession->GetInternalMsg().internal_buy_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_Buy) return EN_Handler_Done;

        // 下一局
        uint32 timeout = pmachine->machine().m_lipstick_info().start_timeout();
        PBInternalMsg msg;
        msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pmachine->set_game_state(EN_State_Wait_Start);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(1);
        cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);

        return EN_Handler_Done;
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(uid);
        if (!pplayer) return EN_Handler_Done;
        if (pplayer->game_state() != EN_State_Wait_Buy) return EN_Handler_Done;

        // 下一局
        pplayer->clear_timer_id();
        pplayer->set_game_state(EN_State_Wait_Start);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_end_lipstick_h5()->set_next_step(1);
        MessageBase::SendMsgToClient(pplayer->GetConnectHead(), cs_msg);

        return EN_Handler_Done;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 保底购买动画超时
ENHandlerResult HandlerBuyAniTimeout::OnMessage(Session * psession)
{
    uint32 uid = psession->GetInternalMsg().internal_buy_ani_timeout().uid();
    if (uid < 10000000)
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(uid);
        if (!pmachine) return EN_Handler_Done;
        if (pmachine->game_state() != EN_State_Wait_Buy_Ani) return EN_Handler_Done;

        // 下一局
        uint32 timeout = pmachine->machine().m_lipstick_info().start_timeout();
        PBInternalMsg msg;
        msg.mutable_internal_start_timeout()->set_uid(pmachine->machine_id());
        pmachine->set_timer_id(TimerManager::Instance()->CreateTimer(timeout, msg));
        pmachine->set_game_state(EN_State_Wait_Start);

        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_end_lipstick()->set_next_step(1);
        cs_msg.mutable_cs_rsp_end_lipstick()->set_timeout(timeout);
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);

        return EN_Handler_Done;
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 游戏退款
ENHandlerResult HandlerGameRefund::OnMessage(Session * psession)
{
    const InternalGameRefund& request = psession->GetInternalMsg().internal_game_refund();
    psession->AddQueryKey(request.player_id(), PBDBKey::kGoldInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerGameRefund::OnQuerySuccess(Session * psession)
{
    const InternalGameRefund& request = psession->GetInternalMsg().internal_game_refund();
    DataCommon::PlayerAddGold(psession, psession->GetDBData(), EN_Acc_Game_Refund, request.refund_num());
    return EN_Handler_Save;
}

ENHandlerResult HandlerGameRefund::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 增加机器出奖数
ENHandlerResult HandlerUpdateMachinePrize::OnMessage(Session * psession)
{
    const InternalUpdateMachinePirze& request = psession->GetInternalMsg().internal_update_machine_prize();
    psession->AddQueryKey(request.machine_id(), PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerUpdateMachinePrize::OnQuerySuccess(Session * psession)
{
    const InternalUpdateMachinePirze& request = psession->GetInternalMsg().internal_update_machine_prize();
    DBMBaseInfo& info = *psession->GetDBData().mutable_m_base_info();
    if (request.has_value())
    {
        info.set_total_prize_value(request.value());
    }
    if (request.has_num())
    {
        info.set_total_prize_num(request.num());
    }
    if (request.has_value())
    {
        info.set_total_income(request.income());
    }

    return EN_Handler_Save;
}

ENHandlerResult HandlerUpdateMachinePrize::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 玩家游戏记录
ENHandlerResult HandlerGameRecord::OnMessage(Session * psession)
{
    const InternalGameRecord& notify = psession->GetInternalMsg().internal_game_record();
    psession->AddQueryKey(notify.player_id(), PBDBKey::kBaseInfo);
    psession->AddQueryKey(notify.player_id(), PBDBKey::kGameInfo);
    if (notify.get_prize())
        psession->AddQueryKey(EN_Uid_Lip_Prize, PBDBKey::kGLipPrizeInfo);

    return EN_Handler_Query;
}

ENHandlerResult HandlerGameRecord::OnQuerySuccess(Session * psession)
{
    if (psession->process_index() == 0)
    {
        return ProcessPlayer(psession);
    }
    else if (psession->process_index() == 1)
    {
        return ProcessGlobal(psession);
    }
    else
    {
        return EN_Handler_Done;
    }
}

ENHandlerResult HandlerGameRecord::ProcessPlayer(Session * psession)
{
    const InternalGameRecord& notify = psession->GetInternalMsg().internal_game_record();
    DBGameInfo& info = *psession->GetDBData().mutable_game_info();
    bool find = false;
    for (auto i = 0; i < info.records_size(); ++i)
    {
        DBGameRecord& record = *info.mutable_records(i);
        if (record.game_type() == EN_G_Type_Lipstick)
        {
            find = true;
            DBPlay* pplay = record.add_plays();
            pplay->set_prize_id(notify.prize_id());
            pplay->set_get_prize(notify.get_prize());
            pplay->set_timestamp(GetTime());
            if (record.plays_size() > 50)
            {
                record.mutable_plays()->DeleteSubrange(0, record.plays_size() - 50);
            }
        }
    }
    if (!find)
    {
        DBGameRecord* precord = info.add_records();
        precord->set_game_type(EN_G_Type_Lipstick);
        DBPlay* pplay = precord->add_plays();
        pplay->set_prize_id(notify.prize_id());
        pplay->set_get_prize(notify.get_prize());
        pplay->set_timestamp(GetTime());
    }
    return EN_Handler_Save;
}

ENHandlerResult HandlerGameRecord::ProcessGlobal(Session * psession)
{
    DBGLipPrizeInfo& info = *psession->GetDBData().mutable_g_lip_prize_info();
    const InternalGameRecord& notify = psession->GetInternalMsg().internal_game_record();
    auto pprize = ConfigPrize::Instance()->GetPrize(notify.prize_id());
    if (pprize)
    {
        info.set_seq(info.seq() + 1);
        DBPlayerPrize& player_prize = *info.add_player_prizes();
        player_prize.set_player_nick(psession->GetDBData(0).base_info().nick());
        player_prize.set_prize_name(pprize->name);
        player_prize.set_seq(info.seq());
        if (info.player_prizes_size() > 20)
        {
            info.mutable_player_prizes()->DeleteSubrange(0, info.player_prizes_size() - 20);
        }
        return EN_Handler_Save;
    }
    else
    {
        return EN_Handler_Done;
    }
}

ENHandlerResult HandlerGameRecord::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Next;
}

