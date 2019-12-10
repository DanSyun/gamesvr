#include "message.h"

#include "cipher.hpp"
#include "base64.hpp"

bool Message::MessageFilter(stConnectHead& head, const char* buf, uint32 buf_len, PBSSMsg& msg)
{
    if (!ParseMsgFromServer(buf, buf_len, msg))
    {
        SendCommonErr(head, EN_Result_Invalid_Msg);
        return false;
    }

    return true;
}

