#pragma once

#include "message_handler_base.h"

class HandlerQueryDataG: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
};

class HandlerSaveDataG: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
    ENHandlerResult OnSaveFailed(Session * psession);
};

