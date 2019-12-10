#include "message_manager.h"
#include "message_handler_common.h"
#include "message_handler_internal.h"

void MessageManager::Init()
{
    _handlers[PBCSMsg::kCsReqHeartBeat]     = new HandlerHeartBeat;
    _handlers[PBCSMsg::kCsReqMLogin]        = new HandlerMLogin;

    _handlers[PBSSMsg::kSsNotifyUpdateMachine]      = new HandlerUpdateMachine;
    _handlers[PBSSMsg::kWebNotifyDeleteMachine]     = new HandlerDeleteMachine;
    _handlers[PBSSMsg::kWebNotifyRestartMachine]    = new HandlerRestartMachine;

    _handlers[PBInternalMsg::kInternalLogout]       = new HandlerLogout;
}

void MessageManager::Process(Session * psession)
{
    if (!psession)
        return;

    if (_handlers.find(psession->msg_id()) != _handlers.end())
    {
        _handlers[psession->msg_id()]->Process(psession);
    }
}


