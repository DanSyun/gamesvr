#pragma once

#include "message_base.h"
#include "machine_manager.h"

class Message: public MessageBase
{
public:
    static bool MessageFilter(stConnectHead& head, const char* buf, uint32 buf_len, PBSSMsg& msg);
};

