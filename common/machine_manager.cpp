#include "machine_manager.h"
#include "serialize.hpp"
#include "timer_manager.h"

size_t Machine::GetByteSize()
{
    //pb len + pb bytes
    return sizeof(size_t) + base::ByteSize() + sizeof(_connect_head);
}

bool Machine::Serialize(uint8* buf, size_t size, size_t& offset)
{
    if (!SerializeBaseType(buf, size, offset, _connect_head))   return false;
    if (!SerializeProtoBuf(buf, size, offset, *this))           return false;

    return true;
}

bool Machine::UnSerialize(uint8* buf, size_t size, size_t& offset)
{
    if (!UnSerializeBaseType(buf, size, offset, _connect_head)) return false;
    if (!UnSerializeProtoBuf(buf, size, offset, *this))         return false;

    return true;
}

void Machine::RefreshTimer()
{
    Timer* ptimer = TimerManager::Instance()->GetTimer(base::timer_id());
    if (ptimer)
    {
        uint32 left_msec = (ptimer->timeout_msec() > GetTimeMsec()) ? (ptimer->timeout_msec() - GetTimeMsec()) : 0;
        uint32 timer_id = TimerManager::Instance()->CreateTimerMsec(left_msec + 1000, ptimer->msg());
        TimerManager::Instance()->ReleaseTimer(base::timer_id());
        base::set_timer_id(timer_id);
    }
}

bool MachineManager::Init(uint32 size, uint32 shmkey)
{
    if (!base::Init(size, shmkey)) return false;

    // ÖØ½¨Ë÷Òı
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        _index[iter->machine_id()] = iter.GetPos();
    }

    return true;
}

Machine* MachineManager::CreateMachine(uint64 machine_id)
{
    int32 pos = base::Malloc();
    if (pos == -1)
        return NULL;

    Machine* pmachine = base::Get(pos);
    pmachine->set_machine_id(machine_id);

    _index[machine_id] = pos;

    return pmachine;
}

Machine* MachineManager::GetMachine(uint64 machine_id)
{
    if (_index.find(machine_id) == _index.end())
        return NULL;

    return base::Get(_index[machine_id]);
}

bool MachineManager::ReleaseMachine(uint64 machine_id)
{
    if (_index.find(machine_id) == _index.end())
        return false;

    Machine* pmachine = base::Get(_index[machine_id]);
    pmachine->Clear();

    base::Free(_index[machine_id]);
    _index.erase(machine_id);

    return true;
}


