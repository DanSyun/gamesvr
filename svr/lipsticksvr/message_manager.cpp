#include "message_manager.h"
#include "message_handler_common.h"
#include "message_handler_h5.h"
#include "message_handler_server.h"
#include "message_handler_internal.h"

void MessageManager::Init()
{
    _handlers[PBCSMsg::kCsReqHeartBeat]             = new HandlerHeartBeat;
    _handlers[PBCSMsg::kCsReqMLoginGame]            = new HandlerMLoginGame;
    _handlers[PBCSMsg::kCsReqStartLipstick]         = new HandlerStartGame;
    _handlers[PBCSMsg::kCsReqRetryLipstick]         = new HandlerRetryGame;
    _handlers[PBCSMsg::kCsReqEndLipstick]           = new HandlerEndGame;
    _handlers[PBCSMsg::kCsReqLottery]               = new HandlerLottery;
    _handlers[PBCSMsg::kCsReqLotteryEnd]            = new HandlerLotteryEnd;
    _handlers[PBCSMsg::kCsReqBuyPrize]              = new HandlerBuyPrize;
    _handlers[PBCSMsg::kCsReqPlayerLogout]          = new HandlerPlayerLogout;

    _handlers[PBCSMsg::kCsReqLoginGameH5]           = new HandlerLoginGameH5;
    _handlers[PBCSMsg::kCsReqStartLipstickH5]       = new HandlerStartGameH5;
    _handlers[PBCSMsg::kCsReqRetryLipstickH5]       = new HandlerRetryGameH5;
    _handlers[PBCSMsg::kCsReqEndLipstickH5]         = new HandlerEndGameH5;
    _handlers[PBCSMsg::kCsReqLotteryH5]             = new HandlerLotteryH5;
    _handlers[PBCSMsg::kCsReqLotteryEndH5]          = new HandlerLotteryEndH5;
    _handlers[PBCSMsg::kCsReqBuyPrizeH5]            = new HandlerBuyPrizeH5;
    _handlers[PBCSMsg::kCsReqPlayerLogoutH5]        = new HandlerPlayerLogoutH5;

    _handlers[PBSSMsg::kWebNotifyUpdateConfig]      = new HandlerUpdateConfig;
    _handlers[PBSSMsg::kWebReqPlayerLogin]          = new HandlerPlayerLogin;
    _handlers[PBSSMsg::kWebReqPlayerBuyIn]          = new HandlerPlayerBuyIn;
    _handlers[PBSSMsg::kWebNotifyPoint]             = new HandlerNotifyPoint;
    _handlers[PBSSMsg::kSsNotifyKickPlayer]         = new HandlerKickPlayer;
    _handlers[PBSSMsg::kSsNotifyPlayerGold]         = new HandlerPlayerGold;

    _handlers[PBInternalMsg::kInternalLogout]               = new HandlerLogout;
    _handlers[PBInternalMsg::kInternalClearBuyIn]           = new HandlerClearBuyIn;
    _handlers[PBInternalMsg::kInternalStartTimeout]         = new HandlerStartTimeout;
    _handlers[PBInternalMsg::kInternalEndTimeout]           = new HandlerEndTimeout;
    _handlers[PBInternalMsg::kInternalLotteryTimeout]       = new HandlerLotteryTimeout;
    _handlers[PBInternalMsg::kInternalLotteryEndTimeout]    = new HandlerLotteryEndTimeout;
    _handlers[PBInternalMsg::kInternalLotteryAniTimeout]    = new HandlerLotteryAniTimeout;
    _handlers[PBInternalMsg::kInternalBuyTimeout]           = new HandlerBuyTimeout;
    _handlers[PBInternalMsg::kInternalBuyAniTimeout]        = new HandlerBuyAniTimeout;
    _handlers[PBInternalMsg::kInternalGameRefund]           = new HandlerGameRefund;
    _handlers[PBInternalMsg::kInternalUpdateMachinePrize]   = new HandlerUpdateMachinePrize;
    _handlers[PBInternalMsg::kInternalGameRecord]           = new HandlerGameRecord;
}

void MessageManager::Process(Session * psession)
{
    if (!psession)
        return;

    if (_handlers.find(psession->msg_id()) != _handlers.end())
    {
        _handlers[psession->msg_id()]->Process(psession);
    }
}


