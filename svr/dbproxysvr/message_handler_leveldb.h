#pragma once

#include "message_handler_base.h"

class HandlerQuery: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
};

class HandlerSave: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
};

