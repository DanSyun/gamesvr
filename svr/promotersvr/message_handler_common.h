#pragma once

#include "message_handler_base.h"

struct stPromoter
{
    uint64 promoter_id;
    int64 giving_num;
    int64 charge_num;
    uint16 count;
};
class HandlerPromoterGold: public MessageHandlerBase
{
    ENHandlerResult OnMessage(Session * psession);
    ENHandlerResult OnQuerySuccess(Session * psession);
    ENHandlerResult OnSaveSuccess(Session * psession);

private:
public:
    static unordered_map<uint64, stPromoter> _promoter_map;
    static void Init();
    static void Release();
};

