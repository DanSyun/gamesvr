#pragma once

#include "base.h"
#include "singleton.hpp"
#include "session_manager.h"
#include "message_handler_base.h"

class MessageManager: public Singleton<MessageManager>
{
public:
    void Init();
    void Process(Session* psession);
private:
    std::unordered_map<uint32, MessageHandlerBase*> _handlers;
};
