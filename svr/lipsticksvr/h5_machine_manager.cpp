#include "jemalloc.h"
#include "message_base.h"
#include "h5_machine_manager.h"
#include "player_manager.h"

void H5MachineManager::Init()
{
    _size = PlayerManager::Instance()->MaxSize();
    _machine_arr = new char[_size];
    memset(_machine_arr, 0, _size);
    for (auto iter = PlayerManager::Instance()->Begin(); iter != PlayerManager::Instance()->End(); ++iter)
    {
        const Player& player = *iter;
        if (!player.machine().has_uid()) continue;
        uint64 machine_id = player.machine().uid();
        uint64 index = machine_id % 100000;
        if (index >= _size) continue;

        _machine_arr[index] = true;
    }
}

int32 H5MachineManager::Alloc()
{
    uint32 max_index = PlayerManager::Instance()->Size() + 5;
    if (max_index > _size) max_index = _size;

    vector<uint32> index_vec;
    for (uint32 i = 0; i < max_index; ++i)
    {
        if (!_machine_arr[i])
            index_vec.push_back(i);
    }

    if (index_vec.size() == 0) return -1;

    uint32 rand_num = rand() % index_vec.size();

    return 5000000 + MessageBase::GetSvrID() * 100000 + index_vec[rand_num];
}

void H5MachineManager::Release(uint64 machine_id)
{
    uint32 index = machine_id % 100000;
    if (index >= _size) return;
    _machine_arr[index] = false;
}

