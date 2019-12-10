#pragma once

#include "message_handler_base.h"

class HandlerHeartBeat: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerMLogin: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session* psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
};

class HandlerUpdateMachine: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerDeleteMachine: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerRestartMachine: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

