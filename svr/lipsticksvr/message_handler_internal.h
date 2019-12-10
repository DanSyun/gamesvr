#pragma once

#include "message_handler_base.h"

class HandlerLogout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerClearBuyIn: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerStartTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerEndTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerLotteryTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerLotteryEndTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerLotteryAniTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerBuyTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerBuyAniTimeout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerGameRefund: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerUpdateMachinePrize: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerGameRecord: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult ProcessPlayer(Session* psession);
    ENHandlerResult ProcessGlobal(Session* psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

