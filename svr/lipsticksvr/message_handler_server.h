#pragma once

#include "message_handler_base.h"

class HandlerUpdateConfig: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerKickPlayer: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerPlayerGold: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerNotifyPoint: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

