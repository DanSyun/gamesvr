#pragma once

#include "message_handler_base.h"

class HandlerLoginGameH5: public MessageHandlerBase
{
    enum state
    {
        state_wait_player_data      = 1,
        state_wait_promoter_data    = 2,
    };
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerStartGameH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerRetryGameH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerEndGameH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerLotteryH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerLotteryEndH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerBuyPrizeH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerPlayerLogoutH5: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

