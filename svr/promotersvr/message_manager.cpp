#include "message_manager.h"
#include "message_handler_common.h"

void MessageManager::Init()
{
    _handlers[PBSSMsg::kSsNotifyPromoterGold]       = new HandlerPromoterGold;
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


