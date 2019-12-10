#pragma once

#include "message_handler_base.h"

class HandlerGMCommand: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnQueryFailed(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);
    ENHandlerResult OnSaveFailed(Session * psession);
private:
    struct stCommand
    {
        string  cmd;
        uint64  arg1;
        uint64  arg2;
    };
    void ProcessAddPrize(Session* psession, PBDBData& data, const stCommand& cmd);
    void ProcessAddGold(Session* psession, PBDBData& data, const stCommand& cmd);
};

class HandlerUpdateConfig: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
};

class HandlerPlayerLogin: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnServerResponse(Session * psession);
};

class HandlerPlayerBuyIn: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnServerResponse(Session * psession);
};

class HandlerDeleteMachine: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
};

class HandlerRestartMachine: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
};

