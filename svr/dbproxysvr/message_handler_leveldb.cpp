#include "message_handler_leveldb.h"
#include "leveldb_manager.h"


ENHandlerResult HandlerQuery::OnMessage(Session * psession)
{
    const SSReqQuery& request = psession->GetSSMsg().ss_req_query();
    SSRspQuery& response = *psession->GetSSResponse().mutable_ss_rsp_query();

    for (auto i = 0; i < request.datas_size(); ++i)
    {
        if (!LeveldbManager::Instance()->Query(request.uid(), request.datas(i).key(), *response.add_datas()))
        {
            response.set_result(EN_KVDB_Failed);
            MessageBase::SendMsgToResponse(psession);
        }
    }

    response.set_result(EN_KVDB_Sucess);
    response.set_uid(request.uid());
    MessageBase::SendMsgToResponse(psession);
    return EN_Handler_Done;
}

ENHandlerResult HandlerSave::OnMessage(Session * psession)
{
    const SSReqSave& request = psession->GetSSMsg().ss_req_save();
    SSRspSave& response = *psession->GetSSResponse().mutable_ss_rsp_save();

    bool version_diff = false;
    for (auto i = 0; i < request.datas_size(); ++i)
    {
        uint32 version;
        if (!LeveldbManager::Instance()->GetVersion(request.uid(), request.datas(i).key(), version))
        {
            response.set_result(EN_KVDB_Failed);
            MessageBase::SendMsgToResponse(psession);
        }

        if (version + 1 != request.datas(i).version())
        {
            version_diff = true;
            break;
        }
    }

    // 保存数据
    if (version_diff == false)
    {
        for (auto i = 0; i < request.datas_size(); ++i)
        {
            if (!LeveldbManager::Instance()->Save(request.uid(), request.datas(i).key(), request.datas(i)))
            {
                response.set_result(EN_KVDB_Failed);
                MessageBase::SendMsgToResponse(psession);
            }
        }

        response.set_result(EN_KVDB_Sucess);
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
    // 返回新的db数据
    else
    {
        for (auto i = 0; i < request.datas_size(); ++i)
        {
            if (!LeveldbManager::Instance()->Query(request.uid(), request.datas(i).key(), *response.add_datas()))
            {
                response.set_result(EN_KVDB_Failed);
                MessageBase::SendMsgToResponse(psession);
            }
        }

        response.set_result(EN_KVDB_Ver_Diff);
        response.set_uid(request.uid());
        MessageBase::SendMsgToResponse(psession);
        return EN_Handler_Done;
    }
}

