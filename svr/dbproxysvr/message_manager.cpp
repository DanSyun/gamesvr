#include "message_manager.h"
#include "message_handler_leveldb.h"

void MessageManager::Init()
{
    _handlers[PBSSMsg::kSsReqQuery]     = new HandlerQuery;
    _handlers[PBSSMsg::kSsReqSave]      = new HandlerSave;
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


