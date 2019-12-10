#pragma once

#include "message_handler_base.h"

class HandlerLogout: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

