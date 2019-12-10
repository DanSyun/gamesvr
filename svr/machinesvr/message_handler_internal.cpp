#include "message_handler_internal.h"
#include "machine_manager.h"
#include "config.h"


///////////////////////////////////////////////////////////////////////////////
// ??????
ENHandlerResult HandlerLogout::OnMessage(Session * psession)
{
    uint64 uid = psession->GetInternalMsg().internal_logout().uid();
    psession->AddQueryKey(uid, PBDBKey::kMBaseInfo);
    return EN_Handler_Query;
}

ENHandlerResult HandlerLogout::OnQuerySuccess(Session * psession)
{
    PBDBData& machine = psession->GetDBData();
    machine.mutable_m_base_info()->clear_machinesvr_id();

    return EN_Handler_Save;
}

ENHandlerResult HandlerLogout::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

