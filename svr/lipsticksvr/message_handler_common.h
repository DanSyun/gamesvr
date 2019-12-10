#pragma once

#include "message_handler_base.h"

class HandlerHeartBeat: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerMLoginGame: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerPlayerLogin: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerPlayerBuyIn: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerStartGame: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerRetryGame: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerEndGame: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerLottery: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerLotteryEnd: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerBuyPrize: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerPlayerLogout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

