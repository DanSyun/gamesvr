#include "message_manager.h"
#include "message_handler_common.h"
#include "message_handler_data.h"
#include "message_handler_data_m.h"
#include "message_handler_data_p.h"
#include "message_handler_data_g.h"

void MessageManager::Init()
{
    // GM����
    _handlers[PBSSMsg::kSsGmCommand]                = new HandlerGMCommand();

    // ���ò���
    _handlers[PBSSMsg::kWebNotifyUpdateConfig]      = new HandlerUpdateConfig();

    // �û����ݲ���
    _handlers[PBSSMsg::kWebNotifyPlayerInfo]        = new HandlerNotifyPlayerInfo();
    _handlers[PBSSMsg::kWebReqPlayerCharge]         = new HandlerPlayerCharge();
    _handlers[PBSSMsg::kWebReqSubPrize]             = new HandlerSubPrize();
    _handlers[PBSSMsg::kWebReqQueryDbData]          = new HandlerQueryData();
    _handlers[PBSSMsg::kWebNotifyPoint]             = new HandlerNotifyPoint();

    // �ֻ����ݲ���
    _handlers[PBSSMsg::kWebReqQueryDbDataM]         = new HandlerQueryDataM();
    _handlers[PBSSMsg::kWebReqSaveDbDataM]          = new HandlerSaveDataM();

    // �������ݲ���
    _handlers[PBSSMsg::kWebReqQueryDbDataP]         = new HandlerQueryDataP();
    _handlers[PBSSMsg::kWebReqSaveDbDataP]          = new HandlerSaveDataP();

    // ȫ�����ݲ���
    _handlers[PBSSMsg::kWebReqQueryDbDataG]         = new HandlerQueryDataG();
    _handlers[PBSSMsg::kWebReqSaveDbDataG]          = new HandlerSaveDataG();

    // �ֻ��ӿ�
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


