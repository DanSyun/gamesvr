#include "message_handler_data_g.h"

///////////////////////////////////////////////////////////////////////////////
// 获取DB数据
ENHandlerResult HandlerQueryDataG::OnMessage(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data_g();
    if (request.uids_size() == 0)
    {
        WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_g();
        response.set_result(EN_Web_Invalid_Params);
        MessageBase::SendMsgToWeb(psession);
        return EN_Handler_Done;
    }
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        if (request.uids(i) == EN_Uid_Lip_Prize)
            psession->AddQueryKey(request.uids(i), PBDBKey::kGLipPrizeInfo);
    }
    return EN_Handler_Query;
}

ENHandlerResult HandlerQueryDataG::OnQuerySuccess(Session * psession)
{
    const WebReqQueryDBData& request = psession->GetWebMsg().web_req_query_db_data_g();
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_g();
    response.set_result(EN_Web_OK);
    for (auto i = 0; i < request.uids_size(); ++i)
    {
        PBDBData* pdata = response.add_datas();
        pdata->CopyFrom(psession->GetDBData(i));
    }
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerQueryDataG::OnQueryFailed(Session * psession)
{
    WebRspQueryDBData& response = *psession->GetSSResponse().mutable_web_rsp_query_db_data_g();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

///////////////////////////////////////////////////////////////////////////////
// 修改DB数据
ENHandlerResult HandlerSaveDataG::OnMessage(Session * psession)
{
    const WebReqSaveDBData& request = psession->GetWebMsg().web_req_save_db_data_g();
    psession->AddQueryKey(request.uid(), PBDBKey::kMLipstickInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerSaveDataG::OnQuerySuccess(Session * psession)
{
    PBDBData& data = psession->GetDBData();
    const PBDBData& update = psession->GetWebMsg().web_req_save_db_data_g().data();
    return EN_Handler_Save;
}

ENHandlerResult HandlerSaveDataG::OnQueryFailed(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_g();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSaveDataG::OnSaveSuccess(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_g();
    response.set_result(EN_Web_OK);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSaveDataG::OnSaveFailed(Session * psession)
{
    WebRspSaveDBData& response = *psession->GetSSResponse().mutable_web_rsp_save_db_data_g();
    response.set_result(EN_Web_DB_Error);
    MessageBase::SendMsgToWeb(psession);
    return EN_Handler_Done;
}

