#include "message_handler_server.h"
#include "machine_manager.h"
#include "player_manager.h"
#include "timer_manager.h"
#include "message.h"
#include "config.h"

///////////////////////////////////////////////////////////////////////////////
// 更新配置
ENHandlerResult HandlerUpdateConfig::OnMessage(Session * psession)
{
    const string& name = psession->GetSSMsg().web_notify_update_config().name();
    ConfigMysql::Instance()->Update(name);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 踢出玩家
ENHandlerResult HandlerKickPlayer::OnMessage(Session * psession)
{
    const SSNotifyKickPlayer& notify = psession->GetSSMsg().ss_notify_kick_player();
    // 在街机登录
    if (notify.has_machine_id())
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(notify.machine_id());
        if (!pmachine)
            return EN_Handler_Done;
        if (pmachine->game_state() == EN_State_Wait_Player || pmachine->player_id() != notify.player_id())
            return EN_Handler_Done;

        // 通知机台
        PBCSMsg cs_msg;
        cs_msg.mutable_cs_rsp_player_logout();
        MessageBase::SendMsgToClient(pmachine->GetConnectHead(), cs_msg);

        // 清除投币
        PBInternalMsg msg;
        msg.mutable_internal_clear_buy_in()->set_player_id(pmachine->player_id());
        msg.mutable_internal_clear_buy_in()->set_machine_id(pmachine->machine_id());
        MessageBase::SendMsgToInternal(msg);
        // 退款
        if (pmachine->game_state() == EN_State_Wait_End)
        {
            // 两个消息留出间隔
            msg.mutable_internal_game_refund()->set_player_id(pmachine->player_id());
            msg.mutable_internal_game_refund()->set_refund_num(pmachine->machine().m_lipstick_info().cost());
            TimerManager::Instance()->CreateTimerMsec(100, msg);
        }

        // 修改状态
        TimerManager::Instance()->ReleaseTimer(pmachine->timer_id());
        pmachine->set_game_state(EN_State_Wait_Player);

        return EN_Handler_Done;
    }
    else
    {
        Message::LogoutPlayer(notify.player_id(), true, EN_Logout_Multi_Login, false);
        return EN_Handler_Done;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 通知玩家金币
ENHandlerResult HandlerPlayerGold::OnMessage(Session * psession)
{
    const SSNotifyPlayerGold& notify = psession->GetSSMsg().ss_notify_player_gold();
    Player* pplayer = PlayerManager::Instance()->GetPlayer(notify.uid());
    if (pplayer)
    {
        PBCSMsg msg;
        msg.mutable_cs_notify_gold()->set_gold(notify.gold());
        MessageBase::SendMsgToClient(pplayer->GetConnectHead(), msg);
    }
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 通知玩家积分
ENHandlerResult HandlerNotifyPoint::OnMessage(Session * psession)
{
    const WebNotifyPoint& notify = psession->GetSSMsg().web_notify_point();
    if (notify.has_machine_id())
    {
        Machine* pmachine = MachineManager::Instance()->GetMachine(notify.machine_id());
        if (pmachine && pmachine->player_id() == notify.uid())
        {
            PBCSMsg msg;
            msg.mutable_cs_notify_point()->set_point(notify.point());
            MessageBase::SendMsgToClient(pmachine->GetConnectHead(), msg);
        }
    }
    else
    {
        Player* pplayer = PlayerManager::Instance()->GetPlayer(notify.uid());
        if (pplayer)
        {
            PBCSMsg msg;
            msg.mutable_cs_notify_point()->set_point(notify.point());
            MessageBase::SendMsgToClient(pplayer->GetConnectHead(), msg);
        }
    }
    return EN_Handler_Done;
}

