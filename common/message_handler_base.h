#pragma once

#include "message_base.h"
#include "session_manager.h"
#include "data_common.h"

enum ENHandlerResult
{
    EN_Handler_Done     = 1,    // 结束handler，session
    EN_Handler_Hup      = 2,    // 处理挂起，等待异步返回
    EN_Handler_Next     = 3,    // 处理下个uid数据
    EN_Handler_Failed   = 4,    // 处理失败
    EN_Handler_Query    = 5,    // 请求query
    EN_Handler_Save     = 6,    // 请求save
};

class MessageHandlerBase
{
public:
    void Process(Session* psession);

private:
    // 子类覆盖接口
    virtual ENHandlerResult OnMessage(Session* psession);           // 收到消息
    virtual ENHandlerResult OnServerResponse(Session* psession);    // svr异步返回
    virtual ENHandlerResult OnQuerySuccess(Session* psession);      // 处理db数据
    virtual ENHandlerResult OnSaveSuccess(Session* psession);       // db更新成功
    virtual ENHandlerResult OnQueryFailed(Session* psession);
    virtual ENHandlerResult OnSaveFailed(Session* psession);
    // 内部流程实现
    ENHandlerResult QueryAllData(Session* psession);
    ENHandlerResult SaveOneData(Session* psession);
    ENHandlerResult QueryResponse(Session* psession);
    ENHandlerResult SaveResponse(Session* psession);
    ENHandlerResult ProcessNext(Session* psession);
    void ProcessDone(Session* psession);
    void ProcessFailed(Session* psession);
    void OnPostMessage(Session* psession);
};

