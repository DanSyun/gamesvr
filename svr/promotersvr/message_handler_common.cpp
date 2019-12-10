#include "message_handler_common.h"

unordered_map<uint64, stPromoter> HandlerPromoterGold::_promoter_map;

ENHandlerResult HandlerPromoterGold::OnMessage(Session * psession)
{
    const SSNotifyPromoterGold& notify = psession->GetSSMsg().ss_notify_promoter_gold();
    if (_promoter_map.find(notify.promoter_id()) == _promoter_map.end())
    {
        stPromoter st_promoter;
        st_promoter.promoter_id = notify.promoter_id();
        st_promoter.giving_num = notify.giving_num();
        st_promoter.charge_num = notify.charge_num();
        st_promoter.count = 0;
        _promoter_map[notify.promoter_id()] = st_promoter;
        return EN_Handler_Done;
    }
    else
    {
        stPromoter& st_promoter = _promoter_map[notify.promoter_id()];
        st_promoter.count++;
        st_promoter.giving_num += notify.giving_num();
        st_promoter.charge_num += notify.charge_num();
        if (st_promoter.count == 10)
        {
            psession->AddQueryKey(notify.promoter_id(), PBDBKey::kPBaseInfo);
            return EN_Handler_Query;
        }
        else
        {
            return EN_Handler_Done;
        }
    }
}

ENHandlerResult HandlerPromoterGold::OnQuerySuccess(Session * psession)
{
    const SSNotifyPromoterGold& notify = psession->GetSSMsg().ss_notify_promoter_gold();
    if (_promoter_map.find(notify.promoter_id()) == _promoter_map.end())
    {
        return EN_Handler_Done;
    }

    stPromoter& st_promoter = _promoter_map[notify.promoter_id()];
    DBPBaseInfo& info = *psession->GetDBData().mutable_p_base_info();
    int64 total_charge = info.total_charge() + st_promoter.charge_num;
    int64 total_giving = info.total_giving() + st_promoter.giving_num;
    if (total_charge < 0) total_charge = 0;
    if (total_giving < 0) total_giving = 0;
    info.set_total_charge(total_charge);
    info.set_total_giving(total_giving);

    st_promoter.count = 0;
    st_promoter.charge_num = 0;
    st_promoter.giving_num = 0;

    return EN_Handler_Save;
}

ENHandlerResult HandlerPromoterGold::OnSaveSuccess(Session * psession)
{
    return EN_Handler_Done;
}

void HandlerPromoterGold::Init()
{
    uint32 shm_key = MessageBase::GetSvrType() * 100000 + MessageBase::GetSvrID() * 100 + 99;
    // shm
    ShareMem shm(shm_key);
    if (!shm.IsAttached()) return;

    uint8* buff = (uint8*)shm.GetPtr();
    size_t byte_size = shm.GetByteSize();
    size_t offset = 0;

    uint32 size = 0;
    UnSerializeBaseType(buff, byte_size, offset, size);

    for (uint32 i = 0; i < size; ++i)
    {
        stPromoter st_promoter;
        UnSerializeBaseType(buff, byte_size, offset, st_promoter);
        _promoter_map[st_promoter.promoter_id] = st_promoter;
    }

    shm.Detach();
    ShareMem::Delete(shm_key);
}

void HandlerPromoterGold::Release()
{
    uint32 shm_key = MessageBase::GetSvrType() * 100000 + MessageBase::GetSvrID() * 100 + 99;
    if (ShareMem::IsExist(shm_key) && !ShareMem::Delete(shm_key))
        return;

    size_t byte_size = 4;
    byte_size += _promoter_map.size() * sizeof(stPromoter);

    if (!ShareMem::Create(shm_key, byte_size))
        return;
    
    ShareMem shm(shm_key);
    uint8* buff = (uint8*)shm.GetPtr();
    size_t offset = 0;

    if (!SerializeBaseType(buff, byte_size, offset, _promoter_map.size())) return;
    for (auto iter = _promoter_map.begin(); iter != _promoter_map.end(); ++iter)
    {
        if (!SerializeBaseType(buff, byte_size, offset, iter->second))
            return;
    }

    shm.Detach();
}

