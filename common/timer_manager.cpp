#include "timer_manager.h"
#include "serialize.hpp"
#include "proto_log.hpp"
#include "message_base.h"

size_t Timer::GetByteSize()
{
    return sizeof(size_t) + base::ByteSize();
}

bool Timer::Serialize(uint8* buf, size_t size, size_t& offset)
{
    if (!SerializeProtoBuf(buf, size, offset, *this))         return false;

    return true;
}

bool Timer::UnSerialize(uint8* buf, size_t size, size_t& offset)
{
    if (!UnSerializeProtoBuf(buf, size, offset, *this))       return false;

    return true;
}

bool TimerManager::Init(uint32 size, uint32 shmkey)
{
    if (!base::Init(size, shmkey)) return false;

    // session id 从0开始
    _cur_max_id = 0;

    // 重建索引
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        uint32 timer_id = iter->timer_id();
        _index[timer_id] = iter.GetPos();
        if (timer_id > _cur_max_id)
            _cur_max_id = timer_id;
    }

    return true;
}

void TimerManager::Tick()
{
    std::vector<uint32> ids;
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        if (iter->timeout_msec() <= GetTimeMsec())
        {
            MessageBase::SendMsgToInternal(iter->msg());
            ids.push_back(iter->timer_id());
        }
    }
    for (auto iter = ids.begin(); iter != ids.end(); ++iter)
    {
        ReleaseTimer(*iter);
    }
}

int TimerManager::CreateTimer(uint32 sec, const PBInternalMsg& internal_msg)
{
    int32 pos = base::Malloc();
    if (pos == -1)
        return -1;

    // 生成timer_id
    uint32 timer_id;
    uint32 count = 0;
    while (true)
    {
        timer_id = ++_cur_max_id;
        if (_index.find(timer_id) == _index.end())
            break;
        count++;
        if (count >= 10)
            return -1;
    }
    _cur_max_id = _cur_max_id & 0xfffffff;

    Timer* ptimer = base::Get(pos);
    ptimer->set_timer_id(timer_id);
    ptimer->set_timeout_msec((uint64)(GetTime() + sec) * 1000);
    ptimer->mutable_msg()->CopyFrom(internal_msg);

    _index[timer_id] = pos;

    TRACE_PROTO(*ptimer, internal_msg.msg_union_case(), "CreateTimer");

    return timer_id;
}

int TimerManager::CreateTimerMsec(uint32 msec, const PBInternalMsg& internal_msg)
{
    int32 pos = base::Malloc();
    if (pos == -1)
        return -1;

    // 生成timer_id
    uint32 timer_id;
    uint32 count = 0;
    while (true)
    {
        timer_id = ++_cur_max_id;
        if (_index.find(timer_id) == _index.end())
            break;
        count++;
        if (count >= 10)
            return -1;
    }
    _cur_max_id = _cur_max_id & 0xfffffff;

    Timer* ptimer = base::Get(pos);
    ptimer->set_timer_id(timer_id);
    ptimer->set_timeout_msec(GetTimeMsec() + msec);
    ptimer->mutable_msg()->CopyFrom(internal_msg);

    _index[timer_id] = pos;

    TRACE_PROTO(*ptimer, internal_msg.msg_union_case(), "CreateTimerMsec");

    return timer_id;
}

Timer* TimerManager::GetTimer(uint64 timer_id)
{
    if (_index.find(timer_id) == _index.end())
        return NULL;

    return base::Get(_index[timer_id]);
}

bool TimerManager::ReleaseTimer(uint32 timer_id)
{
    if (_index.find(timer_id) == _index.end())
        return false;

    Timer* ptimer = base::Get(_index[timer_id]);
    ptimer->Clear();

    base::Free(_index[timer_id]);
    _index.erase(timer_id);

    return true;
}

