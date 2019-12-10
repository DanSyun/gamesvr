#pragma once

#include "message_handler_base.h"

class HandlerNotifyPlayerInfo: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerNotifyPoint: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
};

class HandlerPlayerCharge: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
    ENHandlerResult OnSaveFailed(Session * psession);
};

class HandlerSubPrize: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
    ENHandlerResult OnSaveFailed(Session * psession);
};

class HandlerQueryData: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
};

