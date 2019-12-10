#include "message_manager.h"
#include "message_handler_common.h"
#include "message_handler_data.h"
#include "message_handler_data_m.h"
#include "message_handler_data_p.h"
#include "message_handler_data_g.h"

void MessageManager::Init()
{
    // GM命令
    _handlers[PBSSMsg::kSsGmCommand]                = new HandlerGMCommand();

    // 配置操作
    _handlers[PBSSMsg::kWebNotifyUpdateConfig]      = new HandlerUpdateConfig();

    // 用户数据操作
    _handlers[PBSSMsg::kWebNotifyPlayerInfo]        = new HandlerNotifyPlayerInfo();
    _handlers[PBSSMsg::kWebReqPlayerCharge]         = new HandlerPlayerCharge();
    _handlers[PBSSMsg::kWebReqSubPrize]             = new HandlerSubPrize();
    _handlers[PBSSMsg::kWebReqQueryDbData]          = new HandlerQueryData();
    _handlers[PBSSMsg::kWebNotifyPoint]             = new HandlerNotifyPoint();

    // 街机数据操作
    _handlers[PBSSMsg::kWebReqQueryDbDataM]         = new HandlerQueryDataM();
    _handlers[PBSSMsg::kWebReqSaveDbDataM]          = new HandlerSaveDataM();

    // 代理数据操作
    _handlers[PBSSMsg::kWebReqQueryDbDataP]         = new HandlerQueryDataP();
    _handlers[PBSSMsg::kWebReqSaveDbDataP]          = new HandlerSaveDataP();

    // 全局数据操作
    _handlers[PBSSMsg::kWebReqQueryDbDataG]         = new HandlerQueryDataG();
    _handlers[PBSSMsg::kWebReqSaveDbDataG]          = new HandlerSaveDataG();

    // 街机接口
    _handlers[PBSSMsg::kWebReqPlayerLogin]          = new HandlerPlayerLogin();
    _handlers[PBSSMsg::kWebReqPlayerBuyIn]          = new HandlerPlayerBuyIn();
    _handlers[PBSSMsg::kWebNotifyDeleteMachine]     = new HandlerDeleteMachine();
    _handlers[PBSSMsg::kWebNotifyRestartMachine]    = new HandlerRestartMachine();
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


